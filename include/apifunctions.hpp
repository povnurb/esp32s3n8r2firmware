// funciones que complementan a la api
// declaraciones
boolean borrarBitacora();
boolean salvarBitacora(String nombretecnico, String fechaB, String tipoD, String infoB);
boolean deleteArchivoAlarmas();
void enviarMensaje(String mensaje);
void enviarMensajeWhatsapp(String mensaje);
void timeReset();
void dataGraficasReset();
boolean timeSave();
bool dataGraficasSave();

String apiHistorialAlarmas()
{
    // https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    String response = "";

    JsonDocument jsonAlarmHistorial; // objeto jsonAlarmHistorial donde se leeran las configuraciones iniciales

    File file = SPIFFS.open("/alarmas.json", "r"); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    { // instalar la extencion TODO Highlight
        // FIXME: Serial.println("[WARNING][settings.hpp][settingsRead()]Falla en abrir el archivo settings.json");
        // dejare ese FIXME como ejemplo pero la siguiente linea es la solucion
        myLog("ERROR", "apifunctions.hpp", "apiHistorialAlarmas()", "Error al abrir el archivo alarmas.json");
        // TODO: llamar a la función de iniciar el json de fabrica (HECHO)
        return response;
    }
    myLog("INFO", "apifunctions.hpp", "apiHistorialAlarmas()", "Entregando archivo");
    String contenido = file.readString();
    response = "[" + contenido + "]"; // para que se vea JSON

    file.close();

    return response;
}

// apiHistorialBitacora
String apiHistorialBitacora()
{
    // https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    String response = "";

    JsonDocument jsonBitacoraHistorial; // objeto jsonBitacoraHistorial donde se leeran las configuraciones iniciales

    File file = SPIFFS.open("/bitacora.json", "r"); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    { // instalar la extencion TODO Highlight
        // FIXME: Serial.println("[WARNING][settings.hpp][settingsRead()]Falla en abrir el archivo settings.json");
        // dejare ese FIXME como ejemplo pero la siguiente linea es la solucion
        myLog("ERROR", "apifunctions.hpp", "apiHistorialBitacora()", "Error al abrir el archivo alarmas.json");
        // TODO: llamar a la función de iniciar el json de fabrica (HECHO)
        return response;
    }
    myLog("INFO", "apifunctions.hpp", "apiHistorialBitacora()", "Entregando archivo");
    String contenido = file.readString();
    response = "[" + contenido + "]"; // para que se vea JSON

    file.close();

    return response;
}

