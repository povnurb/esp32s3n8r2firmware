// declaracion
void contadorAlarmas();
void activarAlarma();
void resetIntLoop();
void fechaRlay();
void mostrar();
void ctrlRelays();
float humBme280();
float tempbme280();

// tarea de la reconexion del wifi
void TaskWifiReconnect(void *pvParameters)
{
    (void)pvParameters;
    while (true)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        if (wifi_app == WIFI_STA) // TODO: o WIFI_AP_STA en tareas.hpp y en wifi.hpp
        {
            wifiSTALoop();
        }
        else if (wifi_app == WIFI_AP)
        {
            wifiAPLoop();
        }
    }
}

// tarea para el broker MQTT
void TaskMQTTReconnect(void *pvParameters)
{
    (void)pvParameters;
    while (true)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        if (WiFi.status() == WL_CONNECTED && (wifi_app == WIFI_STA))
        {
            if (mqtt_server != 0)
            {
                mqttLoop(); // loop mqtt
                // enviar un mensaje por mqtt
                if (mqttClient.connected() && mqtt_status_send)
                {
                    if (millis() - lastMsg > mqtt_time_interval)
                    {
                        lastMsg = millis();
                        // llamar la funcion de pubilcar
                        mqtt_publish();
                        // myLog("INFO", "tareas.hpp", "TaskMQTTReconnect()", "Mensaje enviado por MQTT");
                    }
                }
            }
        }
    }
}

