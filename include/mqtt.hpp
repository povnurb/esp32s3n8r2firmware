

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
// declaracion de utilizacion de funciones
bool statusAlarm1();
bool statusAlarm2();
bool statusAlarm3();
bool statusAlarm4();
bool statusAlarm5();
bool statusAlarm6();
bool statusAlarm7();
bool statusAlarm8();

void mqtt_response(const String &jsonMessage);
bool validateCommandJsonKeys(const String &jsonData);
bool apiPostControlDevice(const String &command);

char topic[150];
long lastMqttReconnectAttempt = 0;
long lastMsg = 0;

// menojo de los mensajes entrantes
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String command = "";
    String str_topic(topic);
    for (int16_t i = 0; i < length; i++)
    {
        command += (char)payload[i];
    }

    command.trim();
    String msgTopic = "Tópico MQTT --> " + str_topic;
    String msgCommand = "Mensaje MQTT --> " + command;
    myLog("INFO", "mqtt.hpp", "mqttCallback()", msgTopic.c_str());
    myLog("INFO", "mqtt.hpp", "mqttCallback()", msgCommand.c_str());

    /* TODO: actuar con los comando que no llegan ejemplos
    {"protocol":"MQTT","output":"RELAY1","value":true}
    {"protocol":"MQTT","output":"RELAY2","value":false}
    {"protocol":"MQTT","output":"BUZZER","value":true}
    {"protocol":"MQTT","output":"DIMMER","value":50}*/
    // al topico: coatza/coaigl/uninet2/A0B8/command
    JsonDocument jsonCommand;
    DeserializationError error = deserializeJson(jsonCommand, command);
    if (error)
    {
        mqtt_response("{\"status\":false,\"msg\":\"¡Error,no es un formato JSON!\"}");
        myLog("ERROR", "mqtt.hpp", "mqttCallback()", "Error al deserializar el commando");
        return;
    }
    // validar el Json de los comandos
    if (!validateCommandJsonKeys(command))
    {
        mqtt_response("{\"status\":false, \"msg\":\"Parámetros enviados incorrectos\"}");
        myLog("ERROR", "mqtt.hpp", "mqttCallback()", "Parámetros enviados incorrectos");
        return;
    }
    // ejecutar el comando desde la funcion unificada
    if (apiPostControlDevice(command)) // se reutiliza por lo tanto seria bueno cambiar el nombre a apiPostAndMqttControlDevice
    {
        mqtt_response("{ \"status\":true, \"output\":\"" + jsonCommand["output"].as<String>() + "\",\"value\":\"" + jsonCommand["value"].as<String>() + "\"}");
        myLog("INFO", "mqtt.hpp", "apiPostControlDevice(command)", "Comando ejecutado correctamente");
    }
    else
    {
        mqtt_response("{\"status\":false, \"msg\":\"Comando no permitido\"}");
        myLog("INFO", "mqtt.hpp", "apiPostControlDevice(command)", "Comando no permitido");
    }
}
// subscribe
void SubscribeMqtt()
{
    if (mqttClient.subscribe(mqtt_commandTopic, mqtt_qos))
    {
        String message = "Suscrito al tópico: " + String(mqtt_commandTopic);
        myLog("INFO", "mqtt.hpp", "SubscribeMqtt()", message.c_str());
    }
    else
    {
        myLog("ERROR", "mqtt.hpp", "SubscribeMqtt()", "MQTT - Falló la suscripción");
    }
}

// mqtt connect
boolean connectMqtt()
{
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(mqttCallback);
    myLog("INFO", "mqtt.hpp", "connectMqtt()", "Intentando conexión al broker MQTT ....");
    Serial.println(mqtt_server);
    // topico para mensajes retenidos
    const char *willTopic = PathMqttTopic("status").c_str();
    if (mqttClient.connect(mqtt_id, mqtt_user, mqtt_password, willTopic, mqtt_willQos, mqtt_willRetain, mqtt_willMessage, mqtt_clean_session))
    {
        myLog("INFO", "mqtt.hpp", "connectMqtt()", "ESP32S3 Conectado al broker MQTT");
        // setear el bufer
        mqttClient.setBufferSize(1024 * 5); // 5KBytes
        char buffer[100];
        sprintf(buffer, "MQTT buffer size: %d Bytes", mqttClient.getBufferSize());
        myLog("INFO", "mqtt.hpp", "connectMqtt()", buffer);
        SubscribeMqtt();
        if (mqtt_status_send)
        {
            UltimaConexion = fechaActual(); // Fecha de ultima conexion Certera de MQTT
            mqttClient.publish(willTopic, "{\"Conectado\":true}", mqtt_retain);
        }
        return (1);
    }
    else
    {
        // manejo del error
        String message = "MQTT - fallo " + getErrorMqttMessage(mqttClient.state());
        myLog("ERROR", "mqtt.hpp", "connectMqtt()", message.c_str());
        return (0);
    }
}