// url /api/index
// method: GET
String apiGetIndex()
{
    String response = "";
    JsonDocument jsonDoc;
    // wifi
    JsonObject wifiObj = jsonDoc["wifi"].to<JsonObject>();
    wifiObj["wifi_status"] = WiFi.status() == WL_CONNECTED ? true : false;
    wifiObj["wifi_ssid"] = wifi_mode == WIFI_AP_STA ? WiFi.SSID() : ap_ssid;
    wifiObj["wifi_ipv4"] = wifi_mode == WIFI_AP_STA ? ipToStr(WiFi.localIP()) : ipToStr(WiFi.softAPIP());
    wifiObj["wifi_mac"] = WiFi.macAddress();
    // mqtt
    JsonObject mqttObj = jsonDoc["mqtt"].to<JsonObject>();
    mqttObj["mqtt_status"] = mqttClient.connected() ? true : false;
    mqttObj["mqtt_server"] = mqttClient.connected() ? mqtt_server : "server not connected";
    mqttObj["mqtt_user"] = mqtt_user;
    mqttObj["mqtt_id"] = mqtt_id;
    // info
    JsonObject infoObj = jsonDoc["info"].to<JsonObject>();
    infoObj["device_fecha"] = device_fecha;
    infoObj["device_serial"] = DeviceSerial();
    infoObj["sala"] = sala;
    infoObj["device_name"] = device_name;
    infoObj["device_id"] = device_id;
    infoObj["device_mdns"] = "http://" + String(device_name) + ".local/";
    infoObj["device_firmware"] = device_fw_version;
    infoObj["device_hardware"] = device_hw_version;
    infoObj["device_manufacture"] = device_manufacture;
    infoObj["device_frequency"] = getCpuFrequencyMhz();
    infoObj["device_ram"] = ESP.getHeapSize() / 1024;                   // kb
    infoObj["device_spiffs"] = SPIFFS.totalBytes() / 1024;              // kb
    infoObj["device_flash"] = ESP.getFlashChipSize() / (1024.0 * 1024); // Mb
    infoObj["device_time"] = longTimeStr(millis() / 1000);              // segundos
    infoObj["device_restart"] = device_restart;
    infoObj["spiffs_used"] = SPIFFS.usedBytes() / 1024;                                          // kb
    infoObj["spiffs_total"] = SPIFFS.totalBytes() / 1024;                                        // kb
    infoObj["ram_available"] = ESP.getFreeHeap() / 1024;                                         // kb
    infoObj["ram_total"] = ESP.getHeapSize() / 1024;                                             // kb
    infoObj["wifi_rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;                      // kb
    infoObj["wifi_quality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0; // kb
    // mas information
    infoObj["tempEvaporador"] = device_tempEvaporador;
    infoObj["tempCondensador"] = device_tempCondensador;
    infoObj["tempLm35"] = device_lm35;
    infoObj["tcdht22"] = device_tempDHT;
    infoObj["humdht22"] = device_humedadDHT;
    infoObj["tmin"] = device_tempMinima;
    infoObj["tmax"] = device_tempMaxima;
    infoObj["buzzer_status"] = BUZZER_STATUS;
    // salidas creandolas en bufers
    JsonDocument relay01Obj;
    JsonDocument relay02Obj;
    JsonDocument dimmerObj;
    JsonDocument tgrafObj;

    // relay1
    JsonObject relay1 = relay01Obj.to<JsonObject>();
    relay1["name"] = R_NAME1; //"RELAY1";
    relay1["status"] = RELAY1_STATUS ? true : false;
    // relay2
    JsonObject relay2 = relay02Obj.to<JsonObject>();
    relay2["name"] = R_NAME2; //"RELAY2";
    relay2["status"] = RELAY2_STATUS ? true : false;
    // dimmer
    JsonObject dimmer = dimmerObj.to<JsonObject>();
    dimmer["name"] = "DIMMER";
    dimmer["status"] = dim;
    // tgrafica
    JsonObject tgraf = tgrafObj.to<JsonObject>();
    tgraf["name"] = "tgrafica";
    tgraf["status"] = tgrafica;
    // crearemos un array de objetos
    JsonArray outputs = jsonDoc["outputs"].to<JsonArray>();
    outputs.add(relay1);
    outputs.add(relay2);
    outputs.add(dimmer);
    outputs.add(tgraf);
    // Serializar
    serializeJsonPretty(jsonDoc, response);
    return response;
}

String apiGetInfo()
{
    String response = "";
    JsonDocument jsonDoc;
    // wifi
    JsonObject wifiObj = jsonDoc["wifi"].to<JsonObject>();
    wifiObj["wifi_status"] = WiFi.status() == WL_CONNECTED ? true : false;
    wifiObj["wifi_ssid"] = wifi_mode == WIFI_AP_STA ? WiFi.SSID() : ap_ssid;
    wifiObj["wifi_ipv4"] = wifi_mode == WIFI_AP_STA ? ipToStr(WiFi.localIP()) : ipToStr(WiFi.softAPIP());
    wifiObj["wifi_mac"] = WiFi.macAddress();
    // mqtt
    JsonObject mqttObj = jsonDoc["mqtt"].to<JsonObject>();
    mqttObj["mqtt_status"] = mqttClient.connected() ? true : false;
    mqttObj["mqtt_server"] = mqttClient.connected() ? mqtt_server : "server not connected";
    mqttObj["mqtt_user"] = mqtt_user;
    mqttObj["mqtt_id"] = mqtt_id;
    // info
    JsonObject infoObj = jsonDoc["info"].to<JsonObject>();
    infoObj["device_fecha"] = device_fecha;
    infoObj["device_serial"] = DeviceID();
    infoObj["device_name"] = sala;
    infoObj["device_id"] = device_name;
    infoObj["device_mdns"] = "http://" + String(device_name) + ".local/";
    infoObj["device_firmware"] = device_fw_version;
    infoObj["device_hardware"] = device_hw_version;
    infoObj["device_manufacture"] = device_manufacture;
    infoObj["device_frequency"] = getCpuFrequencyMhz();
    infoObj["device_ram"] = ESP.getHeapSize() / 1024;                   // kb
    infoObj["device_spiffs"] = SPIFFS.totalBytes() / 1024;              // kb
    infoObj["device_flash"] = ESP.getFlashChipSize() / (1024.0 * 1024); // Mb
    infoObj["device_time"] = longTimeStr(millis() / 1000);              // segundos
    infoObj["device_restart"] = device_restart;
    infoObj["spiffs_used"] = SPIFFS.usedBytes() / 1024;                                          // kb
    infoObj["spiffs_total"] = SPIFFS.totalBytes() / 1024;                                        // kb
    infoObj["ram_available"] = ESP.getFreeHeap() / 1024;                                         // kb
    infoObj["ram_total"] = ESP.getHeapSize() / 1024;                                             // kb
    infoObj["wifi_rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;                      // kb
    infoObj["wifi_quality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0; // kb

    // Serializar
    serializeJsonPretty(jsonDoc, response);
    return response;
}
// url /api/connections/wifi
// method: GET
String apiGetWifi()
{
    String response = "";
    JsonDocument jsonDoc;
    // General
    jsonDoc["serial"] = DeviceID();
    jsonDoc["device"] = platform();
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;
    // wifi
    JsonObject wifiObj = jsonDoc["wifi"].to<JsonObject>();
    wifiObj["wifi_mode"] = wifi_mode ? true : false;
    wifiObj["device_name"] = device_name; // para el mdns
    wifiObj["wifi_ssid"] = wifi_ssid;
    wifiObj["wifi_password"] = "";
    wifiObj["wifi_ssid2"] = wifi_ssid2;
    wifiObj["wifi_password2"] = "";
    wifiObj["wifi_ip_static"] = wifi_ip_static;
    wifiObj["wifi_ipv4"] = wifi_ipv4;
    wifiObj["wifi_gateway"] = wifi_gateway;
    wifiObj["wifi_subnet"] = wifi_subnet;
    wifiObj["wifi_dns_primary"] = wifi_dns_primary;
    wifiObj["wifi_dns_secondary"] = wifi_dns_secondary;
    wifiObj["ap_ssid"] = ap_ssid;
    wifiObj["ap_password"] = "";
    wifiObj["ap_channel"] = ap_channel;
    wifiObj["ap_oculto"] = ap_oculto;
    wifiObj["ap_connect"] = ap_connect;
    // code
    jsonDoc["code"] = 1;

    // serializacion

    serializeJsonPretty(jsonDoc, response);
    return response;
}

// url /api/connections/wifi
// method: POST que sirve para guardar informacion
boolean apiPostWifi(String data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostWifi()", "ERROR EN LAdeserializacion");
        return false;
    }
    // validar el json
    if (!validationWifiParam(doc))
    {
        myLog("ERROR", "apifunctions.hpp", "validationWifiParam()", "ERROR EN LA VALIDACION");
        return false;
    }

    // asignar valores a las variables
    wifi_mode = doc["wifi_mode"].as<bool>();
    // wifiObj["device_name"] = String(device_name);
    if (doc["device_name"] && doc["device_name"].as<String>().length() > 0)
    {
        strlcpy(device_name, doc["device_name"].as<String>().c_str(), sizeof(device_name));
    }
    // ssid
    if (doc["wifi_ssid"] && doc["wifi_ssid"].as<String>().length() > 0)
    {
        strlcpy(wifi_ssid, doc["wifi_ssid"].as<String>().c_str(), sizeof(wifi_ssid));
    }
    if (doc["wifi_ssid2"] && doc["wifi_ssid2"].as<String>().length() > 0)
    {
        strlcpy(wifi_ssid2, doc["wifi_ssid2"].as<String>().c_str(), sizeof(wifi_ssid2));
    }
    if (doc["wifi_password"] && doc["wifi_password"].as<String>().length() > 0)
    {
        strlcpy(wifi_password, doc["wifi_password"].as<String>().c_str(), sizeof(wifi_password));
    }
    if (doc["wifi_password2"] && doc["wifi_password2"].as<String>().length() > 0)
    {
        strlcpy(wifi_password2, doc["wifi_password2"].as<String>().c_str(), sizeof(wifi_password2));
    }
    wifi_ip_static = doc["wifi_ip_static"].as<bool>();
    if (doc["wifi_ipv4"] && doc["wifi_ipv4"].as<String>().length() > 0)
    {
        strlcpy(wifi_ipv4, doc["wifi_ipv4"].as<String>().c_str(), sizeof(wifi_ipv4));
    }
    if (doc["wifi_gateway"] && doc["wifi_gateway"].as<String>().length() > 0)
    {
        strlcpy(wifi_gateway, doc["wifi_gateway"].as<String>().c_str(), sizeof(wifi_gateway));
    }
    if (doc["wifi_subnet"] && doc["wifi_subnet"].as<String>().length() > 0)
    {
        strlcpy(wifi_subnet, doc["wifi_subnet"].as<String>().c_str(), sizeof(wifi_subnet));
    }
    if (doc["wifi_dns_primary"] && doc["wifi_dns_primary"].as<String>().length() > 0)
    {
        strlcpy(wifi_dns_primary, doc["wifi_dns_primary"].as<String>().c_str(), sizeof(wifi_dns_primary));
    }
    if (doc["wifi_dns_secondary"] && doc["wifi_dns_secondary"].as<String>().length() > 0)
    {
        strlcpy(wifi_dns_secondary, doc["wifi_dns_secondary"].as<String>().c_str(), sizeof(wifi_dns_secondary));
    }
    if (doc["ap_ssid"] && doc["ap_ssid"].as<String>().length() > 0)
    {
        strlcpy(ap_ssid, doc["ap_ssid"].as<String>().c_str(), sizeof(ap_ssid));
    }
    if (doc["ap_password"] && doc["ap_password"].as<String>().length() > 0)
    {
        strlcpy(ap_password, doc["ap_password"].as<String>().c_str(), sizeof(ap_password));
    }
    // visibilidad 0 visible 1 oculto //FIXME:
    ap_oculto = doc["ap_oculto"].as<bool>(); // ok
    // canal
    if (doc["ap_channel"])
    {
        ap_channel = doc["ap_channel"].as<int>();
    }
    if (doc["ap_connect"])
    {
        ap_connect = doc["ap_connect"].as<int>();
    }
    // guardar la informacion
    return settingsSave();
}

// escanear redes
String apiGetWifiScan()
{
    String response = "";
    JsonDocument jsonDoc;
    int n = WiFi.scanComplete();
    if (n == -2) // cuando no encuentra redes Wifi
    {
        JsonObject metaObj = jsonDoc["meta"].to<JsonObject>();
        metaObj["serial"] = DeviceID();
        metaObj["count"] = 0;
        JsonArray dataObj = jsonDoc["data"].to<JsonArray>();
        jsonDoc["code"] = 0;
    }
    else if (n)
    {
        JsonObject metaObj = jsonDoc["meta"].to<JsonObject>();
        metaObj["serial"] = DeviceID();
        metaObj["count"] = n;
        JsonArray dataObj = jsonDoc["data"].to<JsonArray>(); // array de objetos
        for (int i = 0; i < n; ++i)
        {
            JsonDocument obj;
            JsonObject network = obj.to<JsonObject>();
            network["n"] = i + 1;
            network["ssid"] = WiFi.SSID(i);
            network["rssi"] = WiFi.RSSI(i);
            network["bssid"] = WiFi.BSSIDstr(i); // MAC
            network["channel"] = WiFi.channel(i);
            network["secure"] = EncryptionType(WiFi.encryptionType(i));
            dataObj.add(network);
        }
        jsonDoc["code"] = 1;
    }
    WiFi.scanDelete();
    if (WiFi.scanComplete() == -2)
    {
        WiFi.scanNetworks(true, true);
    }
    serializeJson(jsonDoc, response);
    return response;
}
// parametros de configuracion mqtt
String apiGetMqtt()
{
    String response = "";
    JsonDocument jsonDoc;
    // General
    jsonDoc["serial"] = DeviceID();
    jsonDoc["device"] = platform();
    // jsonDoc["name"] = DeviceName; //TODO: agregar el nombre para modificarlo
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;
    // wifi
    JsonObject mqttObj = jsonDoc["mqtt"].to<JsonObject>();
    mqttObj["mqtt_enabled"] = mqtt_enabled ? true : false;
    mqttObj["mqtt_server"] = mqtt_server;
    mqttObj["mqtt_port"] = mqtt_port;
    mqttObj["mqtt_retain"] = mqtt_retain ? true : false;
    mqttObj["mqtt_qos"] = mqtt_qos;
    mqttObj["mqtt_id"] = mqtt_id;
    mqttObj["mqtt_user"] = mqtt_user;
    mqttObj["mqtt_password"] = mqtt_password;
    mqttObj["mqtt_clean_session"] = mqtt_clean_session ? true : false;
    mqttObj["mqtt_time_send"] = mqtt_time_send ? true : false;
    mqttObj["mqtt_time_interval"] = mqtt_time_interval / 1000; // para convertir a segundos
    mqttObj["mqtt_status_send"] = mqtt_status_send ? true : false;
    mqttObj["mqtt_commandTopic_gerencia"] = gerencia;
    mqttObj["mqtt_commandTopic_ciudad"] = ciudad;
    mqttObj["mqtt_commandTopic_central"] = central;
    mqttObj["mqtt_commandTopic_sala"] = sala;
    mqttObj["mqtt_IDlast4"] = UniqueID();
    mqttObj["mqtt_commandTopic"] = mqtt_commandTopic;
    mqttObj["mqtt_sendTopic"] = mqtt_sendTopic;
    mqttObj["mqtt_willTopic"] = mqtt_willTopic;
    mqttObj["mqtt_willMessage"] = mqtt_willMessage;
    mqttObj["mqtt_willQos"] = mqtt_willQos;
    mqttObj["mqtt_willRetain"] = mqtt_willRetain ? true : false;
    jsonDoc["code"] = 1;
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// muestra el estado de las alarmas FIXME:
String apiGetAlarms()
{
    // para la actualizacion de la API
    ALARM_STATUS[0] = ALARM_STATUS1;
    ALARM_STATUS[1] = ALARM_STATUS2;
    ALARM_STATUS[2] = ALARM_STATUS3;
    ALARM_STATUS[3] = ALARM_STATUS4;
    ALARM_STATUS[4] = ALARM_STATUS5;
    ALARM_STATUS[5] = ALARM_STATUS6;
    ALARM_STATUS[6] = ALARM_STATUS7;
    ALARM_STATUS[7] = ALARM_STATUS8;
    bool reconocidas[NUM_ALARMS] = {ALARM_RECONOCIDA1, ALARM_RECONOCIDA2, ALARM_RECONOCIDA3, ALARM_RECONOCIDA4, ALARM_RECONOCIDA5, ALARM_RECONOCIDA6, ALARM_RECONOCIDA7, ALARM_RECONOCIDA8};
    String response = "";
    JsonDocument jsonDoc;

    jsonDoc["cantidad"] = NUM_ALARMS;
    jsonDoc["prueba_de_alarmas"] = prueba;
    // alarmas
    JsonArray alarmsObj = jsonDoc["alarmas"].to<JsonArray>();
    for (size_t i = 0; i < NUM_ALARMS; ++i)
    {
        JsonDocument obj;
        JsonObject alarms = obj.to<JsonObject>();
        alarms["numero"] = i + 1;               // falta actualizar
        alarms["name"] = ALARM_NAME[i].c_str(); // falta actualizar
        alarms["logic"] = logicas[i];
        alarms["status"] = ALARM_STATUS[i];
        alarms["reconocida"] = reconocidas[i]; // TODO: verificar
        alarms["on"] = fechaAct[i].c_str();
        alarms["off"] = fechaClear[i].c_str();
        alarms["count"] = cont[i];
        alarmsObj.add(alarms);
    }
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// apiGetRelays
// muestra la informacion de los relevadores
String apiGetRelay1() // TODO:
{

    String response = "";
    JsonDocument jsonDoc;
    // para los iconos
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;

    JsonObject r1Obj = jsonDoc["RELAY1"].to<JsonObject>();
    r1Obj["STATUS1"] = device_relay01; // es el estado del relevador1 (ON u OFF)
    //----------------------------------------------------------------
    r1Obj["DOMINGO1A"] = DOMINGO1A;
    r1Obj["LUNES1A"] = LUNES1A;
    r1Obj["MARTES1A"] = MARTES1A;
    r1Obj["MIERCOLES1A"] = MIERCOLES1A;
    r1Obj["JUEVES1A"] = JUEVES1A;
    r1Obj["VIERNES1A"] = VIERNES1A;
    r1Obj["SABADO1A"] = SABADO1A;
    r1Obj["DOMINGO1B"] = DOMINGO1B;
    r1Obj["LUNES1B"] = LUNES1B;
    r1Obj["MARTES1B"] = MARTES1B;
    r1Obj["MIERCOLES1B"] = MIERCOLES1B;
    r1Obj["JUEVES1B"] = JUEVES1B;
    r1Obj["VIERNES1B"] = VIERNES1B;
    r1Obj["SABADO1B"] = SABADO1B;
    //----------------------------------------------------------------
    r1Obj["R_NAME1"] = String(R_NAME1);
    r1Obj["R_DESCRIPTION1"] = R_DESCRIPTION1;
    r1Obj["R_TIMERON1"] = R_TIMERON1;         // si debe se encenderse por algun tiempo
    r1Obj["R_TIMER1"] = R_TIMER1;             // Que tiempo
    r1Obj["TEMPORIZADOR1"] = TEMPORIZADOR1;   // si tienes un horario de encendido o apagado
    r1Obj["TIMEONRELAY1A"] = TIMEONRELAY1A;   // a que horas enciende
    r1Obj["TIMEOFFRELAY1A"] = TIMEOFFRELAY1A; // a que horas se apaga
    r1Obj["TIMEONRELAY1B"] = TIMEONRELAY1B;   // a que horas enciende
    r1Obj["TIMEOFFRELAY1B"] = TIMEOFFRELAY1B; // a que horas se apaga
    r1Obj["FECHAON1"] = FECHAON1;             // la hora que se encendio
    r1Obj["FECHAOFF1"] = FECHAOFF1;           // la hora que se apago
    r1Obj["PROGRAMADO1"] = programado1;       // indica si esta programado
    //----------------------------------------------------------------
    jsonDoc["code"] = 1;
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// apiGetRelays
// muestra la informacion de los relevadores
String apiGetRelay2() // TODO:
{

    String response = "";
    JsonDocument jsonDoc;
    // para los iconos
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;

    JsonObject r2Obj = jsonDoc["RELAY2"].to<JsonObject>();
    r2Obj["STATUS2"] = device_relay02; // es el estado del relevador2(ON u OFF)
    r2Obj["DOMINGO2A"] = DOMINGO2A;
    r2Obj["LUNES2A"] = LUNES2A;
    r2Obj["MARTES2A"] = MARTES2A;
    r2Obj["MIERCOLES2A"] = MIERCOLES2A;
    r2Obj["JUEVES2A"] = JUEVES2A;
    r2Obj["VIERNES2A"] = VIERNES2A;
    r2Obj["SABADO2A"] = SABADO2A;
    r2Obj["DOMINGO2B"] = DOMINGO2B;
    r2Obj["LUNES2B"] = LUNES2B;
    r2Obj["MARTES2B"] = MARTES2B;
    r2Obj["MIERCOLES2B"] = MIERCOLES2B;
    r2Obj["JUEVES2B"] = JUEVES2B;
    r2Obj["VIERNES2B"] = VIERNES2B;
    r2Obj["SABADO2B"] = SABADO2B;
    //----------------------------------------------------------------
    r2Obj["R_NAME2"] = String(R_NAME2); // GPIO 18
    r2Obj["R_DESCRIPTION2"] = R_DESCRIPTION2;
    r2Obj["R_TIMERON2"] = R_TIMERON2;
    r2Obj["R_TIMER2"] = R_TIMER2;
    r2Obj["TEMPORIZADOR2"] = TEMPORIZADOR2;
    r2Obj["TIMEONRELAY2A"] = TIMEONRELAY2A;
    r2Obj["TIMEOFFRELAY2A"] = TIMEOFFRELAY2A;
    r2Obj["TIMEONRELAY2B"] = TIMEONRELAY2B;
    r2Obj["TIMEOFFRELAY2B"] = TIMEOFFRELAY2B;
    r2Obj["FECHAON2"] = FECHAON2;       // la hora que se encendio
    r2Obj["FECHAOFF2"] = FECHAOFF2;     // la hora que se apago
    r2Obj["PROGRAMADO2"] = programado2; // indica si esta programado
    jsonDoc["code"] = 1;
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// muestra las temperatuas
String apiGetTemperaturas()
{

    String response = "";
    JsonDocument jsonDoc;
    jsonDoc["serial"] = DeviceID();
    jsonDoc["device"] = platform();
    jsonDoc["tempDHT22"] = device_tempDHT;
    jsonDoc["device_ajuste_tempDHT"] = ajTmpDht22;
    jsonDoc["humRelativa"] = device_humedadDHT;
    jsonDoc["tempEvaporador"] = device_tempEvaporador;
    jsonDoc["device_ajuste_tempEvaporador"] = ajTmpP1;
    jsonDoc["tempCondensador"] = device_tempCondensador;
    jsonDoc["device_ajuste_tempCondensador"] = ajTmpP2;
    jsonDoc["tempLM35"] = device_lm35;
    jsonDoc["device_ajuste_lm35"] = ajTmpLm35;
    jsonDoc["code"] = 1;
    serializeJsonPretty(jsonDoc, response);
    return response;
}

// metodo POST ALARMAS
bool apiPostAlarms(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRelays()", "problemas al deserializar la data");
        return false;
    }

    if (doc["alarmas"][0]["name"] && doc["alarmas"][0]["name"].as<String>().length() > 0)
    {
        ALARM_NAME1 = doc["alarmas"][0]["name"].as<String>();
    }
    ALARM_RECONOCIDA1 = doc["alarmas"][0]["reconocida"].as<boolean>();
    if (doc["alarmas"][1]["name"] && doc["alarmas"][1]["name"].as<String>().length() > 0)
    {
        ALARM_NAME2 = doc["alarmas"][1]["name"].as<String>();
    }
    ALARM_RECONOCIDA2 = doc["alarmas"][1]["reconocida"].as<boolean>();
    if (doc["alarmas"][2]["name"] && doc["alarmas"][2]["name"].as<String>().length() > 0)
    {
        ALARM_NAME3 = doc["alarmas"][2]["name"].as<String>();
    }

    ALARM_RECONOCIDA3 = doc["alarmas"][2]["reconocida"].as<boolean>();

    if (doc["alarmas"][3]["name"] && doc["alarmas"][3]["name"].as<String>().length() > 0)
    {
        ALARM_NAME4 = doc["alarmas"][3]["name"].as<String>();
    }

    ALARM_RECONOCIDA4 = doc["alarmas"][3]["reconocida"].as<boolean>();

    if (doc["alarmas"][4]["name"] && doc["alarmas"][4]["name"].as<String>().length() > 0)
    {
        ALARM_NAME5 = doc["alarmas"][4]["name"].as<String>();
    }

    ALARM_RECONOCIDA5 = doc["alarmas"][4]["reconocida"].as<boolean>();

    if (doc["alarmas"][5]["name"] && doc["alarmas"][5]["name"].as<String>().length() > 0)
    {
        ALARM_NAME6 = doc["alarmas"][5]["name"].as<String>();
    }

    ALARM_RECONOCIDA6 = doc["alarmas"][5]["reconocida"].as<boolean>();

    if (doc["alarmas"][6]["name"] && doc["alarmas"][6]["name"].as<String>().length() > 0)
    {
        ALARM_NAME7 = doc["alarmas"][6]["name"].as<String>();
    }

    ALARM_RECONOCIDA7 = doc["alarmas"][6]["reconocida"].as<boolean>();

    if (doc["alarmas"][7]["name"] && doc["alarmas"][7]["name"].as<String>().length() > 0)
    {
        ALARM_NAME8 = doc["alarmas"][7]["name"].as<String>();
    }

    ALARM_RECONOCIDA8 = doc["alarmas"][7]["reconocida"].as<boolean>();

    // guardar la informacion
    return settingsSave();
}

// metodo POST RELAY apiPostRelays
bool apiPostRelay1(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRelays()", "problemas al deserializar la data");
        return false;
    }
    // validar el json
    /*if (!validationMqttParam(doc)) //FIXME: tal vez no es necesacio
    {
        return false;
    }*/
    // si paso los filtros anteriores empezamo a validar el MQTT
    RELAY1_STATUS = doc["STATUS1"].as<bool>();
    DOMINGO1A = doc["DOMINGO1A"].as<bool>();
    LUNES1A = doc["LUNES1A"].as<bool>();
    MARTES1A = doc["MARTES1A"].as<bool>();
    MIERCOLES1A = doc["MIERCOLES1A"].as<bool>();
    JUEVES1A = doc["JUEVES1A"].as<bool>();
    VIERNES1A = doc["VIERNES1A"].as<bool>();
    SABADO1A = doc["SABADO1A"].as<bool>();
    DOMINGO1B = doc["DOMINGO1B"].as<bool>();
    LUNES1B = doc["LUNES1B"].as<bool>();
    MARTES1B = doc["MARTES1B"].as<bool>();
    MIERCOLES1B = doc["MIERCOLES1B"].as<bool>();
    JUEVES1B = doc["JUEVES1B"].as<bool>();
    VIERNES1B = doc["VIERNES1B"].as<bool>();
    SABADO1B = doc["SABADO1B"].as<bool>();
    if (doc["R_NAME1"] && doc["R_NAME1"].as<String>().length() > 0)
    {
        strlcpy(R_NAME1, doc["R_NAME1"].as<String>().c_str(), sizeof(R_NAME1));
    }
    R_TIMERON1 = doc["R_TIMERON1"].as<bool>(); // si debe se encenderse por algun tiempo
    if (doc["R_TIMER1"])
    {
        R_TIMER1 = doc["R_TIMER1"].as<int>();
    }
    if (doc["R_DESCRIPTION1"] && doc["R_DESCRIPTION1"].as<String>().length() > 0)
    {
        R_DESCRIPTION1 = doc["R_DESCRIPTION1"].as<String>();
    }
    if (doc["TEMPORIZADOR1"] && doc["TEMPORIZADOR1"].as<String>().length() > 0)
    {
        TEMPORIZADOR1 = doc["TEMPORIZADOR1"].as<String>();
    }
    if (doc["TIMEONRELAY1A"] && doc["TIMEONRELAY1A"].as<String>().length() > 0)
    {
        TIMEONRELAY1A = doc["TIMEONRELAY1A"].as<String>();
    }
    if (doc["TIMEONRELAY1B"] && doc["TIMEONRELAY1B"].as<String>().length() > 0)
    {
        TIMEONRELAY1B = doc["TIMEONRELAY1B"].as<String>();
    }
    if (doc["TIMEOFFRELAY1A"] && doc["TIMEOFFRELAY1A"].as<String>().length() > 0)
    {
        TIMEOFFRELAY1A = doc["TIMEOFFRELAY1A"].as<String>();
    }
    if (doc["TIMEOFFRELAY1B"] && doc["TIMEOFFRELAY1B"].as<String>().length() > 0)
    {
        TIMEOFFRELAY1B = doc["TIMEOFFRELAY1B"].as<String>();
    }
    programado1 = doc["PROGRAMADO1"].as<bool>();

    // guardar la informacion
    return settingsSave();
}

// metodo POST RELAY apiPostRelays
bool apiPostRelay2(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRelays()", "problemas al deserializar la data");
        return false;
    }
    // validar el json
    /*if (!validationMqttParam(doc)) //FIXME: tal vez no es necesacio
    {
        return false;
    }*/
    // si paso los filtros anteriores empezamo a validar el MQTT

    RELAY2_STATUS = doc["STATUS2"].as<bool>();
    DOMINGO2A = doc["DOMINGO2A"].as<bool>();
    LUNES2A = doc["LUNES2A"].as<bool>();
    MARTES2A = doc["MARTES2A"].as<bool>();
    MIERCOLES2A = doc["MIERCOLES2A"].as<bool>();
    JUEVES2A = doc["JUEVES2A"].as<bool>();
    VIERNES2A = doc["VIERNES2A"].as<bool>();
    SABADO2A = doc["SABADO2A"].as<bool>();
    DOMINGO2B = doc["DOMINGO2B"].as<bool>();
    LUNES2B = doc["LUNES2B"].as<bool>();
    MARTES2B = doc["MARTES2B"].as<bool>();
    MIERCOLES2B = doc["MIERCOLES2B"].as<bool>();
    JUEVES2B = doc["JUEVES2B"].as<bool>();
    VIERNES2B = doc["VIERNES2B"].as<bool>();
    SABADO2B = doc["SABADO2B"].as<bool>();
    if (doc["R_NAME2"] && doc["R_NAME2"].as<String>().length() > 0)
    {
        strlcpy(R_NAME2, doc["R_NAME2"].as<String>().c_str(), sizeof(R_NAME2));
    }
    R_TIMERON2 = doc["R_TIMERON2"].as<bool>(); // si debe se encenderse por algun tiempo
    if (doc["R_TIMER2"])
    {
        R_TIMER2 = doc["R_TIMER2"].as<int>();
    }
    if (doc["R_DESCRIPTION2"] && doc["R_DESCRIPTION2"].as<String>().length() > 0)
    {
        R_DESCRIPTION2 = doc["R_DESCRIPTION2"].as<String>();
    }
    if (doc["TEMPORIZADOR2"] && doc["TEMPORIZADOR2"].as<String>().length() > 0)
    {
        TEMPORIZADOR2 = doc["TEMPORIZADOR2"].as<String>();
    }
    if (doc["TIMEONRELAY2A"] && doc["TIMEONRELAY2A"].as<String>().length() > 0)
    {
        TIMEONRELAY2A = doc["TIMEONRELAY2A"].as<String>();
    }
    // hasta aqui ok
    if (doc["TIMEONRELAY2B"] && doc["TIMEONRELAY2B"].as<String>().length() > 0)
    {
        TIMEONRELAY2B = doc["TIMEONRELAY2B"].as<String>();
    }
    if (doc["TIMEOFFRELAY2A"] && doc["TIMEOFFRELAY2A"].as<String>().length() > 0)
    {
        TIMEOFFRELAY2A = doc["TIMEOFFRELAY2A"].as<String>();
    }
    if (doc["TIMEOFFRELAY2B"] && doc["TIMEOFFRELAY2B"].as<String>().length() > 0)
    {
        TIMEOFFRELAY2B = doc["TIMEOFFRELAY2B"].as<String>();
    }
    programado2 = doc["PROGRAMADO2"].as<bool>();
    // guardar la informacion
    return settingsSave();
}

// metodo POST MQTT
bool apiPostMqtt(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    Serial.println("la data de apipostMqtt:");
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostMqtt()", "problemas al deserializar la data");
        // return false;
    }
    // validar el json
    if (!validationMqttParam(doc))
    {
        myLog("ERROR", "apifunctions.hpp", "validationMqttParam()", "ERROR EN LA VALIDACION");
        return false;
    }
    // si paso los filtros anteriores empezamo a validar el MQTT para guardar la informacion
    if (doc["mqtt_commandTopic_gerencia"] && doc["mqtt_commandTopic_gerencia"].as<String>().length() > 0)
    {
        gerencia = doc["mqtt_commandTopic_gerencia"].as<String>();
        // como se modifico hayq que actualizar
        // Serial.println("Se guarda el commandTopic");
        strlcpy(mqtt_commandTopic, PathMqttTopic("command").c_str(), sizeof(mqtt_commandTopic));
        // Serial.println(PathMqttTopic("command").c_str());
        // Serial.println(mqtt_commandTopic);
        strlcpy(mqtt_sendTopic, PathMqttTopic("device").c_str(), sizeof(mqtt_sendTopic));
        strlcpy(mqtt_willTopic, PathMqttTopic("status").c_str(), sizeof(mqtt_willTopic));
    }
    if (doc["mqtt_commandTopic_ciudad"] && doc["mqtt_commandTopic_ciudad"].as<String>().length() > 0)
    {
        ciudad = doc["mqtt_commandTopic_ciudad"].as<String>();
        // como se modifico hayq que actualizar
        strlcpy(mqtt_commandTopic, PathMqttTopic("command").c_str(), sizeof(mqtt_commandTopic));
        strlcpy(mqtt_sendTopic, PathMqttTopic("device").c_str(), sizeof(mqtt_sendTopic));
        strlcpy(mqtt_willTopic, PathMqttTopic("status").c_str(), sizeof(mqtt_willTopic));
    }
    if (doc["mqtt_commandTopic_central"] && doc["mqtt_commandTopic_central"].as<String>().length() > 0)
    {
        central = doc["mqtt_commandTopic_central"].as<String>();
        strlcpy(mqtt_commandTopic, PathMqttTopic("command").c_str(), sizeof(mqtt_commandTopic));
        strlcpy(mqtt_sendTopic, PathMqttTopic("device").c_str(), sizeof(mqtt_sendTopic));
        strlcpy(mqtt_willTopic, PathMqttTopic("status").c_str(), sizeof(mqtt_willTopic));
    }
    if (doc["mqtt_commandTopic_sala"] && doc["mqtt_commandTopic_sala"].as<String>().length() > 0)
    {
        sala = doc["mqtt_commandTopic_sala"].as<String>();
        strlcpy(mqtt_commandTopic, PathMqttTopic("command").c_str(), sizeof(mqtt_commandTopic));
        strlcpy(mqtt_sendTopic, PathMqttTopic("device").c_str(), sizeof(mqtt_sendTopic));
        strlcpy(mqtt_willTopic, PathMqttTopic("status").c_str(), sizeof(mqtt_willTopic));
    }
    mqtt_enabled = doc["mqtt_enabled"].as<bool>();
    if (doc["mqtt_server"] && doc["mqtt_server"].as<String>().length() > 0)
    {
        strlcpy(mqtt_server, doc["mqtt_server"].as<String>().c_str(), sizeof(mqtt_server));
    }
    if (doc["mqtt_port"]) // ya que no deberia llegar vacio
    {
        mqtt_port = doc["mqtt_port"].as<int>();
    }
    mqtt_retain = doc["mqtt_retain"].as<bool>();
    mqtt_qos = doc["mqtt_qos"].as<int>(); // cero o uno

    if (doc["mqtt_id"] && doc["mqtt_id"].as<String>().length() > 0)
    {
        strlcpy(mqtt_id, doc["mqtt_id"].as<String>().c_str(), sizeof(mqtt_id));
    }

    if (doc["mqtt_user"] && doc["mqtt_user"].as<String>().length() > 0)
    {
        strlcpy(mqtt_user, doc["mqtt_user"].as<String>().c_str(), sizeof(mqtt_user));
    }
    if (doc["mqtt_password"] && doc["mqtt_password"].as<String>().length() > 0)
    {
        strlcpy(mqtt_password, doc["mqtt_password"].as<String>().c_str(), sizeof(mqtt_password));
    }
    mqtt_clean_session = doc["mqtt_clean_session"].as<bool>();
    // se quitan debido a que la aplicacion no debe volver a guardar los valores modificando el resultado
    /*if (doc["mqtt_commandTopic"] && doc["mqtt_commandTopic"].as<String>().length() > 0)
    {
        strlcpy(mqtt_commandTopic, doc["mqtt_commandTopic"].as<String>().c_str(), sizeof(mqtt_commandTopic));
    }
    if (doc["mqtt_sendTopic"] && doc["mqtt_sendTopic"].as<String>().length() > 0)
    {
        strlcpy(mqtt_sendTopic, doc["mqtt_sendTopic"].as<String>().c_str(), sizeof(mqtt_sendTopic));
    }*/
    mqtt_time_send = doc["mqtt_time_send"].as<bool>();
    if (doc["mqtt_time_interval"])
    {
        mqtt_time_interval = doc["mqtt_time_interval"].as<int>() * 1000; // para que este en valores de millisegundos
    }
    mqtt_status_send = doc["mqtt_status_send"].as<bool>();
    /*if (doc["mqtt_willTopic"] && doc["mqtt_willTopic"].as<String>().length() > 0)
    {
        strlcpy(mqtt_willTopic, doc["mqtt_willTopic"].as<String>().c_str(), sizeof(mqtt_willTopic));
    }*/
    if (doc["mqtt_willMessage"] && doc["mqtt_willMessage"].as<String>().length() > 0)
    {
        strlcpy(mqtt_willMessage, doc["mqtt_willMessage"].as<String>().c_str(), sizeof(mqtt_willMessage));
    }
    mqtt_willRetain = doc["mqtt_willRetain"].as<bool>();
    mqtt_willQos = doc["mqtt_willQos"].as<int>();
    // guardar la informacion
    return settingsSave();
}

// ApiPostStatus
bool apiPostFecha(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        return false;
    }
    // validar el json
    /*if (!validationMqttParam(doc))
    {
        myLog("ERROR", "apifunctions.hpp", "validationMqttParam()", "ERROR EN LA VALIDACION");
        return false;
    }*/
    // si paso los filtros anteriores empezamo a validar el MQTT
    String newtime = doc["fecha"].as<String>();
    if (newtime.startsWith(":", 13) && newtime.startsWith("T", 10))
    { // si contiene en el tercer digito un dos puntos (:)
        cambiarfecha = true;
        nuevaFecha = newtime;
        setDyMsYr(); // pone a tiempo siempre y cuando la variable cambiarfecha igual a true
        return settingsSave();
    }
    else
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
        return false;
    }
    // guardar la informacion
}

// get status (apiGetStatus)
String apiGetStatus()
{
    String response = "";
    JsonDocument jsonDoc;
    // General
    jsonDoc["serial"] = DeviceID();
    jsonDoc["device"] = platform();
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;
    jsonDoc["fecha"] = fechaActual();
    jsonDoc["code"] = 1;
    serializeJsonPretty(jsonDoc, response);
    return response;
}
// get telegram (apiGetTelegram)
String apiGetTelegram()
{
    String response = "";
    JsonDocument jsonDoc;
    // General
    jsonDoc["telegram"] = telegram;
    jsonDoc["botToken"] = botToken;
    jsonDoc["chatID"] = chatID;

    serializeJsonPretty(jsonDoc, response);
    return response;
}

// post telegram
bool apiPostTelegram(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRelays()", "problemas al deserializar la data");
        return false;
    }
    // validar el json
    /*if (!validationMqttParam(doc)) //FIXME: tal vez no es necesacio
    {
        return false;
    }*/
    // si paso los filtros anteriores empezamo a validar el MQTT
    telegram = doc["telegram"].as<bool>();

    if (doc["botToken"] && doc["botToken"].as<String>().length() > 0)
    {
        botToken = doc["botToken"].as<String>();
    }

    if (doc["chatID"] && doc["chatID"].as<String>().length() > 0)
    {
        chatID = doc["chatID"].as<String>();
    }

    // guardar la informacion
    return settingsSave();
}

