// definicion de funciones
boolean settingsRead();
boolean settingsSave();
void settingsReset();

// funcion que leera el archivo settings.json devolviendo un true o false
//  documentación www.arduinojson.org/v7/
//  la nueva version de JSON ya calcula el tamaño (excelente)

boolean settingsRead()
{
    JsonDocument jsonSettings;                            // objeto jsonSettings donde se leeran las configuraciones iniciales
    File file = SPIFFS.open("/settings.json", FILE_READ); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    { // instalar la extencion TODO Highlight
        // FIXME: Serial.println("[WARNING][settings.hpp][settingsRead()]Falla en abrir el archivo settings.json");
        // dejare ese FIXME como ejemplo pero la siguiente linea es la solucion
        myLog("ERROR", "settings.hpp", "settingsRead()", "Error al abrir el archivo settings.json o este no existe");
        // TODO: llamar a la función de iniciar el json de fabrica (HECHO)
        settingsReset();
        return false;
    }
    // Deserealizacion del archivo settings.json osea, lo que esta en el archivo file
    // lo pazamos a la variable jsonSettings gracias a la funcion deserializeJson() y
    // si eso no sucede dará un error
    // la deseralizeJson() si todo sale ok devuelve un 0 o false indicando que no hay error
    // en caso contrario da
    DeserializationError error = deserializeJson(jsonSettings, file);
    // cerramos el archivo este paso es necesario ya que ya no o usaremos y con esto liberamos memoria
    file.close();
    // si se presenta un error
    if (error)
    {
        myLog("ERROR", "settings.hpp", "settingsRead()", "Fallo la deserealización del archivo settings.json");
        settingsReset();
        return false;
    }
    myLog("INFO", "settings.hpp", "settingsRead()", "Se procede a asignar valores almacenados a las variables");
    // si ya pasamos todos esos errores, pasaremos a copiar los valores del archivo json a las variables globales
    // como son de tipo char
    telegram = jsonSettings["telegram"] | false;
    botToken = jsonSettings["botToken"].as<String>();
    chatID = jsonSettings["chatID"].as<String>();
    whatsapp = jsonSettings["whatsapp"] | false;
    MobileNumber = jsonSettings["MobileNumber"].as<String>();
    APIKey = jsonSettings["APIKey"].as<String>();
    gerencia = jsonSettings["gerencia"].as<String>();
    ciudad = jsonSettings["ciudad"].as<String>();
    central = jsonSettings["central"].as<String>();
    sala = jsonSettings["sala"].as<String>();
    strlcpy(device_id, jsonSettings["device_id"] | "", sizeof(device_id));                   // si no hay nada lo pone vacio
    strlcpy(device_name, jsonSettings["device_name"] | "", sizeof(device_name));             // si no hay nada lo pone vacio
    strlcpy(device_user, jsonSettings["device_user"] | "", sizeof(device_user));             // si no hay nada lo pone vacio
    strlcpy(device_password, jsonSettings["device_password"] | "", sizeof(device_password)); // si no hay nada lo pone vacio
    // wifi modo estación
    // como el siguiente es un boolean es de la siguiente manera o si son numeros enteros
    wifi_mode = jsonSettings["wifi"]["wifi_mode"] | false;
    // TODO:wifi_mode = jsonSettings["wifi"]["wifi_mode"] && switchMode; // si no viene nada se pone en false para cambiar el modo de manera manual
    strlcpy(wifi_ssid, jsonSettings["wifi"]["wifi_ssid"] | "", sizeof(wifi_ssid));
    strlcpy(wifi_ssid2, jsonSettings["wifi"]["wifi_ssid2"] | "", sizeof(wifi_ssid2));
    strlcpy(wifi_password, jsonSettings["wifi"]["wifi_password"] | "", sizeof(wifi_password));
    strlcpy(wifi_password2, jsonSettings["wifi"]["wifi_password2"] | "", sizeof(wifi_password2));
    wifi_ip_static = jsonSettings["wifi"]["wifi_ip_static"] | false; // si no viene nada se pone en false
    strlcpy(wifi_ipv4, jsonSettings["wifi"]["wifi_ipv4"] | "", sizeof(wifi_ipv4));
    strlcpy(wifi_subnet, jsonSettings["wifi"]["wifi_subnet"] | "", sizeof(wifi_subnet));
    strlcpy(wifi_gateway, jsonSettings["wifi"]["wifi_gateway"] | "", sizeof(wifi_gateway));
    strlcpy(wifi_dns_primary, jsonSettings["wifi"]["wifi_dns_primary"] | "", sizeof(wifi_dns_primary));
    strlcpy(wifi_dns_secondary, jsonSettings["wifi"]["wifi_dns_secondary"] | "", sizeof(wifi_dns_secondary));
    // wifi ap
    strlcpy(ap_ssid, jsonSettings["wifi"]["ap_ssid"] | "", sizeof(ap_ssid));
    strlcpy(ap_password, jsonSettings["wifi"]["ap_password"] | "", sizeof(ap_password));
    ap_channel = jsonSettings["wifi"]["ap_channel"] | 1;
    ap_oculto = jsonSettings["wifi"]["ap_oculto"] | false; // 0 es para que se vea
    ap_connect = jsonSettings["wifi"]["ap_connect"] | 3;   // cantidad de conexiones en modo AP
    // MQTT
    mqtt_enabled = jsonSettings["mqtt"]["mqtt_enabled"] | false;
    strlcpy(mqtt_server, jsonSettings["mqtt"]["mqtt_server"] | "", sizeof(mqtt_server));
    mqtt_port = jsonSettings["mqtt"]["mqtt_port"] | 1883;
    mqtt_retain = jsonSettings["mqtt"]["mqtt_retain"] | false;
    mqtt_qos = jsonSettings["mqtt"]["mqtt_qos"] | 0;
    strlcpy(mqtt_id, jsonSettings["mqtt"]["mqtt_id"] | "", sizeof(mqtt_id));
    strlcpy(mqtt_user, jsonSettings["mqtt"]["mqtt_user"] | "", sizeof(mqtt_user));
    strlcpy(mqtt_password, jsonSettings["mqtt"]["mqtt_password"] | "", sizeof(mqtt_password));
    mqtt_clean_session = jsonSettings["mqtt"]["mqtt_clean_session"] | false;
    strlcpy(mqtt_commandTopic, jsonSettings["mqtt"]["mqtt_commandTopic"] | "", sizeof(mqtt_commandTopic));
    strlcpy(mqtt_sendTopic, jsonSettings["mqtt"]["mqtt_sendTopic"] | "", sizeof(mqtt_sendTopic));
    strlcpy(mqtt_willTopic, jsonSettings["mqtt"]["mqtt_willTopic"] | "", sizeof(mqtt_willTopic));
    strlcpy(mqtt_willMessage, jsonSettings["mqtt"]["mqtt_willMessage"] | "", sizeof(mqtt_willMessage));
    mqtt_willQos = jsonSettings["mqtt"]["mqtt_willQos"] | 0;
    mqtt_willRetain = jsonSettings["mqtt"]["mqtt_willRetain"] | false;
    mqtt_time_send = jsonSettings["mqtt"]["mqtt_time_send"] | false;
    mqtt_time_interval = jsonSettings["mqtt"]["mqtt_time_interval"] | 60; // 60 es el standard
    mqtt_status_send = jsonSettings["mqtt"]["mqtt_status_send"] | false;
    // Graficacion
    tgrafica = jsonSettings["grafica"]["tgrafica"] | 1;
    // DIMER
    dim = jsonSettings["dimmer"]["dim_value"] | 0;
    ajTmpDht22 = jsonSettings["ajTemp"]["ajTmpDht22"] | 0;
    ajTmpLm35 = jsonSettings["ajTemp"]["ajTmpLm35"] | 0;
    ajTmpP1 = jsonSettings["ajTemp"]["ajTmpP1"] | 0;
    ajTmpP2 = jsonSettings["ajTemp"]["ajTmpP2"] | 0;
    // ---------------------------------------------------------
    //  ZONA DE ALARMAS
    // ---------------------------------------------------------
    ALARM_LOGICA1 = jsonSettings["alarmas"]["ALARM_LOGICA1"].as<bool>();
    ALARM_RECONOCIDA1 = jsonSettings["alarmas"]["ALARM_RECONOCIDA1"].as<bool>();
    ALARM_NAME1 = jsonSettings["alarmas"]["ALARM_NAME1"].as<String>();
    ALARM_LOGICA2 = jsonSettings["alarmas"]["ALARM_LOGICA2"].as<bool>();
    ALARM_RECONOCIDA2 = jsonSettings["alarmas"]["ALARM_RECONOCIDA2"].as<bool>();
    ALARM_NAME2 = jsonSettings["alarmas"]["ALARM_NAME2"].as<String>();
    ALARM_LOGICA3 = jsonSettings["alarmas"]["ALARM_LOGICA3"].as<bool>();
    ALARM_RECONOCIDA3 = jsonSettings["alarmas"]["ALARM_RECONOCIDA3"].as<bool>();
    ALARM_NAME3 = jsonSettings["alarmas"]["ALARM_NAME3"].as<String>();
    ALARM_LOGICA4 = jsonSettings["alarmas"]["ALARM_LOGICA4"].as<bool>();
    ALARM_RECONOCIDA4 = jsonSettings["alarmas"]["ALARM_RECONOCIDA4"].as<bool>();
    ALARM_NAME4 = jsonSettings["alarmas"]["ALARM_NAME4"].as<String>();
    ALARM_LOGICA5 = jsonSettings["alarmas"]["ALARM_LOGICA5"].as<bool>();
    ALARM_RECONOCIDA5 = jsonSettings["alarmas"]["ALARM_RECONOCIDA5"].as<bool>();
    ALARM_NAME5 = jsonSettings["alarmas"]["ALARM_NAME5"].as<String>();
    ALARM_LOGICA6 = jsonSettings["alarmas"]["ALARM_LOGICA6"].as<bool>();
    ALARM_RECONOCIDA6 = jsonSettings["alarmas"]["ALARM_RECONOCIDA6"].as<bool>();
    ALARM_NAME6 = jsonSettings["alarmas"]["ALARM_NAME6"].as<String>();
    ALARM_LOGICA7 = jsonSettings["alarmas"]["ALARM_LOGICA7"].as<bool>();
    ALARM_RECONOCIDA7 = jsonSettings["alarmas"]["ALARM_RECONOCIDA7"].as<bool>();
    ALARM_NAME7 = jsonSettings["alarmas"]["ALARM_NAME7"].as<String>();
    ALARM_LOGICA8 = jsonSettings["alarmas"]["ALARM_LOGICA8"].as<bool>();
    ALARM_RECONOCIDA8 = jsonSettings["alarmas"]["ALARM_RECONOCIDA8"].as<bool>();
    ALARM_NAME8 = jsonSettings["alarmas"]["ALARM_NAME8"].as<String>();
    // Zona RELAYS
    strlcpy(R_NAME1, jsonSettings["relay"]["R_NAME1"], sizeof(R_NAME1)); // variables de tipo char primero se define la variable (R_NAME1)
    strlcpy(R_NAME2, jsonSettings["relay"]["R_NAME2"], sizeof(R_NAME2)); // variables de tipo char primero se define la variable (R_NAME2)
    RELAY1_STATUS = jsonSettings["relay"]["RELAY1_STATUS"] | false;
    RELAY2_STATUS = jsonSettings["relay"]["RELAY2_STATUS"] | false;
    R_DESCRIPTION1 = jsonSettings["relay"]["R_DESCRIPTION1"].as<String>();
    R_DESCRIPTION2 = jsonSettings["relay"]["R_DESCRIPTION2"].as<String>();
    R_TIMERON1 = jsonSettings["relay"]["R_TIMERON1"].as<bool>();       // indica si se activa el timer del relevador1
    R_TIMERON2 = jsonSettings["relay"]["R_TIMERON2"].as<bool>();       // indica si se activa el timer del relevador2
    TEMPORIZADOR1 = jsonSettings["relay"]["TEMPORIZADOR1"].as<bool>(); // indica si hay un control por horario
    TEMPORIZADOR2 = jsonSettings["relay"]["TEMPORIZADOR2"].as<bool>(); // indica si hay un control por hirario

    TIMEONRELAY1A = jsonSettings["relay"]["TIMEONRELAY1A"].as<String>();   // indica a que horas se prende
    TIMEONRELAY1B = jsonSettings["relay"]["TIMEONRELAY1B"].as<String>();   // indica a que horas se prende
    TIMEONRELAY2A = jsonSettings["relay"]["TIMEONRELAY2A"].as<String>();   // indica a que horas se prende
    TIMEONRELAY2B = jsonSettings["relay"]["TIMEONRELAY2B"].as<String>();   // indica a que horas se prende
    TIMEOFFRELAY1A = jsonSettings["relay"]["TIMEOFFRELAY1A"].as<String>(); // indica a que horas se apaga
    TIMEOFFRELAY1B = jsonSettings["relay"]["TIMEOFFRELAY1B"].as<String>(); // indica a que horas se apaga
    TIMEOFFRELAY2A = jsonSettings["relay"]["TIMEOFFRELAY2A"].as<String>(); // indica a que horas se apaga
    TIMEOFFRELAY2B = jsonSettings["relay"]["TIMEOFFRELAY2B"].as<String>(); // indica a que horas se apaga

    LUNES1A = jsonSettings["relay"]["LUNES1A"].as<bool>();
    LUNES1B = jsonSettings["relay"]["LUNES1B"].as<bool>();
    LUNES2A = jsonSettings["relay"]["LUNES2A"].as<bool>();
    LUNES2B = jsonSettings["relay"]["LUNES2B"].as<bool>();
    MARTES1A = jsonSettings["relay"]["MARTES1A"].as<bool>();
    MARTES1B = jsonSettings["relay"]["MARTES1B"].as<bool>();
    MARTES2A = jsonSettings["relay"]["MARTES2A"].as<bool>();
    MARTES2B = jsonSettings["relay"]["MARTES2B"].as<bool>();
    MIERCOLES1A = jsonSettings["relay"]["MIERCOLES1A"].as<bool>();
    MIERCOLES1B = jsonSettings["relay"]["MIERCOLES1B"].as<bool>();
    MIERCOLES2A = jsonSettings["relay"]["MIERCOLES2A"].as<bool>();
    MIERCOLES2B = jsonSettings["relay"]["MIERCOLES2B"].as<bool>();
    JUEVES1A = jsonSettings["relay"]["JUEVES1A"].as<bool>();
    JUEVES1B = jsonSettings["relay"]["JUEVES1B"].as<bool>();
    JUEVES2A = jsonSettings["relay"]["JUEVES2A"].as<bool>();
    JUEVES2B = jsonSettings["relay"]["JUEVES2B"].as<bool>();
    VIERNES1A = jsonSettings["relay"]["VIERNES1A"].as<bool>();
    VIERNES1B = jsonSettings["relay"]["VIERNES1B"].as<bool>();
    VIERNES2A = jsonSettings["relay"]["VIERNES2A"].as<bool>();
    VIERNES2B = jsonSettings["relay"]["VIERNES2B"].as<bool>();
    SABADO1A = jsonSettings["relay"]["SABADO1A"].as<bool>();
    SABADO1B = jsonSettings["relay"]["SABADO1B"].as<bool>();
    SABADO2A = jsonSettings["relay"]["SABADO2A"].as<bool>();
    SABADO2B = jsonSettings["relay"]["SABADO2B"].as<bool>();
    DOMINGO1A = jsonSettings["relay"]["DOMINGO1A"].as<bool>();
    DOMINGO1B = jsonSettings["relay"]["DOMINGO1B"].as<bool>();
    DOMINGO2A = jsonSettings["relay"]["DOMINGO2A"].as<bool>();
    DOMINGO2B = jsonSettings["relay"]["DOMINGO2B"].as<bool>();
    FECHAON1 = jsonSettings["relay"]["FECHAON1"].as<String>();   // muestra la FECHA de encendido
    FECHAON2 = jsonSettings["relay"]["FECHAON2"].as<String>();   // muestra la FECHA de encendido
    FECHAOFF1 = jsonSettings["relay"]["FECHAOFF1"].as<String>(); // muestra la FECHA de apagado
    FECHAOFF2 = jsonSettings["relay"]["FECHAOFF2"].as<String>(); // muestra la FECHA de apagado
    programado1 = jsonSettings["relay"]["programado1"].as<bool>();
    programado2 = jsonSettings["relay"]["programado2"].as<bool>();

    // TELEGRAM =

    // Time settings.json
    // time_ajuste = jsonSettings["time"]["time_ajuste"];
    strlcpy(time_date, jsonSettings["time"]["time_date"], sizeof(time_date));
    // time_z_horaria = jsonSettings["time"]["time_z_horaria"];
    // strlcpy(time_server, jsonSettings["time"]["time_server"], sizeof(time_server));
    //  log
    myLog("INFO", "settings.hpp", "settingsRead()", "lecturas de las configuraciones correctamente");
    // Serial.print("ajuste DHT22: ");
    // Serial.println(ajTmpDht22);
    return true;
}