// Funcion que genera el json para enviar por mqtt
String JsonMsg()
{
    String response;
    JsonDocument jsonDoc;
    jsonDoc["UltimaConexion"] = UltimaConexion; // solo se ejecuta cuando se conecta a MQTT
    jsonDoc["FechaMsg"] = FechaMsgMqtt;         // cada 1 seg es el tiempo en el que envio el ultimo mensaje al broker
    jsonDoc["RedWiFi"] = WifiSsidMqtt;          // una sola vez cuando se conecta a la red WiFi ejemplo: "INFINITUMD378"
    jsonDoc["ip"] = IpMqtt;                     // ip del dispositivo una sola vez cuando se conecta a la red WiFi
    //  crear el json con informacion que solo se ejecuta una sola vez
    jsonDoc["device_serial"] = deviceSerial;
    jsonDoc["central"] = central;
    jsonDoc["sala"] = sala;
    jsonDoc["device_manufacture"] = deviceManofactura;
    jsonDoc["device_firmware"] = device_fw_version;
    jsonDoc["device_hardware"] = String(device_hw_version);
    jsonDoc["device_sdk"] = sdkVersion;
    jsonDoc["prueba_de_alarmas"] = prueba;
    jsonDoc["telegram"] = telegram;
    jsonDoc["whatsapp"] = whatsapp;
    // data con informacion que actualizaremos cada 1 segundo
    JsonObject dataObj = jsonDoc["data"].to<JsonObject>();
    dataObj["device_dimmer"] = device_dimmer;
    dataObj["device_lm35"] = device_lm35;
    dataObj["device_ajuste_lm35"] = ajTmpLm35;
    dataObj["device_tempEvaporador"] = device_tempEvaporador;
    dataObj["device_ajuste_tempEvaporador"] = ajTmpP1;
    dataObj["device_tempCondensador"] = device_tempCondensador;
    dataObj["device_ajuste_tempCondensador"] = ajTmpP2;
    dataObj["device_tempDHT"] = device_tempDHT;
    dataObj["device_ajuste_tempDHT"] = ajTmpDht22;
    dataObj["device_humedadDHT"] = device_humedadDHT;
    dataObj["device_tempMinima"] = device_tempMinima;
    dataObj["device_tempMaxima"] = device_tempMaxima;
    dataObj["device_activeTime"] = device_ativeTime;
    dataObj["device_restart"] = device_restart;
    dataObj["device_rssi"] = device_rssi;
    dataObj["device_quality"] = device_quality;
    // ---------------------------------------------------
    // Zona de alarmas
    // ---------------------------------------------------
    JsonObject alarm1Obj = jsonDoc["Alarma1"].to<JsonObject>();
    alarm1Obj["ALARM_NAME1"] = ALARM_NAME1;
    alarm1Obj["ALARM_LOGICA1"] = ALARM_LOGICA1;
    alarm1Obj["ALARM_STATUS1"] = ALARM_STATUS1;
    alarm1Obj["ALARM_RECONOCIDA1"] = ALARM_RECONOCIDA1;
    alarm1Obj["ALARM_TIMEON1"] = ALARM_TIMEON1;
    alarm1Obj["ALARM_TIMEOFF1"] = ALARM_TIMEOFF1;
    alarm1Obj["ALARM_CONT1"] = ALARM_CONT1;
    JsonObject alarm2Obj = jsonDoc["Alarma2"].to<JsonObject>();
    alarm2Obj["ALARM_NAME2"] = ALARM_NAME2;
    alarm2Obj["ALARM_LOGICA2"] = ALARM_LOGICA2;
    alarm2Obj["ALARM_STATUS2"] = ALARM_STATUS2;
    alarm2Obj["ALARM_RECONOCIDA2"] = ALARM_RECONOCIDA2;
    alarm2Obj["ALARM_TIMEON2"] = ALARM_TIMEON2;
    alarm2Obj["ALARM_TIMEOFF2"] = ALARM_TIMEOFF2;
    alarm2Obj["ALARM_CONT2"] = ALARM_CONT2;
    JsonObject alarm3Obj = jsonDoc["Alarma3"].to<JsonObject>();
    alarm3Obj["ALARM_NAME3"] = ALARM_NAME3;
    alarm3Obj["ALARM_LOGICA3"] = ALARM_LOGICA3;
    alarm3Obj["ALARM_STATUS3"] = ALARM_STATUS3;
    alarm3Obj["ALARM_RECONOCIDA3"] = ALARM_RECONOCIDA3;
    alarm3Obj["ALARM_TIMEON3"] = ALARM_TIMEON3;
    alarm3Obj["ALARM_TIMEOFF3"] = ALARM_TIMEOFF3;
    alarm3Obj["ALARM_CONT3"] = ALARM_CONT3;
    JsonObject alarm4Obj = jsonDoc["Alarma4"].to<JsonObject>();
    alarm4Obj["ALARM_NAME4"] = ALARM_NAME4;
    alarm4Obj["ALARM_LOGICA4"] = ALARM_LOGICA4;
    alarm4Obj["ALARM_STATUS4"] = ALARM_STATUS4;
    alarm4Obj["ALARM_RECONOCIDA4"] = ALARM_RECONOCIDA4;
    alarm4Obj["ALARM_TIMEON4"] = ALARM_TIMEON4;
    alarm4Obj["ALARM_TIMEOFF4"] = ALARM_TIMEOFF4;
    alarm4Obj["ALARM_CONT4"] = ALARM_CONT4;
    JsonObject alarm5Obj = jsonDoc["Alarma5"].to<JsonObject>();
    alarm5Obj["ALARM_NAME5"] = ALARM_NAME5;
    alarm5Obj["ALARM_LOGICA5"] = ALARM_LOGICA5;
    alarm5Obj["ALARM_STATUS5"] = ALARM_STATUS5;
    alarm5Obj["ALARM_RECONOCIDA5"] = ALARM_RECONOCIDA5;
    alarm5Obj["ALARM_TIMEON5"] = ALARM_TIMEON5;
    alarm5Obj["ALARM_TIMEOFF5"] = ALARM_TIMEOFF5;
    alarm5Obj["ALARM_CONT5"] = ALARM_CONT5;
    JsonObject alarm6Obj = jsonDoc["Alarma6"].to<JsonObject>();
    alarm6Obj["ALARM_NAME6"] = ALARM_NAME6;
    alarm6Obj["ALARM_LOGICA6"] = ALARM_LOGICA6;
    alarm6Obj["ALARM_STATUS6"] = ALARM_STATUS6;
    alarm6Obj["ALARM_RECONOCIDA6"] = ALARM_RECONOCIDA6;
    alarm6Obj["ALARM_TIMEON6"] = ALARM_TIMEON6;
    alarm6Obj["ALARM_TIMEOFF6"] = ALARM_TIMEOFF6;
    alarm6Obj["ALARM_CONT6"] = ALARM_CONT6;
    JsonObject alarm7Obj = jsonDoc["Alarma7"].to<JsonObject>();
    alarm7Obj["ALARM_NAME7"] = ALARM_NAME7;
    alarm7Obj["ALARM_LOGICA7"] = ALARM_LOGICA7;
    alarm7Obj["ALARM_STATUS7"] = ALARM_STATUS7;
    alarm7Obj["ALARM_RECONOCIDA7"] = ALARM_RECONOCIDA7;
    alarm7Obj["ALARM_TIMEON7"] = ALARM_TIMEON7;
    alarm7Obj["ALARM_TIMEOFF7"] = ALARM_TIMEOFF7;
    alarm7Obj["ALARM_CONT7"] = ALARM_CONT7;
    JsonObject alarm8Obj = jsonDoc["Alarma8"].to<JsonObject>();
    alarm8Obj["ALARM_NAME8"] = ALARM_NAME8;
    alarm8Obj["ALARM_LOGICA8"] = ALARM_LOGICA8;
    alarm8Obj["ALARM_STATUS8"] = ALARM_STATUS8;
    alarm8Obj["ALARM_RECONOCIDA8"] = ALARM_RECONOCIDA8;
    alarm8Obj["ALARM_TIMEON8"] = ALARM_TIMEON8;
    alarm8Obj["ALARM_TIMEOFF8"] = ALARM_TIMEOFF8;
    alarm8Obj["ALARM_CONT8"] = ALARM_CONT8;
    // TODO:agregar con sus variables
    JsonObject r1Obj = jsonDoc["TimerRelay1"].to<JsonObject>();
    r1Obj["device_relay01"] = device_relay01; // es el estado del relevador1 (ON u OFF)
    r1Obj["R_NAME1"] = String(R_NAME1);
    r1Obj["R_TIMERON1"] = R_TIMERON1; // si debe se encenderse por algun tiempo
    r1Obj["R_TIMER1"] = R_TIMER1;     // Que tiempo
    r1Obj["R_DESCRIPTION1"] = R_DESCRIPTION1;
    r1Obj["TEMPORIZADOR1"] = TEMPORIZADOR1;   // si tienes un horario de encendido o apagado
    r1Obj["TIMEONRELAY1A"] = TIMEONRELAY1A;   // a que horas enciende
    r1Obj["TIMEONRELAY1B"] = TIMEONRELAY1B;   // a que horas enciende
    r1Obj["TIMEOFFRELAY1A"] = TIMEOFFRELAY1A; // a que horas se apaga
    r1Obj["TIMEOFFRELAY1B"] = TIMEOFFRELAY1B; // a que horas se apaga
    r1Obj["FECHAON1"] = FECHAON1;             // la hora que se encendio
    r1Obj["FECHAOFF1"] = FECHAOFF1;           // la hora que se apago
    r1Obj["programado1"] = programado1;       // indica si esta programado

    JsonObject r2Obj = jsonDoc["TimerRelay2"].to<JsonObject>();
    r2Obj["device_relay02"] = device_relay02; // es el estado del relevador2(ON u OFF)
    r2Obj["R_NAME2"] = String(R_NAME2);       // GPIO 18
    r2Obj["R_TIMERON2"] = R_TIMERON2;
    r2Obj["R_TIMER2"] = R_TIMER2;
    r2Obj["R_DESCRIPTION2"] = R_DESCRIPTION2;
    r2Obj["TEMPORIZADOR2"] = TEMPORIZADOR2;
    r2Obj["TIMEONRELAY2A"] = TIMEONRELAY2A;
    r2Obj["TIMEONRELAY2B"] = TIMEONRELAY2B;
    r2Obj["TIMEOFFRELAY2A"] = TIMEOFFRELAY2A;
    r2Obj["TIMEOFFRELAY2B"] = TIMEOFFRELAY2B;
    r2Obj["FECHAON2"] = FECHAON2;       // la hora que se encendio
    r2Obj["FECHAOFF2"] = FECHAOFF2;     // la hora que se apago
    r2Obj["programado2"] = programado2; // indica si esta programado

    // EL BUZZER NO DEBE GUARDARSE en el SPIFFS
    JsonObject buzObj = jsonDoc["buzzer"].to<JsonObject>();
    buzObj["buzzer_status"] = BUZZER_STATUS ? "true" : "false";

    // mete el jsonDoc en response
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// Mqtt respuesta al broker
void mqtt_response(const String &jsonMessage)
{
    mqttClient.publish(mqtt_sendTopic, jsonMessage.c_str(), mqtt_retain);
}

// publicar
void mqtt_publish()
{
    FechaMsgMqtt = fechaActual(); // antes de publicar actualiza la fecha de publicacion
    mqttClient.publish(mqtt_sendTopic, JsonMsg().c_str(), mqtt_retain);
}

// Loop MQTT
void mqttLoop()
{
    if (mqtt_enabled)
    {
        if (!mqttClient.connected())
        {
            long now = millis();
            // intentar la reconexión
            blinkNeoPixelAsy(100, 1000, "rojo"); // rojo que significa que un no se conecta a MQTT
            if (now - lastMqttReconnectAttempt > 5000)
            {
                lastMqttReconnectAttempt = now;
                if (connectMqtt())
                {
                    lastMqttReconnectAttempt = 0;
                }
            }
        }
        else
        {
            mqttClient.loop(); // escuchar los mensajes del MQTT
        }
    }
}