// get whatsapp
String apiGetWhatsapp()
{
    String response = "";
    JsonDocument jsonDoc;
    // General
    jsonDoc["whatsapp"] = whatsapp;
    jsonDoc["MobileNumber"] = MobileNumber;
    jsonDoc["APIKey"] = APIKey;

    serializeJsonPretty(jsonDoc, response);
    return response;
}

// post whatsapp
bool apiPostWhatsapp(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRelays()", "problemas al deserializar la data");
        return false;
    }
    // validar el json
    /*if (!validationMqttParam(doc)) //FIXME: tal vez no es necesacio
    {
        return false;
    }*/
    // si paso los filtros anteriores empezamo a validar el MQTT
    whatsapp = doc["whatsapp"].as<bool>();

    if (doc["MobileNumber"] && doc["MobileNumber"].as<String>().length() > 0)
    {
        MobileNumber = doc["MobileNumber"].as<String>();
    }

    if (doc["APIKey"] && doc["APIKey"].as<String>().length() > 0)
    {
        APIKey = doc["APIKey"].as<String>();
    }

    // guardar la informacion
    return settingsSave();
}

// reiniciar el ESP32
void apiPostRestart(const char *origen)
{
    char message[128];
    myLog("INFO", "apifunctions.hpp", "apiPostRestart()", "Restarting");
    snprintf(message, sizeof(message), "Dispositivo reiniciado desde: %s", origen); // por alguna razon esta instruccion no se ejecuta
    Serial.println("................................");
    Serial.flush(); // para que espere que se envie el mensaje
    delay(1000);
    ESP.restart();
}

