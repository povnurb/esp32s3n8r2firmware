
#include "apifunctions.hpp"
// declaraciones
void sendErrorResponse(AsyncWebServerRequest *request, const String &message);
void sendSuccessResponse(AsyncWebServerRequest *request, const String &message);

// aqui estanlos manejadores
const char *HEADER_TEXT = "API ESP32-S3 Server";
const char *JSON_SAVE_TRUE = "{\"save\":true}";
const char *JSON_ERROR = "{\"status\":\"Error de parámetros\"}";

// el siguiente es para websocket handleApiIndexWs
//  url: http://192.168.1.75/api/indexws
//  method: GET
void handleApiIndexWs(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiGetIndexWs());
}

// url: http://192.168.1.75/api/index
// method: GET
void handleApiIndex(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiGetIndex());
}

// url: http://192.168.1.75/api/device/alarmas-historial
// method: GET
void handleApiHistorialAlarmas(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiHistorialAlarmas());
}

// handleApiHistorialBitacora
void handleApiHistorialBitacora(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiHistorialBitacora());
}

void handleApiInfo(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiGetIndex());
}
// url: http://192.168.1.75/api/connections/wifi
// method: GET
void handleApiWifi(AsyncWebServerRequest *request) //(espera el AsyncWebServerRequest)
{
    // valido el usuario y contraseña
    validateUserAndPasswordResponse(request);   // validacion el usuario y contraseña
    request->addInterestingHeader(HEADER_TEXT); // este es el header que responde al cliente 200 es OK
    request->send(200, dataTypeJson, apiGetWifi());
}
// url: http://192.168.1.75/api/connections/wifi
// method: POST
void handleApiPostWifi(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostWifi(bodyContent))
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

// escanear reder
void handleApiWifiScan(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetWifiScan());
}

// get de las configuraciones de MQTT

void handleApiMQTT(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetMqtt());
}

// muestra el estado de las alarmas

void handleApiGetAlarms(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetAlarms());
}

// handleApiGetRelays
//  muestra el estado de los relays

void handleApiGetRelay1(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetRelay1());
}

void handleApiGetRelay2(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetRelay2());
}

// muestra las temperaturas
void handleApiGetTemperaturas(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetTemperaturas());
}

// POST Etiquetado de ALARMAS  handleApiPostAlarms
void handleApiPostAlarms(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostAlarms(bodyContent)) // para almacenar la data de MQTT responde con un true
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

// POST RELAYS  handleApiPostRelays
void handleApiPostRelay1(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostRelay1(bodyContent)) // para almacenar la data de MQTT responde con un true
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

void handleApiPostRelay2(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostRelay2(bodyContent)) // para almacenar la data de MQTT responde con un true
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

// seteo de las configuraciones de MQTT
// URL: /api/connections/mqtt/
void handleApiPostMQTT(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostMqtt(bodyContent)) // para almacenar la data de MQTT
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

void handleApiGetStatus(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetStatus());
}

// handleApiPostStatus
void handleApiPostFecha(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostFecha(bodyContent))
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

void handleApiGetTelegram(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, apiGetTelegram());
}

// handleApiPostTelegram
void handleApiPostTelegram(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostTelegram(bodyContent))
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

// handleApiPostBitacora
void handleApiPostBitacora(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    String bodyContent = GetBodyContent(data, len);
    // save
    if (apiPostBitacora(bodyContent)) // en el bodyContent viene la informacion de la aplicacion web
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(200, dataTypeJson, JSON_SAVE_TRUE);
    }
    else
    {
        request->addInterestingHeader(HEADER_TEXT);
        request->send(400, dataTypeJson, JSON_ERROR);
    }
}

// reiniciar el dispositivo
void handleApiPostRestart(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, "{\"restart\": true}");
    // llamar a la funcion de reinicio
    apiPostRestart("API");
}

// volver a los valores de fábrica
void handleApiPostRestore(AsyncWebServerRequest *request)
{
    // validar el usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    request->send(200, dataTypeJson, "{\"restore\":true}");
    delay(1000);
    // llamar a la funcion de reinicio
    apiPostRestore("API");
}

