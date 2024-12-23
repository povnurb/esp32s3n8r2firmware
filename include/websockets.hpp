// long lastWSend = 0;
//  agregar un interruptor para activar WS
//  declaración de funciones
// capturar los comandos por WebSockets
void ProcessRequest(AsyncWebSocketClient *client, String request)
{
    String command = request;
    command.trim();
    // TODO: procesar mensajes de comando
};
// para la otra usar la libreria Arduinowebsocket o investigar la de espresift para activar el wss:
//-----------------------------------------------------------------------------
//  funcion predefinida en la libreria de WebSockets
//  Eventos del websocket
//--------------------------------------------------------------------
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("[ INFO ] ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("{\"ClientID\": %u }", client->id());
        client->ping();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        // Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        myLog("ERROR", "websockets.hpp", "onWsEvent()", "Errores en websockets -24");
        // Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    }
    else if (type == WS_EVT_PONG)
    {
        // Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len)
        {
            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < info->len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[3];
                for (size_t i = 0; i < info->len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            if (info->opcode == WS_TEXT)
                ProcessRequest(client, msg);
        }
        else
        {
            // message is comprised of multiple frames or the frame is split into multiple packets
            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[3];
                for (size_t i = 0; i < len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if ((info->index + len) == info->len)
            {
                if (info->final)
                {
                    if (info->message_opcode == WS_TEXT)
                        ProcessRequest(client, msg);
                }
            }
        }
    }
}
// -------------------------------------------------------------------
// Manejador de ordenes que llegan por Websocket
// -------------------------------------------------------------------
/*
void ProcessRequest(AsyncWebSocketClient *client, String request)
{

    String command = request;
    command.trim(); // limpia espacios

    if (strcmp(command.c_str(), "restore") == 0)
    {
        myLog("INFO", "websockets.hpp", "ProcessRequest()", "Commando restaurar por WS => restore"); //+ command FIXME:
    }
    else if (strcmp(command.c_str(), "restart") == 0)
    {
        myLog("INFO", "websockets.hpp", "ProcessRequest()", "Commando reiniciar por WS => restart "); //+ command FIXME:
    }
    else
    {
        // OnOffRelays(command); // verificar si esta correcto
        Serial.print("comandos pendientes"); // TODO: and FIXME:
    }
}*/
// -------------------------------------------------------------------
// Inicializar el Websocket
// -------------------------------------------------------------------
// mas información //https://techtutorialsx.com/2018/08/14/esp32-async-http-web-server-websockets-introduction/
void initWebSockets()
{
    ws.onEvent(onWsEvent);  // es el callback
    server.addHandler(&ws); // puntero de websockets
    myLog("INFO", "websockets.hpp", "initWebsockets()", "Servidor WebSocket iniciado");
}
// -------------------------------------------------------------------
// Función para enviar mensaje por Websocket tipo json
// -------------------------------------------------------------------
void wsMessageSend()
{
    ws.textAll(apiGetIndexWs()); // TODO:para enviar a todos los que esten conectados los datos
    ws.cleanupClients();         // limpieza de los clientes ws
}

// imprimier el progreso del firmware

void printFirmwareProgress(size_t prog, size_t sz)
{
    static int lastSendProgress = -1;
    int progress = (prog * 100) / sz;

    if (progress >= 0 && progress <= 100 && (progress - lastSendProgress >= 2)) // subio un 2%
    {
        lastSendProgress = progress;
        ws.textAll(getSendJson("update", String(progress)));
        String progressString = "Memoria llenada al: " + String(progress) + "%";
        myLog("INFO", "websockets.hpp", "printFirmwareProgress()", progressString.c_str()); // convierte a caracter
    }
}
/*
void wsMessageSend(String msg, String icon, String Type)
{
    if (strcmp(Type.c_str(), "info") == 0)
    {
        String response;
        JsonDocument doc;
        doc["type"] = Type;
        doc["msg"] = msg;
        doc["icon"] = icon;
        serializeJson(doc, response);
        ws.textAll(response);
    }
    else
    {
        ws.textAll(msg);
    }
}*/
// -------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS Datos para Index cada 1s
// -------------------------------------------------------------------
/*
String getJsonIndex()
{
    String response = "";
    JsonDocument jsonDoc; // antes 3000
    jsonDoc["type"] = "data";
    jsonDoc["activeTime"] = longTimeStr(millis() / 1000);
    // jsonDoc["ramAvailable"] = ESP.getFreeHeap();
    // jsonDoc["RAM_SIZE_KB"] = ESP.getHeapSize();
    jsonDoc["mqttStatus"] = mqttClient.connected() ? true : false;
    jsonDoc["mqtt_server"] = mqttClient.connected() ? F(mqtt_server) : F("server not connected");
    jsonDoc["wifiStatus"] = WiFi.status() == WL_CONNECTED ? true : false;
    jsonDoc["rssiStatus"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifiQuality"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["status1"] = RELAY1_STATUS ? true : false;
    jsonDoc["status2"] = RELAY2_STATUS ? true : false;
    jsonDoc["dimmer"] = dim;
    // jsonDoc["cpuTemp"] = TempCPUValue();
    jsonDoc["tC"] = String(Temperatura());
    jsonDoc["hum"] = String(Humedad());
    jsonDoc["tmin"] = String(tempMin());
    jsonDoc["tmax"] = String(tempMax());
    jsonDoc["BUZZER_STATUS"] = BUZZER_STATUS;
    //jsonDoc["ALRM_NAME1"] = ALRM_NAME1;
    //jsonDoc["ALRM_STATUS1"] = digitalRead(ALARM_PIN1) ? true : false;
    //jsonDoc["ALRM_TON1"] = ALRM_TON1;
    //jsonDoc["ALRM_TOFF1"] = ALRM_TOFF1;
    //jsonDoc["ALRM_NAME2"] = ALRM_NAME2;
    //jsonDoc["ALRM_STATUS2"] = digitalRead(ALARM_PIN2) ? true : false;
    //jsonDoc["ALRM_TON2"] = ALRM_TON2;
    //jsonDoc["ALRM_TOFF2"] = ALRM_TOFF2;
    //jsonDoc["ALRM_NAME3"] = ALRM_NAME3;
    //jsonDoc["ALRM_STATUS3"] = digitalRead(ALARM_PIN3) ? true : false;
    //jsonDoc["ALRM_TON3"] = ALRM_TON3;
    //jsonDoc["ALRM_TOFF3"] = ALRM_TOFF3;
    //jsonDoc["ALRM_NAME4"] = ALRM_NAME4;
    //jsonDoc["ALRM_STATUS4"] = digitalRead(CLIMAA) ? true : false;
    //jsonDoc["ALRM_TON4"] = ALRM_TON4;
    //jsonDoc["ALRM_TOFF4"] = ALRM_TOFF4;
    //jsonDoc["ALRM_NAME5"] = ALRM_NAME5;
    //jsonDoc["ALRM_STATUS5"] = digitalRead(CLIMAB) ? true : false;
    //jsonDoc["ALRM_TON5"] = ALRM_TON5;
    //jsonDoc["ALRM_TOFF5"] = ALRM_TOFF5;
    jsonDoc["cT0"] = vTemp[1];
    jsonDoc["cT1"] = vTemp[2];
    jsonDoc["cT2"] = vTemp[3];
    jsonDoc["cT3"] = vTemp[4];
    jsonDoc["cT4"] = vTemp[5];
    jsonDoc["cT5"] = vTemp[6];
    jsonDoc["cT6"] = vTemp[7];
    jsonDoc["cT7"] = vTemp[8];
    jsonDoc["cT8"] = vTemp[9];
    jsonDoc["cT9"] = vTemp[10];
    jsonDoc["cT10"] = vTemp[11];
    jsonDoc["cT11"] = vTemp[12];
    jsonDoc["cT12"] = vTemp[13];
    jsonDoc["cT13"] = vTemp[14];
    jsonDoc["cT14"] = vTemp[15];
    jsonDoc["cT15"] = vTemp[16];
    jsonDoc["cT16"] = vTemp[17];
    jsonDoc["cT17"] = vTemp[18];
    jsonDoc["cT18"] = vTemp[19];
    jsonDoc["cT19"] = vTemp[20];
    jsonDoc["cT20"] = vTemp[21];
    jsonDoc["cT21"] = vTemp[22];
    jsonDoc["cT22"] = vTemp[23];
    jsonDoc["cT23"] = vTemp[24];
    jsonDoc["cH0"] = vHum[1];
    jsonDoc["cH1"] = vHum[2];
    jsonDoc["cH2"] = vHum[3];
    jsonDoc["cH3"] = vHum[4];
    jsonDoc["cH4"] = vHum[5];
    jsonDoc["cH5"] = vHum[6];
    jsonDoc["cH6"] = vHum[7];
    jsonDoc["cH7"] = vHum[8];
    jsonDoc["cH8"] = vHum[9];
    jsonDoc["cH9"] = vHum[10];
    jsonDoc["cH10"] = vHum[11];
    jsonDoc["cH11"] = vHum[12];
    jsonDoc["cH12"] = vHum[13];
    jsonDoc["cH13"] = vHum[14];
    jsonDoc["cH14"] = vHum[15];
    jsonDoc["cH15"] = vHum[16];
    jsonDoc["cH16"] = vHum[17];
    jsonDoc["cH17"] = vHum[18];
    jsonDoc["cH18"] = vHum[19];
    jsonDoc["cH19"] = vHum[20];
    jsonDoc["cH20"] = vHum[21];
    jsonDoc["cH21"] = vHum[22];
    jsonDoc["cH22"] = vHum[23];
    jsonDoc["cH23"] = vHum[24];
    serializeJson(jsonDoc, response);
    return response;
}*/