// Funcion que resetea a los valores de fabrica
void settingsReset()
{
    myLog("INFO", "settings.hpp", "settingsReset()", "Se procede a asignar valores de fabrica");
    telegram = false;
    botToken = "7678398303"; // 7678398303:AAEZq0Y941WBAfQcDR0qEn5CNITJwyT4FSA// Reemplaza con tu token de bot
    chatID = "7077248297";   // 7077248297
    whatsapp = false;
    MobileNumber = "5219211355959"; // 5219211355959
    APIKey = "8124673";             // 8124673
    gerencia = "gerencia";          // coatza
    ciudad = "coatza";
    central = "coaigl";
    sala = "sala";
    strlcpy(device_id, DeviceID().c_str(), sizeof(device_id));
    strlcpy(device_name, DeviceName.c_str(), sizeof(device_name));
    strlcpy(device_user, Administrador.c_str(), sizeof(device_user));
    strlcpy(device_password, Contra.c_str(), sizeof(device_password));
    // wifi modo estación
    // como el siguiente es un boolean es de la siguiente manera o si son numeros enteros
    wifi_mode = WifiMode; // true es modo estación
    strlcpy(wifi_ssid, SSID.c_str(), sizeof(wifi_ssid));
    strlcpy(wifi_ssid2, SSID2.c_str(), sizeof(wifi_ssid2));
    strlcpy(wifi_password, WifiPassword.c_str(), sizeof(wifi_password));
    strlcpy(wifi_password2, WifiPassword2.c_str(), sizeof(wifi_password2));
    wifi_ip_static = wifi_ip_static; // para configurar con ips fijas
    strlcpy(wifi_ipv4, Ipv4.c_str(), sizeof(wifi_ipv4));
    strlcpy(wifi_subnet, SubRed.c_str(), sizeof(wifi_subnet));
    strlcpy(wifi_gateway, Gateway.c_str(), sizeof(wifi_gateway));
    strlcpy(wifi_dns_primary, DnsPrimary.c_str(), sizeof(wifi_dns_primary));
    strlcpy(wifi_dns_secondary, DnsSecondary.c_str(), sizeof(wifi_dns_secondary));
    // wifi ap
    strlcpy(ap_ssid, DeviceID().c_str(), sizeof(ap_ssid));
    strlcpy(ap_password, Contra.c_str(), sizeof(ap_password));
    ap_channel = 9;    // 1-13
    ap_oculto = false; // para que se vea
    ap_connect = 3;    // cantidad de conexiones en modo AP 0-8
    // MQTT
    mqtt_enabled = MqttAcceso;
    strlcpy(mqtt_server, MqttServer.c_str(), sizeof(mqtt_server));
    mqtt_port = MqttPort;
    mqtt_retain = false;
    mqtt_qos = 0;
    strlcpy(mqtt_id, DeviceID().c_str(), sizeof(mqtt_id));
    strlcpy(mqtt_user, MqttUser.c_str(), sizeof(mqtt_user));
    strlcpy(mqtt_password, MqttPassword.c_str(), sizeof(mqtt_password));
    mqtt_clean_session = true;
    strlcpy(mqtt_commandTopic, PathMqttTopic("command").c_str(), sizeof(mqtt_commandTopic));
    strlcpy(mqtt_sendTopic, PathMqttTopic("device").c_str(), sizeof(mqtt_sendTopic));
    strlcpy(mqtt_willTopic, PathMqttTopic("status").c_str(), sizeof(mqtt_willTopic));
    strlcpy(mqtt_willMessage, Willmsg(DeviceName).c_str(), sizeof(mqtt_willMessage));
    mqtt_willQos = 0;
    mqtt_willRetain = false;
    mqtt_time_send = true;
    mqtt_time_interval = 10000; // 60000 es el standard
    mqtt_status_send = true;
    // tiempo de muestra de la grafica en minutos
    tgrafica = 1;
    // DIMER
    dim = 0;
    ajTmpDht22 = 0;
    ajTmpLm35 = 0;
    ajTmpP1 = -2; // ajuste de temperatura del pozo 1 personalizado
    ajTmpP2 = -4; // ajuste de temperatura del pozo 2 personalizado
    // ---------------------------------------------------------
    //  ZONA DE ALARMAS true = se activa el estatus con tierra y se pone en false
    // ---------------------------------------------------------
    ALARM_LOGICA1 = true;
    ALARM_RECONOCIDA1 = true;
    ALARM_NAME1 = "Falla rectificador P.B.";
    ALARM_LOGICA2 = true;
    ALARM_RECONOCIDA2 = true;
    ALARM_NAME2 = "Descarga Baterias P.B.";
    ALARM_LOGICA3 = true;
    ALARM_RECONOCIDA3 = true;
    ALARM_NAME3 = "Bajo Voltaje de Baterias P.B.";
    ALARM_LOGICA4 = true;
    ALARM_RECONOCIDA4 = true;
    ALARM_NAME4 = "Grupo Electrogeno Bloqueado";
    ALARM_LOGICA5 = true;
    ALARM_RECONOCIDA5 = true;
    ALARM_NAME5 = "Alta Temperatura PTTI";
    ALARM_LOGICA6 = true;
    ALARM_RECONOCIDA6 = true;
    ALARM_NAME6 = "Falla Compresor Clima PTTI";
    ALARM_LOGICA7 = true;
    ALARM_RECONOCIDA7 = true;
    ALARM_NAME7 = "Grupo Electrogeno Operando";
    ALARM_LOGICA8 = true;
    ALARM_RECONOCIDA8 = true;
    ALARM_NAME8 = "Falla de red CFE";
    /********************************************************************************
     * variables para los actuadores RELAYS                                          *
     *******************************************************************************/
    strlcpy(R_NAME1, "RELAY1 GPIO17", sizeof(R_NAME1)); // variables de tipo char primero se define la variable (R_NAME1)
    strlcpy(R_NAME2, "RELAY2 GPIO18", sizeof(R_NAME2)); // variables de tipo char primero se define la variable (R_NAME2)
    RELAY1_STATUS = false;                              // el estado almacenado de los relevadores
    RELAY2_STATUS = false;
    R_DESCRIPTION1 = "Control del GPIO17";
    R_DESCRIPTION2 = "Control del GPIO18";
    R_TIMERON1 = false;    // indica si se activa el timer del relevador1 //no se guarda
    R_TIMERON2 = false;    // indica si se activa el timer del relevador2 //no se guarda
    TEMPORIZADOR1 = false; // indica si hay un control por horario
    TEMPORIZADOR2 = false; // indica si hay un control por hirario

    TIMEONRELAY1A = "00:00";  // indica a que horas se prende
    TIMEONRELAY1B = "00:00";  // indica a que horas se prende
    TIMEONRELAY2A = "00:00";  // indica a que horas se prende
    TIMEONRELAY2B = "00:00";  // indica a que horas se prende
    TIMEOFFRELAY1A = "00:00"; // indica a que horas se apaga
    TIMEOFFRELAY1B = "00:00"; // indica a que horas se apaga
    TIMEOFFRELAY2A = "00:00"; // indica a que horas se apaga
    TIMEOFFRELAY2B = "00:00"; // indica a que horas se apaga

    LUNES1A = false;
    LUNES1B = false;
    LUNES2A = false;
    LUNES2B = false;
    MARTES1A = false;
    MARTES1B = false;
    MARTES2A = false;
    MARTES2B = false;
    MIERCOLES1A = false;
    MIERCOLES1B = false;
    MIERCOLES2A = false;
    MIERCOLES2B = false;
    JUEVES1A = false;
    JUEVES1B = false;
    JUEVES2A = false;
    JUEVES2B = false;
    VIERNES1A = false;
    VIERNES1B = false;
    VIERNES2A = false;
    VIERNES2B = false;
    SABADO1A = false;
    SABADO1B = false;
    SABADO2A = false;
    SABADO2B = false;
    DOMINGO1A = false;
    DOMINGO1B = false;
    DOMINGO2A = false;
    DOMINGO2B = false;
    FECHAON1 = "";      // muestra la FECHA de encendido
    FECHAON2 = "";      // muestra la FECHA de encendido
    FECHAOFF1 = "";     // muestra la FECHA de apagado
    FECHAOFF2 = "";     // muestra la FECHA de apagado
    programado1 = true; // programado para enviar el cambio por telegram
    programado2 = true; // programado para enviar el cambio por telegram
    //  -------------------------------------------------------------------
    //  Time settings.json
    //  -------------------------------------------------------------------
    // time_ajuste = true;
    strlcpy(time_date, "2024-11-21T21:21", sizeof(time_date));
    // time_z_horaria = -14400;
    // strlcpy(time_server, "time.nist.gov", sizeof(time_server));
    //  log
    myLog("INFO", "settings.hpp", "settingsReset()", "Se reiniciaron todos los valores por defecto de fabrica");
}
// hasta el momento todo esto solo pone en la memoria la información pero aun no se salva y toda la configuración se pierde al reiniciar el dispositivo
// Guardar configuraciones en el json
boolean settingsSave()
{
    // definicion del json
    JsonDocument jsonSettings; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // telegram
    jsonSettings["telegram"] = telegram;
    jsonSettings["botToken"] = botToken;
    jsonSettings["chatID"] = chatID;
    jsonSettings["whatsapp"] = whatsapp;
    jsonSettings["MobileNumber"] = MobileNumber;
    jsonSettings["APIKey"] = APIKey;
    // generales
    jsonSettings["gerencia"] = gerencia;
    jsonSettings["ciudad"] = ciudad;
    jsonSettings["central"] = central;
    jsonSettings["sala"] = sala;
    jsonSettings["device_id"] = device_id;
    jsonSettings["device_name"] = device_name;
    jsonSettings["device_user"] = device_user;
    jsonSettings["device_password"] = device_password;
    // wifi
    JsonObject wifiObj = jsonSettings["wifi"].to<JsonObject>(); // se le indica que es un objeto de JSON
    wifiObj["wifi_mode"] = wifi_mode;
    wifiObj["wifi_ssid"] = wifi_ssid;
    wifiObj["wifi_ssid2"] = wifi_ssid2;
    wifiObj["wifi_password"] = wifi_password;
    wifiObj["wifi_password2"] = wifi_password2;
    wifiObj["wifi_ip_static"] = wifi_ip_static;
    wifiObj["wifi_ipv4"] = wifi_ipv4;
    wifiObj["wifi_subnet"] = wifi_subnet;
    wifiObj["wifi_gateway"] = wifi_gateway;
    wifiObj["wifi_dns_primary"] = wifi_dns_primary;
    wifiObj["wifi_dns_secondary"] = wifi_dns_secondary;
    wifiObj["ap_ssid"] = ap_ssid;
    wifiObj["ap_password"] = ap_password;
    wifiObj["ap_channel"] = ap_channel;
    wifiObj["ap_oculto"] = ap_oculto;
    wifiObj["ap_connect"] = ap_connect;

    // MQTT
    JsonObject mqttObj = jsonSettings["mqtt"].to<JsonObject>(); // se le indica que es un objeto de JSON
    mqttObj["mqtt_enabled"] = mqtt_enabled;
    mqttObj["mqtt_server"] = mqtt_server;
    mqttObj["mqtt_port"] = mqtt_port;
    mqttObj["mqtt_retain"] = mqtt_retain;
    mqttObj["mqtt_qos"] = mqtt_qos;
    mqttObj["mqtt_id"] = mqtt_id;
    mqttObj["mqtt_user"] = mqtt_user;
    mqttObj["mqtt_password"] = mqtt_password;
    mqttObj["mqtt_clean_session"] = mqtt_clean_session; // verificar si no es mqtt_clean_sessions
    mqttObj["mqtt_commandTopic"] = mqtt_commandTopic;
    mqttObj["mqtt_sendTopic"] = mqtt_sendTopic;
    mqttObj["mqtt_willTopic"] = mqtt_willTopic;
    mqttObj["mqtt_willMessage"] = mqtt_willMessage;
    mqttObj["mqtt_willQos"] = mqtt_willQos; // verificar si es mqtt_willQoS
    mqttObj["mqtt_willRetain"] = mqtt_willRetain;
    mqttObj["mqtt_time_send"] = mqtt_time_send;
    mqttObj["mqtt_time_interval"] = mqtt_time_interval;
    mqttObj["mqtt_status_send"] = mqtt_status_send;
    // relays
    JsonObject relayObj = jsonSettings["relay"].to<JsonObject>();
    relayObj["R_NAME1"] = R_NAME1;             // variables de tipo char primero se define la variable (R_NAME1)
    relayObj["R_NAME2"] = R_NAME2;             // variables de tipo char primero se define la variable (R_NAME2)
    relayObj["RELAY1_STATUS"] = RELAY1_STATUS; // lee el valor de la variable que indica si esta encendido o apagado
    relayObj["RELAY2_STATUS"] = RELAY2_STATUS;
    relayObj["R_DESCRIPTION1"] = R_DESCRIPTION1;
    relayObj["R_DESCRIPTION2"] = R_DESCRIPTION2;
    relayObj["R_TIMERON1"] = R_TIMERON1; // si debe se encenderse por algun tiempo
    relayObj["R_TIMERON2"] = R_TIMERON2; // si debe se encenderse por algun tiempo
    relayObj["TEMPORIZADOR1"] = TEMPORIZADOR1;
    relayObj["TEMPORIZADOR2"] = TEMPORIZADOR2;

    relayObj["TIMEONRELAY1A"] = TIMEONRELAY1A;
    relayObj["TIMEONRELAY1B"] = TIMEONRELAY1B;
    relayObj["TIMEONRELAY2A"] = TIMEONRELAY2A;
    relayObj["TIMEONRELAY2B"] = TIMEONRELAY2B;
    relayObj["TIMEOFFRELAY1A"] = TIMEOFFRELAY1A;
    relayObj["TIMEOFFRELAY1B"] = TIMEOFFRELAY1B;
    relayObj["TIMEOFFRELAY2A"] = TIMEOFFRELAY2A;
    relayObj["TIMEOFFRELAY2B"] = TIMEOFFRELAY2B;

    relayObj["LUNES1A"] = LUNES1A;
    relayObj["LUNES1B"] = LUNES1B;
    relayObj["LUNES2A"] = LUNES2A;
    relayObj["LUNES2B"] = LUNES2B;
    relayObj["MARTES1A"] = MARTES1A;
    relayObj["MARTES1B"] = MARTES1B;
    relayObj["MARTES2A"] = MARTES2A;
    relayObj["MARTES2B"] = MARTES2B;
    relayObj["MIERCOLES1A"] = MIERCOLES1A;
    relayObj["MIERCOLES1B"] = MIERCOLES1B;
    relayObj["MIERCOLES2A"] = MIERCOLES2A;
    relayObj["MIERCOLES2B"] = MIERCOLES2B;
    relayObj["JUEVES1A"] = JUEVES1A;
    relayObj["JUEVES1B"] = JUEVES1B;
    relayObj["JUEVES2A"] = JUEVES2A;
    relayObj["JUEVES2B"] = JUEVES2B;
    relayObj["VIERNES1A"] = VIERNES1A;
    relayObj["VIERNES1B"] = VIERNES1B;
    relayObj["VIERNES2A"] = VIERNES2A;
    relayObj["VIERNES2B"] = VIERNES2B;
    relayObj["SABADO1A"] = SABADO1A;
    relayObj["SABADO1B"] = SABADO1B;
    relayObj["SABADO2A"] = SABADO2A;
    relayObj["SABADO2B"] = SABADO2B;
    relayObj["DOMINGO1A"] = DOMINGO1A;
    relayObj["DOMINGO1B"] = DOMINGO1B;
    relayObj["DOMINGO2A"] = DOMINGO2A;
    relayObj["DOMINGO2B"] = DOMINGO2B;
    relayObj["FECHAON1"] = FECHAON1;
    relayObj["FECHAON2"] = FECHAON2;
    relayObj["FECHAOFF1"] = FECHAOFF1;
    relayObj["FECHAOFF2"] = FECHAOFF2;
    relayObj["programado1"] = programado1;
    relayObj["programado2"] = programado2;
    //  tiempo de la grafica en minutos
    JsonObject graficaObj = jsonSettings["grafica"].to<JsonObject>();
    graficaObj["tgrafica"] = tgrafica;
    // DIMER
    JsonObject dimmerObj = jsonSettings["dimmer"].to<JsonObject>();
    dimmerObj["dim_value"] = dim;
    JsonObject ajusTempObj = jsonSettings["ajTemp"].to<JsonObject>();
    ajusTempObj["ajTmpDht22"] = ajTmpDht22;
    ajusTempObj["ajTmpLm35"] = ajTmpLm35;
    ajusTempObj["ajTmpP1"] = ajTmpP1;
    ajusTempObj["ajTmpP2"] = ajTmpP2;

    // ---------------------------------------------------------
    //  ZONA DE ALARMAS
    // ---------------------------------------------------------
    JsonObject alarmasObj = jsonSettings["alarmas"].to<JsonObject>();
    alarmasObj["ALARM_LOGICA1"] = ALARM_LOGICA1;
    alarmasObj["ALARM_RECONOCIDA1"] = ALARM_RECONOCIDA1;
    alarmasObj["ALARM_NAME1"] = ALARM_NAME1;
    alarmasObj["ALARM_LOGICA2"] = ALARM_LOGICA2;
    alarmasObj["ALARM_RECONOCIDA2"] = ALARM_RECONOCIDA2;
    alarmasObj["ALARM_NAME2"] = ALARM_NAME2;
    alarmasObj["ALARM_LOGICA3"] = ALARM_LOGICA3;
    alarmasObj["ALARM_RECONOCIDA3"] = ALARM_RECONOCIDA3;
    alarmasObj["ALARM_NAME3"] = ALARM_NAME3;
    alarmasObj["ALARM_LOGICA4"] = ALARM_LOGICA4;
    alarmasObj["ALARM_RECONOCIDA4"] = ALARM_RECONOCIDA4;
    alarmasObj["ALARM_NAME4"] = ALARM_NAME4;
    alarmasObj["ALARM_LOGICA5"] = ALARM_LOGICA5;
    alarmasObj["ALARM_RECONOCIDA5"] = ALARM_RECONOCIDA5;
    alarmasObj["ALARM_NAME5"] = ALARM_NAME5;
    alarmasObj["ALARM_LOGICA6"] = ALARM_LOGICA6;
    alarmasObj["ALARM_RECONOCIDA6"] = ALARM_RECONOCIDA6;
    alarmasObj["ALARM_NAME6"] = ALARM_NAME6;
    alarmasObj["ALARM_LOGICA7"] = ALARM_LOGICA7;
    alarmasObj["ALARM_RECONOCIDA7"] = ALARM_RECONOCIDA7;
    alarmasObj["ALARM_NAME7"] = ALARM_NAME7;
    alarmasObj["ALARM_LOGICA8"] = ALARM_LOGICA8;
    alarmasObj["ALARM_RECONOCIDA8"] = ALARM_RECONOCIDA8;
    alarmasObj["ALARM_NAME8"] = ALARM_NAME8;
    // ---------------------------------------------------------------------------------
    // TIME settings.json
    // -------------------------------------------------------------------
    JsonObject timeObj = jsonSettings["time"].to<JsonObject>();
    // timeObj["time_ajuste"] = time_ajuste;
    timeObj["time_date"] = nuevaFecha; // FIXME:
    // timeObj["time_z_horaria"] = time_z_horaria;
    // timeObj["time_server"] = time_server;
    //  guardar el json en spiffs
    File file = SPIFFS.open("/settings.json", "w"); // modo escritura
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "settings.hpp", "settingsSave()", "Error en abrir el archivo de configuraciones settings.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsonSettings, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "settings.hpp", "serializeJson(jsonSettings,file)", "Error al serializar el archivo settings.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "settings.hpp", "settingsSave()", "Se guardaron las configuraciones en el archivo settings.json");
    // mostrar el settings.json en el monitor
    // serializeJsonPretty(jsonSettings, Serial);
    return true;
}