// Restaurar el ESP32 a los valores iniciales
void apiPostRestore(const char *origen)
{
    settingsReset(); // para volve a los valores de fabrica
    deleteArchivoAlarmas();
    borrarBitacora();
    timeReset();
    dataGraficasReset();

    if (settingsSave() && timeSave() && dataGraficasSave())
    {
        apiPostRestart(origen);
    }
    else
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostRestore", "Error al guardar la configuracion");
    }
}
// funcion para el control de las salidas funciona tanto para la API como para MQTT
// Ejemplo para Relays:{"protocol":"API","output":"RELAY1","value":true}
// Ejemplo para Dimmer:{"protocol":"MQTT","output":"DIMMER","value":50}
bool apiPostControlDevice(const String &command) // cambiar el nombre de la funcion a apiPostAndMqttcontroldevice
{
    JsonDocument jsonCommand;
    DeserializationError error = deserializeJson(jsonCommand, command);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Error al deserializar el commando");
        return false;
    }
    String message = "Commando enviado desde: " + jsonCommand["protocol"].as<String>() + " => " + jsonCommand["output"].as<String>() + " => " + jsonCommand["value"].as<String>();
    myLog("INFO", "apifunctions.hpp", "apiPostControlDevice()", message.c_str());
    String output = jsonCommand["output"].as<String>();
    if (output.startsWith("RELAY1"))
    {
        bool relayValue1 = jsonCommand["value"];
        digitalWrite(RELAY1, relayValue1 ? HIGH : LOW);
        RELAY1_STATUS = relayValue1 ? HIGH : LOW;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("RELAY2"))
    {
        bool relayValue2 = jsonCommand["value"];
        digitalWrite(RELAY2, relayValue2 ? HIGH : LOW);
        RELAY2_STATUS = relayValue2 ? HIGH : LOW;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("TELEGRAM"))
    {
        bool onOffTele = jsonCommand["value"];

        telegram = onOffTele;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("TELEMENSAJE"))
    {
        bool mensaje = jsonCommand["value"];

        if (mensaje)
        {
            enviarMensaje("Bot " + sala + " en linea");
            return true;
        }
        return false;
    }
    else if (output.startsWith("BOTTOKEN"))
    {
        String teleTok = jsonCommand["value"].as<String>();

        botToken = teleTok;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("CHATID"))
    {
        String idChat = jsonCommand["value"].as<String>();

        chatID = idChat;
        settingsSave(); // guardar la configuracion
        return true;
    }
    //-----------------------------------------------------------------
    else if (output.startsWith("WHATSAPP"))
    {
        bool onOffWhatsapp = jsonCommand["value"];

        whatsapp = onOffWhatsapp;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("WHATSMENSAJE"))
    {
        bool menswhats = jsonCommand["value"];

        if (menswhats)
        {
            enviarMensajeWhatsapp("Bot " + sala + " en linea");
            return true;
        }
        return false;
    }
    else if (output.startsWith("NUMEROMOVIL"))
    {
        String cel = jsonCommand["value"].as<String>();

        MobileNumber = cel;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("APIKEY"))
    {
        String wapikey = jsonCommand["value"].as<String>();

        APIKey = wapikey;
        settingsSave(); // guardar la configuracion
        return true;
    }
    //-----------------------------------------------------------------
    else if (output.startsWith("BUZZER"))
    {
        bool buzzerValue = jsonCommand["value"];
        digitalWrite(BUZZER, buzzerValue ? HIGH : LOW);
        BUZZER_STATUS = buzzerValue ? HIGH : LOW;
        return true;
    }
    else if (output.startsWith("TIMEGRAFICA"))
    {
        int tgraf = jsonCommand["value"].as<int>();
        tgrafica = constrain(tgraf, 0, maxTimeMin); // sera un valor min de 1 y maximo de 1440 (1 dia)
        // Serial.println(tgrafica);
        settingsSave();
        return true;
    }
    else if (output.startsWith("DIMMER"))
    {
        dim = jsonCommand["value"].as<int>();
        dim = constrain(dim, 0, 100); // sera un valor min de 0 y maximo de 100
        if (settingsSave())
        {
            ledcWrite(ledChannel, dim * 2.55);
        }
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA1","value":true} ok
    else if (output.startsWith("ALARM_LOGICA1"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA1 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA2","value":true} ok
    else if (output.startsWith("ALARM_LOGICA2"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA2 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA3","value":true} ok
    else if (output.startsWith("ALARM_LOGICA3"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA3 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA4","value":true} ok
    else if (output.startsWith("ALARM_LOGICA4"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA4 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA5","value":true} ok
    else if (output.startsWith("ALARM_LOGICA5"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA5 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA6","value":true} ok
    else if (output.startsWith("ALARM_LOGICA6"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA6 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA7","value":true} ok
    else if (output.startsWith("ALARM_LOGICA7"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA7 = logicaValue ? true : false;
        settingsSave();
        return true;
    }
    // Ejemplo para la logica:{"protocol":"API","output":"ALARM_LOGICA8","value":true} ok
    else if (output.startsWith("ALARM_LOGICA8"))
    {
        bool logicaValue = jsonCommand["value"];
        ALARM_LOGICA8 = logicaValue ? true : false;
        settingsSave();
        return true;
    }

    else if (output.startsWith("DeviceName"))
    {
        strlcpy(device_name, jsonCommand["value"], sizeof(device_name));
        String id = jsonCommand["value"].as<String>();
        strlcpy(mqtt_willMessage, Willmsg(id).c_str(), sizeof(mqtt_willMessage));
        settingsSave(); // falta reiniciar
        return true;
    }
    // Ejemplo para cambiar el nombre de la alarma:{"protocol":"API","output":"ALARM_NAME1","value":"ALTA TEMPERATURA"}
    else if (output.startsWith("ALARM_NAME1"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME1 = nameValue;
        settingsSave(); // falta reiniciar
        return true;
    }
    else if (output.startsWith("reconocida1"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA1 = nameValue; // TODO: deberia ser un true //TODO: modificar en MQTT
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME2"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME2 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida2"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA2 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME3"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME3 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida3"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA3 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME4"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME4 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida4"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA4 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME5"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME5 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida5"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA5 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME6"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME6 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida6"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA6 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME7"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME7 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida7"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA7 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    else if (output.startsWith("ALARM_NAME8"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        ALARM_NAME8 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("reconocida8"))
    {
        bool nameValue = jsonCommand["value"].as<bool>();
        ALARM_RECONOCIDA8 = nameValue; // deberia ser un true
        // settingsSave();                // falta reiniciar
        return true;
    }
    // Ejemplo para cambiar el nombre del relevador:{"protocol":"API","output":"R_NAME1","value":"COMPRESOR1"} //RELAY1 GPIO17
    else if (output.startsWith("R_NAME1"))
    {
        strlcpy(R_NAME1, jsonCommand["value"], sizeof(R_NAME1));
        settingsSave();
        return true;
    }
    else if (output.startsWith("R_NAME2"))
    {
        strlcpy(R_NAME2, jsonCommand["value"], sizeof(R_NAME2));
        settingsSave();
        return true;
    }
    else if (output.startsWith("R_DESCRIPTION1"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        R_DESCRIPTION1 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("R_DESCRIPTION2"))
    {
        String nameValue = jsonCommand["value"].as<String>();
        R_DESCRIPTION2 = nameValue;
        settingsSave();
        return true;
    }
    else if (output.startsWith("TEMPORIZADOR1"))
    {
        bool tValue1 = jsonCommand["value"];
        TEMPORIZADOR1 = tValue1 ? true : false;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("TEMPORIZADOR2"))
    {
        bool tValue2 = jsonCommand["value"];
        TEMPORIZADOR2 = tValue2 ? true : false;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("TIMEONRELAY1A"))
    {
        String tOnValue1 = jsonCommand["value"].as<String>();
        if (tOnValue1.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEONRELAY1A = tOnValue1;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEONRELAY1B"))
    {
        String tOnValue1 = jsonCommand["value"].as<String>();
        if (tOnValue1.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEONRELAY1B = tOnValue1;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEONRELAY2A"))
    {
        String tOnValue2 = jsonCommand["value"].as<String>();
        if (tOnValue2.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEONRELAY2A = tOnValue2;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEONRELAY2B"))
    {
        String tOnValue2 = jsonCommand["value"].as<String>();
        if (tOnValue2.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEONRELAY2B = tOnValue2;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEOFFRELAY1A"))
    {
        String tOffValue1 = jsonCommand["value"].as<String>();
        if (tOffValue1.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEOFFRELAY1A = tOffValue1;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEOFFRELAY1B"))
    {
        String tOffValue1 = jsonCommand["value"].as<String>();
        if (tOffValue1.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEOFFRELAY1B = tOffValue1;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEOFFRELAY2A"))
    {
        String tOffValue2 = jsonCommand["value"].as<String>();
        if (tOffValue2.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEOFFRELAY2A = tOffValue2;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("TIMEOFFRELAY2B"))
    {
        String tOffValue2 = jsonCommand["value"].as<String>();
        if (tOffValue2.startsWith(":", 2))
        { // si contiene en el tercer digito un dos puntos (:)
            TIMEOFFRELAY2B = tOffValue2;
            settingsSave(); // guardar la configuracion
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Formato no aceptado");
            return false;
        }
    }
    else if (output.startsWith("programado1"))
    {
        bool prog1 = jsonCommand["value"];
        programado1 = prog1 ? true : false;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("programado2"))
    {
        bool prog2 = jsonCommand["value"];
        programado2 = prog2 ? true : false;
        settingsSave(); // guardar la configuracion
        return true;
    }
    else if (output.startsWith("device_ajuste_tempDHT"))
    {
        float valueAjDht22 = jsonCommand["value"].as<float>();
        Serial.print(valueAjDht22);
        ajTmpDht22 = constrain(valueAjDht22, -5.0, 5.0); // sera un valor min de -5 y maximo de 5
        if (-5.0 <= ajTmpDht22 <= 5.0)
        {
            settingsSave();
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "El valor de ajuste de temperatura del DHT22 debe estar entre -5 y 5 grados");
            return false;
        }
    }
    else if (output.startsWith("device_ajuste_lm35"))
    {
        float valueAjLM35 = jsonCommand["value"].as<float>();
        ajTmpLm35 = constrain(valueAjLM35, -5.0, 5.0); // sera un valor min de -5 y maximo de 5
        if (-5.0 <= ajTmpLm35 <= 5.0)
        {
            settingsSave();
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "El valor de ajuste de temperatura del LM35 debe estar entre -5 y 5 grados");
            return false;
        }
    }
    else if (output.startsWith("device_ajuste_tempEvaporador"))
    {
        float valueAjP1 = jsonCommand["value"].as<float>();
        ajTmpP1 = constrain(valueAjP1, -5.0, 5.0); // sera un valor min de -5 y maximo de 5
        if (-5.0 <= ajTmpP1 <= 5.0)
        {
            settingsSave();
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "El valor de ajuste de temperatura del Evaporador debe estar entre -5 y 5 grados");
            return false;
        }
    }
    else if (output.startsWith("device_ajuste_tempCondensador"))
    {
        float valueAjP2 = jsonCommand["value"].as<float>();
        ajTmpP2 = constrain(valueAjP2, -5.0, 5.0); // sera un valor min de -5 y maximo de 5
        if (-5.0 <= ajTmpP2 <= 5.0)
        {
            settingsSave();
            return true;
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "El valor de ajuste de temperatura del Condensador debe estar entre -5 y 5 grados");
            return false;
        }
    }
    // prueba_de_alarmas
    else if (output.startsWith("prueba_de_alarmas"))
    {
        bool valor = jsonCommand["value"].as<bool>();

        if (valor)
        {
            remotas = true;
            myLog("WARNING", "apifunctions.hpp", "apiPostControlDevice()", "Activando alarmas remotamente");
            return true;
        }
        else
        {
            remotas = false;
            myLog("INFO", "apifunctions.hpp", "apiPostControlDevice()", "Normalizando");
            return true;
        }
    }
    else if (output.startsWith("delete_file_alarms"))
    {
        bool valor = jsonCommand["value"].as<bool>();

        if (valor)
        {
            if (deleteArchivoAlarmas())
            {
                myLog("INFO", "apifunctions.hpp", "apiPostControlDevice()", "Archivo de alarmas borrado");
                return true;
            }
            else
            {
                myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "Error al borrar el archivo de alarmas");
                return false;
            }
        }
        else
        {
            myLog("ERROR", "apifunctions.hpp", "apiPostControlDevice()", "No se ha ejecutado ninguna operación");
            return true;
        }
    }
    else if (output.startsWith("RESTART"))
    {
        bool reiniciar = jsonCommand["value"];
        if (reiniciar)
        {
            settingsSave(); // guardar la configuracion
            Serial.println("[RESTART][RESTART][RESTART][RESTART][RESTART][RESTART][RESTART][RESTART]");
            Serial.flush();
            ESP.restart(); // reiniciar la ESP
        }
        myLog("INFO", "apifunctions.hpp", "apiPostControlDevice()", "Intento de reiniciar en false");
        return false;
    }
    else if (output.startsWith("RESTORE"))
    {
        String restore = jsonCommand["value"];
        if (restore == "21111979")
        {
            settingsReset();
            // resetear graficas
            dataGraficasReset();
            // resetear archivo de alarmas
            deleteArchivoAlarmas();
            borrarBitacora();
            Serial.println("[RESTORE][RESTORE][RESTORE][RESTORE][RESTORE][RESTORE][RESTORE][RESTORE]");
            Serial.flush();
            ESP.restart(); // reiniciar la ESP
        }
        myLog("INFO", "apifunctions.hpp", "apiPostControlDevice()", "Intento de restaurar falló no es la contraseña");
        return false;
    }
    // https://developer.mozilla.org/es/docs/Web/JavaScript/Reference/Global_Objects/String/startsWith
    else
    {
        myLog("WARNING", "apifunctions.hpp", "apiPostControlDevice()", "Output no encontrado");
        return false;
    }
}
// actualizar la contraseña
// Códigos de respuesta
// 0- Error de json enviado
// 1- Error de contraseña anterior en blanco
// 2- Error de contraseña nueva y confirmación en blanco
// 3- Error de contraseña nueva diferente a la anterior
// 4- Error de contraseña nueva y la confirmacion no coinciden
// 5- Error de contraseña anterior no es reconocida
// 6- Error en la actualización
// 7- Contraseña actualizada correctamente
int apiPostUser(const String &data)
{
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, data);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostUser()", "Error al deserializar el JSON");
        return 0;
    }
    // capturar las variables enviadas en el JSON
    String oldPassword = jsonDoc["device_old_password"].as<String>();
    String newPassword = jsonDoc["device_new_password"].as<String>();
    String confirmPassword = jsonDoc["device_c_new_password"].as<String>();
    // validar la contraseña anterior no nula o en blanco
    if (oldPassword.isEmpty())
    {
        return 1;
    }
    // validar que la contraseña nueva no nulas o en blanco y la confirmacion
    if (newPassword.isEmpty() && confirmPassword.isEmpty())
    {
        return 2;
    }
    // validar que la contraseña nueva no sea la misma que la anterior
    if (newPassword == device_password) //.c_str() convierte de string a char
    {
        return 3;
    }
    // validar que la contraseña nueva y la confirmacion coincidan
    if (newPassword != confirmPassword)
    {
        return 4;
    }
    // contraseña anterior coincida con la almacenada
    if (oldPassword != device_password)
    {
        return 5;
    }
    // Actualizar la contraseña
    strlcpy(device_password, newPassword.c_str(), sizeof(device_password));
    return (settingsSave()) ? 7 : 6;
}

// informacion para webSocket
String apiGetIndexWs()
{

    String response = "";
    JsonDocument jsonDoc;
    // wifi
    JsonObject wifiObj = jsonDoc["wifi"].to<JsonObject>();
    wifiObj["wifi_status"] = WiFi.status() == WL_CONNECTED ? true : false;
    wifiObj["wifi_ssid"] = wifi_mode == WIFI_AP_STA ? WiFi.SSID() : ap_ssid;
    wifiObj["wifi_ipv4"] = wifi_mode == WIFI_AP_STA ? ipToStr(WiFi.localIP()) : ipToStr(WiFi.softAPIP());
    wifiObj["wifi_mac"] = WiFi.macAddress();
    // mqtt
    JsonObject mqttObj = jsonDoc["mqtt"].to<JsonObject>();
    mqttObj["mqtt_status"] = mqttClient.connected() ? true : false;
    mqttObj["mqtt_server"] = mqttClient.connected() ? mqtt_server : "server not connected";
    mqttObj["mqtt_user"] = mqtt_user;
    mqttObj["mqtt_id"] = mqtt_id;
    // info
    JsonObject infoObj = jsonDoc["info"].to<JsonObject>();
    infoObj["device_fecha"] = device_fecha;
    infoObj["device_serial"] = DeviceSerial();
    infoObj["sala"] = sala;
    infoObj["device_name"] = device_name;
    infoObj["device_id"] = device_id;
    infoObj["device_mdns"] = "http://" + String(device_name) + ".local/";
    infoObj["device_firmware"] = device_fw_version;
    infoObj["device_hardware"] = device_hw_version;
    infoObj["device_manufacture"] = device_manufacture;
    infoObj["device_frequency"] = getCpuFrequencyMhz();
    infoObj["device_ram"] = ESP.getHeapSize() / 1024;                   // kb
    infoObj["device_spiffs"] = SPIFFS.totalBytes() / 1024;              // kb
    infoObj["device_flash"] = ESP.getFlashChipSize() / (1024.0 * 1024); // Mb
    infoObj["device_time"] = longTimeStr(millis() / 1000);              // segundos
    infoObj["device_restart"] = device_restart;
    infoObj["spiffs_used"] = SPIFFS.usedBytes() / 1024;                                          // kb
    infoObj["spiffs_total"] = SPIFFS.totalBytes() / 1024;                                        // kb
    infoObj["ram_available"] = ESP.getFreeHeap() / 1024;                                         // kb
    infoObj["ram_total"] = ESP.getHeapSize() / 1024;                                             // kb
    infoObj["wifi_rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;                      // kb
    infoObj["wifi_quality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0; // kb
    infoObj["tempEvaporador"] = device_tempEvaporador;
    infoObj["tempCondensador"] = device_tempCondensador;
    infoObj["tempLm35"] = device_lm35;
    infoObj["tcdht22"] = device_tempDHT;
    infoObj["humdht22"] = device_humedadDHT;
    infoObj["tmin"] = device_tempMinima;
    infoObj["tmax"] = device_tempMaxima;
    infoObj["buzzer_status"] = BUZZER_STATUS;
    // alarmas
    JsonArray alarmsObj = jsonDoc["alarmas"].to<JsonArray>();
    for (size_t i = 0; i < NUM_ALARMS; ++i)
    {
        JsonDocument obj;
        JsonObject alarms = obj.to<JsonObject>();
        alarms["numero"] = i + 1;               // falta actualizar
        alarms["name"] = ALARM_NAME[i].c_str(); // falta actualizar
        alarms["logic"] = logicas[i];
        alarms["status"] = ALARM_STATUS[i];
        alarms["reconocida"] = reconocidas[i]; // TODO: verificar
        alarms["on"] = fechaAct[i].c_str();
        alarms["off"] = fechaClear[i].c_str();
        alarms["count"] = cont[i];
        alarmsObj.add(alarms);
    }

    // salidas creandolas en bufers
    JsonDocument relay01Obj;
    JsonDocument relay02Obj;
    JsonDocument dimmerObj;
    JsonDocument tgrafObj;

    // relay1
    JsonObject relay1 = relay01Obj.to<JsonObject>();
    relay1["name"] = R_NAME1; //"RELAY1";
    relay1["status"] = RELAY1_STATUS ? true : false;
    // relay2
    JsonObject relay2 = relay02Obj.to<JsonObject>();
    relay2["name"] = R_NAME2; //"RELAY2";
    relay2["status"] = RELAY2_STATUS ? true : false;
    // dimmer
    JsonObject dimmer = dimmerObj.to<JsonObject>();
    dimmer["name"] = "DIMMER";
    dimmer["status"] = dim;
    JsonObject tgraf = tgrafObj.to<JsonObject>();
    tgraf["name"] = "tgrafica";
    tgraf["status"] = tgrafica;
    // crearemos un array de objetos
    JsonArray outputs = jsonDoc["outputs"].to<JsonArray>();
    outputs.add(relay1);
    outputs.add(relay2);
    outputs.add(dimmer);
    outputs.add(tgraf);
    // valores de las graficas
    JsonObject tdht22Obj = jsonDoc["tdht22"].to<JsonObject>();
    tdht22Obj["cT0"] = vTemp[1];
    tdht22Obj["cT1"] = vTemp[2];
    tdht22Obj["cT2"] = vTemp[3];
    tdht22Obj["cT3"] = vTemp[4];
    tdht22Obj["cT4"] = vTemp[5];
    tdht22Obj["cT5"] = vTemp[6];
    tdht22Obj["cT6"] = vTemp[7];
    tdht22Obj["cT7"] = vTemp[8];
    tdht22Obj["cT8"] = vTemp[9];
    tdht22Obj["cT9"] = vTemp[10];
    tdht22Obj["cT10"] = vTemp[11];
    tdht22Obj["cT11"] = vTemp[12];
    tdht22Obj["cT12"] = vTemp[13];
    tdht22Obj["cT13"] = vTemp[14];
    tdht22Obj["cT14"] = vTemp[15];
    tdht22Obj["cT15"] = vTemp[16];
    tdht22Obj["cT16"] = vTemp[17];
    tdht22Obj["cT17"] = vTemp[18];
    tdht22Obj["cT18"] = vTemp[19];
    tdht22Obj["cT19"] = vTemp[20];
    tdht22Obj["cT20"] = vTemp[21];
    tdht22Obj["cT21"] = vTemp[22];
    tdht22Obj["cT22"] = vTemp[23];
    // tdht22Obj["cT23"] = vTemp[24];
    JsonObject hdht22Obj = jsonDoc["hdht22"].to<JsonObject>();
    hdht22Obj["cH0"] = vHum[1];
    hdht22Obj["cH1"] = vHum[2];
    hdht22Obj["cH2"] = vHum[3];
    hdht22Obj["cH3"] = vHum[4];
    hdht22Obj["cH4"] = vHum[5];
    hdht22Obj["cH5"] = vHum[6];
    hdht22Obj["cH6"] = vHum[7];
    hdht22Obj["cH7"] = vHum[8];
    hdht22Obj["cH8"] = vHum[9];
    hdht22Obj["cH9"] = vHum[10];
    hdht22Obj["cH10"] = vHum[11];
    hdht22Obj["cH11"] = vHum[12];
    hdht22Obj["cH12"] = vHum[13];
    hdht22Obj["cH13"] = vHum[14];
    hdht22Obj["cH14"] = vHum[15];
    hdht22Obj["cH15"] = vHum[16];
    hdht22Obj["cH16"] = vHum[17];
    hdht22Obj["cH17"] = vHum[18];
    hdht22Obj["cH18"] = vHum[19];
    hdht22Obj["cH19"] = vHum[20];
    hdht22Obj["cH20"] = vHum[21];
    hdht22Obj["cH21"] = vHum[22];
    hdht22Obj["cH22"] = vHum[23];
    // hdht22Obj["cH23"] = vHum[24];
    JsonObject tEvaObj = jsonDoc["tEva"].to<JsonObject>();
    tEvaObj["tE0"] = vSensorPozo1[1];
    tEvaObj["tE1"] = vSensorPozo1[2];
    tEvaObj["tE2"] = vSensorPozo1[3];
    tEvaObj["tE3"] = vSensorPozo1[4];
    tEvaObj["tE4"] = vSensorPozo1[5];
    tEvaObj["tE5"] = vSensorPozo1[6];
    tEvaObj["tE6"] = vSensorPozo1[7];
    tEvaObj["tE7"] = vSensorPozo1[8];
    tEvaObj["tE8"] = vSensorPozo1[9];
    tEvaObj["tE9"] = vSensorPozo1[10];
    tEvaObj["tE10"] = vSensorPozo1[11];
    tEvaObj["tE11"] = vSensorPozo1[12];
    tEvaObj["tE12"] = vSensorPozo1[13];
    tEvaObj["tE13"] = vSensorPozo1[14];
    tEvaObj["tE14"] = vSensorPozo1[15];
    tEvaObj["tE15"] = vSensorPozo1[16];
    tEvaObj["tE16"] = vSensorPozo1[17];
    tEvaObj["tE17"] = vSensorPozo1[18];
    tEvaObj["tE18"] = vSensorPozo1[19];
    tEvaObj["tE19"] = vSensorPozo1[20];
    tEvaObj["tE20"] = vSensorPozo1[21];
    tEvaObj["tE21"] = vSensorPozo1[22];
    tEvaObj["tE22"] = vSensorPozo1[23];
    // tEvaObj["tE23"] = vSensorPozo1[24];
    JsonObject tCondObj = jsonDoc["tCond"].to<JsonObject>();
    tCondObj["tC0"] = vSensorPozo2[1];
    tCondObj["tC1"] = vSensorPozo2[2];
    tCondObj["tC2"] = vSensorPozo2[3];
    tCondObj["tC3"] = vSensorPozo2[4];
    tCondObj["tC4"] = vSensorPozo2[5];
    tCondObj["tC5"] = vSensorPozo2[6];
    tCondObj["tC6"] = vSensorPozo2[7];
    tCondObj["tC7"] = vSensorPozo2[8];
    tCondObj["tC8"] = vSensorPozo2[9];
    tCondObj["tC9"] = vSensorPozo2[10];
    tCondObj["tC10"] = vSensorPozo2[11];
    tCondObj["tC11"] = vSensorPozo2[12];
    tCondObj["tC12"] = vSensorPozo2[13];
    tCondObj["tC13"] = vSensorPozo2[14];
    tCondObj["tC14"] = vSensorPozo2[15];
    tCondObj["tC15"] = vSensorPozo2[16];
    tCondObj["tC16"] = vSensorPozo2[17];
    tCondObj["tC17"] = vSensorPozo2[18];
    tCondObj["tC18"] = vSensorPozo2[19];
    tCondObj["tC19"] = vSensorPozo2[20];
    tCondObj["tC20"] = vSensorPozo2[21];
    tCondObj["tC21"] = vSensorPozo2[22];
    tCondObj["tC22"] = vSensorPozo2[23];
    // tCondObj["tC23"] = vSensorPozo2[24];
    JsonObject tObj = jsonDoc["timeM"].to<JsonObject>();
    tObj["tm0"] = vTime[1];
    tObj["tm1"] = vTime[2];
    tObj["tm2"] = vTime[3];
    tObj["tm3"] = vTime[4];
    tObj["tm4"] = vTime[5];
    tObj["tm5"] = vTime[6];
    tObj["tm6"] = vTime[7];
    tObj["tm7"] = vTime[8];
    tObj["tm8"] = vTime[9];
    tObj["tm9"] = vTime[10];
    tObj["tm10"] = vTime[11];
    tObj["tm11"] = vTime[12];
    tObj["tm12"] = vTime[13];
    tObj["tm13"] = vTime[14];
    tObj["tm14"] = vTime[15];
    tObj["tm15"] = vTime[16];
    tObj["tm16"] = vTime[17];
    tObj["tm17"] = vTime[18];
    tObj["tm18"] = vTime[19];
    tObj["tm19"] = vTime[20];
    tObj["tm20"] = vTime[21];
    tObj["tm21"] = vTime[22];
    tObj["tm22"] = vTime[23];
    // tObj["tm23"] = vTime[24];
    //   Serializar
    serializeJsonPretty(jsonDoc, response);
    return response;
}

//-------------------post bitacora
// post bitácora
bool apiPostBitacora(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Serial.println("la data es:");
    // Serial.println(data);
    Serial.println(data);
    Serial.flush();
    serializeJsonPretty(doc, Serial);
    if (error)
    {
        myLog("ERROR", "apifunctions.hpp", "apiPostBitacora()", "problemas al deserializar la data");
        return false;
    }

    if (doc["nombretec"] && doc["nombretec"].as<String>().length() > 0)
    {
        nombretec = doc["nombretec"].as<String>();
    }

    if (doc["fecha"] && doc["fecha"].as<String>().length() > 0)
    {
        fecha = doc["fecha"].as<String>();
    }
    if (doc["tipo"] && doc["tipo"].as<String>().length() > 0)
    {
        tipo = doc["tipo"].as<String>();
    }
    if (doc["info"] && doc["info"].as<String>().length() > 0)
    {
        info = doc["info"].as<String>();
    }

    // guardar la informacion
    return salvarBitacora(nombretec, fecha, tipo, info);
}