// descargar el archivo settings.json
void handleApiDownload(AsyncWebServerRequest *request)
{
    nuevaFecha = fechaActual(); // me de la fecha de descarga
    settingsSave();             // guarde la informacion actual del dispositivo
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    myLog("INFO", "api.hpp", "handleApiDownload", "Descargar del archivo settings.json");
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/settings.json", dataTypeJson, true);
    request->send(response);
}
// handleApiDownloadAlarmas
void handleApiDownloadAlarmas(AsyncWebServerRequest *request)
{
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    myLog("INFO", "api.hpp", "handleApiDownloadAlarmas", "Descargar del archivo alarmas.json");
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/alarmas.json", dataTypeJson, true);
    request->send(response);
}

// handleApiDownloadBitacora
void handleApiDownloadBitacora(AsyncWebServerRequest *request)
{
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    myLog("INFO", "api.hpp", "handleApiDownloadBitacora", "Descargar del archivo bitacora.json");
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/bitacora.json", dataTypeJson, true); // TODO: probar con dataTypeHTML
    request->send(response);
}

// subir el Firmware
void handleApiFirmware(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    // validacion de usuario y contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    // determinar el tipo de actualización
    int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH; // si el nombre del archivo contiene spiffs se actualiza la particion del spiffs y si no se actualiza la flash
    String updateSystem = (cmd == U_PART) ? "FileSystem" : "Firmware";
    if (index == 0)
    {
        content_len = request->contentLength();
        String message = "Actulización del " + updateSystem + " iniciado...";
        myLog("INFO", "api.hpp", "handleApiFirmware", message.c_str());
        // iniciamos al actualización
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
        {
            sendErrorResponse(request, "Error, No se pudo actualizar el " + updateSystem + "Index: " + filename + "!");
            return;
        }
    }
    // escribir el firmware o el filesystem
    if (Update.write(data, len) != len)
    {
        Update.printError(Serial);
    }
    // finalizar la acción
    if (final)
    {
        if (!Update.end(true))
        {
            sendErrorResponse(request, "Error, No se pudo actualizar el " + updateSystem + "Final:" + filename + "!");
        }
        else
        {
            sendSuccessResponse(request, updateSystem);
            apiPostRestart("API"); // reiniciar el dispositivo
        }
    }
}
//----------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------
// Manejo de la carga del archivo settings.json
// url: "/api/device/upload"
// Método: POST     modificado por que el de validaciones tiene detalles
// -------------------------------------------------------------------
// Variables para la carga del archivo
File file;
bool opened = false;
void handleApiUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (security)
    {
        if (!request->authenticate(device_user, device_password))
            return request->requestAuthentication();
    }
    if (filename != "settings.json")
    {
        myLog("ERROR", "api.hpp", "handleApiUpload()", "No es el archivo valido: settings.json");
        request->send(500, dataTypeJson, "{ \"save\": false, \"msg\": \"¡Error, No es el archivo valido: " + filename + " !\"}");
        return;
    }
    if (!index)
    {
        Serial.printf("INFO", "Upload Start: %s\n", filename.c_str());
        Serial.println("");
    }
    // Validar el archivo si esta abierto settings.json
    if (opened == false)
    {
        opened = true;
        // file = SPIFFS.open(String("/") + filename, FILE_WRITE); //se modifica a settings.json
        file = SPIFFS.open(String("/settings.json"), FILE_WRITE);
        if (!file)
        {
            myLog("ERROR", "api.hpp", "handleApiUpload()", "No se pudo abrir el archivo para escribir");
            request->send(500, dataTypeJson, "{ \"save\": false, \"msg\": \"¡Error, No se pudo abrir el archivo para escribir!\"}");
            return;
        }
    }

    // Escribir el archivo en la memoria
    if (file.write(data, len) != len)
    {
        myLog("ERROR", "api.hpp", "handleApiUpload()", "No se pudo escribir el archivo");
        request->send(500, dataTypeJson, "{ \"save\": false, \"msg\": \"¡Error, No se pudo escribir el archivo: " + filename + " !\"}");
        return;
    }
    myLog("SUCCESS", "api.hpp", "handleApiUpload()", "Archivo file Actualizado");
    // Finalizada la carga del archivo.
    if (final)
    {
        AsyncWebServerResponse *response = request->beginResponse(201, dataTypeJson, "{ \"save\": true, \"msg\": \"¡Carga del archivo: " + filename + " completada!\"}");
        response->addHeader("Cache-Control", "no-cache");
        response->addHeader("Location", "/");
        request->send(response);
        file.close();
        opened = false;
        myLog("INFO", "api.hpp", "handleApiUpload()", "Carga del archivo completada");
        // Esperar la transmisión de los datos seriales
        Serial.flush();
        // Reiniciar el ESP32
        apiPostRestart("API");
    }
}
// control de los reles y dimmer
//{"protocol":"API","output":"RELAY1","value":true}
//{"protocol":"API","output":"RELAY2","value":false}
//{"protocol":"API","output":"Dimmer","value":50}
void handleApiPostControl(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y la contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    String bodyContent = GetBodyContent(data, len);
    // validar el JSON de los comandos
    if (!validateCommandJsonKeys(bodyContent))
    {
        sendErrorResponse(request, "{ \"status\": false, \"msg\": \"Parámetros enviados incorrectos\" }");
        return;
    }
    // validar el json
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, bodyContent);
    if (error)
    {
        sendErrorResponse(request, JSON_ERROR);
        return;
    }
    if (apiPostControlDevice(bodyContent))
    {
        // sendSuccessResponse(request, "{\"status\":true, \"output\":\"" + doc["output"].as<String>() + "\"}");
        request->send(200, dataTypeJson, "{ \"status\": true, \"output\": \"" + doc["output"].as<String>() + "\" }");
    }
    else
    {
        // sendErrorResponse(request, "{\"status\":false, \"msg\":\"Comando no permitido\"}");
        request->send(400, dataTypeJson, "{ \"status\": false, \"msg\": \"Comando no permitido\" }");
    }
}