// tarea para el pestaño de Led MQTT
void TaskLedMqttReconnect(void *pvParameters)
{
    (void)pvParameters;
    // desabilitar el washdog con esto ya no se ejecuta el perro guardian dentro de esta tarea
    disableCore0WDT();                // el core 0
    unsigned long previousMillis = 0; // tiempo anterior
    const long ledOnTime = 50;        // tiempo encendido en milisegundos
    const long ledOfftime = 1000;     // tiempo apagado  para acompletar un segundo
    bool ledState = LOW;

    while (true)
    {
        if (mqtt_enabled && mqttClient.connected())
        { // si esta conectado
            unsigned long currentMillis = millis();
            if (ledState == LOW && (currentMillis - previousMillis >= ledOfftime))
            {
                ledState = HIGH;
                NeoLedPixel(50, 0, 0, 255);     // encendemos el neopixel en color AZUL
                previousMillis = currentMillis; // reiniciamos el temporizador
            }
            else if (ledState == HIGH && (currentMillis - previousMillis >= ledOnTime))
            {
                ledState = LOW;
                NeoLedPixel(0, 0, 0, 0);        // apagamos el neopixel
                previousMillis = currentMillis; // reiniciamos el temporizador
            }
        }
        else
        {
            NeoLedPixel(0, 0, 0, 0);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// tareas que actualizan variables importantes cada 1 segundo
void TaskActualiza1seg(void *pvParameters)
{
    (void)pvParameters;
    // desabilitar el washdog con esto ya no se ejecuta el perro guardian dentro de esta tarea
    while (true)
    {
        // device es una variable que guarda el estado
        device_relay01 = RELAY1_STATUS ? true : false;
        device_relay02 = RELAY2_STATUS ? true : false;
        device_dimmer = dim;
        // templm35 = sensorLm35();//FIXME: se quitan mientras no exista sensor
        // device_lm35 = String(templm35);//FIXME: se quitan mientras no exista sensor
        device_tempEvaporador = String(readSensorPozo(SENSOREVAPORADOR) + ajTmpP1);
        temp1 = readSensorPozo(SENSOREVAPORADOR) + ajTmpP1;
        device_tempCondensador = String(readSensorPozo(SENSORCONDENSADOR) + ajTmpP2);
        temp2 = readSensorPozo(SENSORCONDENSADOR) + ajTmpP2;
        // TODO: agregar las temperaturas del pozo1 y pozo2 asi como del lm35
        device_ativeTime = longTimeStr(millis() / 1000);
        device_rssi = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
        device_quality = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
        if (!prueba)
        {
            ALARM_STATUS1 = statusAlarm1();
            ALARM_STATUS2 = statusAlarm2();
            ALARM_STATUS3 = statusAlarm3();
            ALARM_STATUS4 = statusAlarm4();
            ALARM_STATUS5 = statusAlarm5();
            ALARM_STATUS6 = statusAlarm6();
            ALARM_STATUS7 = statusAlarm7();
            ALARM_STATUS8 = statusAlarm8();
        }
        else
        {
            ALARM_LOGICA1 ? ALARM_STATUS1 = true : ALARM_STATUS1 = false;
            ALARM_LOGICA2 ? ALARM_STATUS2 = true : ALARM_STATUS2 = false;
            ALARM_LOGICA3 ? ALARM_STATUS3 = true : ALARM_STATUS3 = false;
            ALARM_LOGICA4 ? ALARM_STATUS4 = true : ALARM_STATUS4 = false;
            ALARM_LOGICA5 ? ALARM_STATUS5 = true : ALARM_STATUS5 = false;
            ALARM_LOGICA6 ? ALARM_STATUS6 = true : ALARM_STATUS6 = false;
            ALARM_LOGICA7 ? ALARM_STATUS7 = true : ALARM_STATUS7 = false;
            ALARM_LOGICA8 ? ALARM_STATUS8 = true : ALARM_STATUS8 = false;
        }
        device_fecha = getDateTime();
        activarAlarma();
        actRele();        // verifica si hay que activar el relevador
        fechaRlay();      // indica el horario de activacion y desactivacion
        programaRelays(); // indica los dias de activacion de los relays
        ctrlRelays();     // ejecuta el tiempo que debe activarse el relevador
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// tareas que actualizan variables cada 2 segundos para el DHT22
// version IA1 pero la primer funcion se ejecuta 2 veces
/*

void TaskActualiza2seg(void *pvParameters)
{
    (void)pvParameters;

    unsigned long previousMillisTemp = 0;
    unsigned long previousMillisHum = 0;
    const unsigned long interval = 2000; // 2 segundos

    while (true)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillisTemp >= interval)
        {
            previousMillisTemp = currentMillis;
            device_tempDHT = String(Temperatura());
        }

        if (currentMillis - previousMillisHum >= 2 * interval)
        { // 4 segundos desde el inicio
            previousMillisHum = currentMillis;
            device_humedadDHT = String(Humedad());
        }

        vTaskDelay(10 / portTICK_PERIOD_MS); // Pequeño retardo para evitar el "task starvation"
    }
}
*/
// version IA2
/*
void TaskActualiza2seg(void *pvParameters)
{
    (void)pvParameters;

    while (true)
    {
        device_tempDHT = String(Temperatura());
        vTaskDelay(2501 / portTICK_PERIOD_MS); // Espera 2 segundos
    }
}*/
/*void TaskActualiza2Humseg(void *pvParameters)
{
    (void)pvParameters;
    disableCore0WDT(); // asegurate que esta tarea esta en el core 0
    while (true)
    {
        device_humedadDHT = String(Humedad());
        vTaskDelay(3100 / portTICK_PERIOD_MS); // Espera 2 segundos
    }
}
*/
// tercera version IA
void TaskActualiza2seg(void *pvParameters)
{
    (void)pvParameters;
    // disableCore0WDT(); //no finciono por eso se quito
    unsigned long previousMillis = 0;
    const unsigned long interval = 3500; // 3 segundos
    bool isTempNext = true;

    while (true)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;
            if (!sensorTempBME280)
            {
                if (isTempNext)
                {
                    device_tempDHT = String(Temperatura());
                    // Serial.print("Temperatura: ");
                    // Serial.println(device_tempDHT);
                }
                else
                {
                    device_humedadDHT = String(Humedad());
                    // Serial.print("Humedad: ");
                    // Serial.println(device_humedadDHT);
                }
            }
            else
            {
                if (isTempNext)
                {
                    device_tempDHT = String(tempbme280());
                    // Serial.print("Temperatura: ");
                    // Serial.println(device_tempDHT);
                }
                else
                {
                    device_humedadDHT = String(humBme280());
                    // Serial.print("Humedad: ");
                    // Serial.println(device_humedadDHT);
                }
            }

            isTempNext = !isTempNext; // Alterna entre temperatura y humedad
        }

        vTaskDelay(200 / portTICK_PERIOD_MS); // Pequeño retardo para evitar el "task starvation"
    }
}

/*void TaskActualiza2seg(void *pvParameters)
{

    (void)pvParameters;

    while (true)
    {

        device_tempDHT = String(Temperatura());
        device_humedadDHT = String(Humedad());
        //device_tempMinima = String(min2);
        //device_tempMaxima = String(max2);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }
}
*/
// tareas que actualizan variables menos importantes cada 30 segundo
void TaskActualiza10seg(void *pvParameters)
{
    (void)pvParameters;
    // desabilitar el washdog con esto ya no se ejecuta el perro guardian dentro de esta tarea
    while (true)
    {
        if (!rtcOk)
        {
            myLog("INFO", "tareas.hpp", "TaskActualiza30seg(void *pvParameters)", "La fecha no es correcta, verificar conexion con el temporizador GPIO4 y GPIO5 o su alimentación y reiniciar.");
        }
        contadorAlarmas(); // contador de alarmas se actualizara cada 30 segundos lo que asegura que es real
        // para evitar que si una alarma no se quita o no se pone contabilice
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void TaskTimeGrafica(void *pvParameters)
{
    (void)pvParameters;
    while (1)
    {
        muestra();
        // vTaskDelay(2000 / portTICK_PERIOD_MS); // para que de 2 segundo
        vTaskDelay(60000 / portTICK_PERIOD_MS); // para 10 minuto portTICK_PERIOD_MS
    }
}

//----------------------------------------------------
// tarea para verificar si se requiere restaurar de fabrica
//-----------------------------------------------------------
void TaskRestore(void *pvParameters)
{

    (void)pvParameters;
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // esto es igual a 3 segundo lo cual hara lo siguiente:
        resetIntLoop();
    }
}

// tarea para el webSocket para manejar los mensajes
void TaskWsSend(void *pvParameters)
{
    (void)pvParameters;
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // se ejecutara coda 1 seg
        // verifica si hay datos para enviar
        wsMessageSend();
    }
}
void TaskLCD(void *pvParameters)
{
    (void)pvParameters;
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // se ejecutara coda 1 seg
        // verifica si hay datos para enviar
        mostrar();
    }
}