// actualizar contraseña
void handleApiPostUser(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // validar el usuario y la contraseña
    validateUserAndPasswordResponse(request);
    request->addInterestingHeader(HEADER_TEXT);
    // contenido de la información en string
    String bodyContent = GetBodyContent(data, len);
    int resp = apiPostUser(bodyContent);
    const char *errorMessages[] = {
        "�¡Error, JSON no valido!�",
        "�¡Error, No se permite contraseña anterior en blanco!�",
        "�¡Error, No se permite los datos de la nueva contraseña y confirmacion vacia!�",
        "�¡Error, La contraseña nueva no puede ser igual a la anterior!�",
        "�¡Error, La nueva contraseña y confirmación de nueva contraseña no coinciden!�",
        "�!Error, No se pudo actualizar la contraseña, la contraseña anterior no coinciden!�",
        "�!Error, No se pudo actualizar la contraseña en SPIFFS!�",
        "!Contraseña actualizada correctamente¡",
    };
    // respuestas
    if (resp >= 0 && resp < 7)
    {
        request->send(400, dataTypeJson, "{\"save\": false,\"msg\":\"" + String(errorMessages[resp]) + "\"}");
    }
    else if (resp == 7)
    {
        request->send(200, dataTypeJson, "{\"save\": true,\"msg\":\"" + String(errorMessages[resp]) + "\"}");
    }
    else
    {
        request->send(500, dataTypeJson, "{ \"save\": false,  \"msg\": \"" + String(errorMessages[resp]) + "\" }");
    }
}

// respuesta generica para los errores
void sendErrorResponse(AsyncWebServerRequest *request, const String &message)
{
    AsyncWebServerResponse *response = request->beginResponse(400, dataTypeJson, "{ \"save\": false, \"msg\": \"" + message + "\"}");
    request->send(response);
    Update.printError(Serial);
    myLog("ERROR", "api.hpp", "sendErrorResponse", message.c_str());
}

// respuesta generica para los exitos

void sendSuccessResponse(AsyncWebServerRequest *request, const String &message)
{
    AsyncWebServerResponse *response = request->beginResponse(201, dataTypeJson, "{ \"save\": true, \"msg\": \"" + message + "\"}");
    response->addHeader("Cache-Control", "no-cache"); // para que no lo guarde ne cache
    response->addHeader("Location", "/");             // para que lo mande al inicio
    request->send(response);
    myLog("SUCCESS", "api.hpp", "sendSuccessResponse", message.c_str());
}
// TODO: pendiente modificar fechas, datos de los relays,