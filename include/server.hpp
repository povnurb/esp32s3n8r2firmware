//
#include <Update.h> //para actualizar el firmware

// Setting up the server //Configuracion del servidor
AsyncWebServer server(80);
// servidor Websocket
AsyncWebSocket ws("/ws");           // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events) escucha loseventos

bool security = true; // si esta en true pide usuario y contraseña y no deja hacer las pruebas con vue cli

// validar usuario y contraseña
void validateUserAndPasswordResponse(AsyncWebServerRequest *request)
{
    if (security && !request->authenticate(device_user, device_password))
    {
        return request->requestAuthentication();
    }
}
// definición de tipos de archivos
const char *dataTypeJson = "application/json";
const char *dataTypeHTML = "text/html";
const char *dataTypePNG = "image/png";
const char *dataTypeJavaScripts = "application/javascript";
const char *dataTypeWOFF = "font/woff";
const char *dataTypeWOFF2 = "font/woff2";
const char *dataEncoding = "gzip";

#include "api.hpp"
#include "vuejs.h" //todo el proyecto de vue

void initServer()
{
    // se incliye el siguiente para websocket
    // URL: http://192.168.1.160/api/indexws
    // es un metodo GET
    server.on("/api/indexws", HTTP_GET, handleApiIndexWs); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // API REST

    // URL: http://192.168.1.160/api/index
    // es un metodo GET //TODO: ver si no cambiar handleApiIndex por handleApiIndexWs
    server.on("/api/index", HTTP_GET, handleApiIndex); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // URL: http://192.168.1.160/api/index
    // es un metodo GET
    server.on("/api/info", HTTP_GET, handleApiInfo); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // URL: /api/connections/wifi
    // es un metodo GET
    server.on("/api/connections/wifi", HTTP_GET, handleApiWifi); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // URL: /api/connections/wifi (ENDPOINT)
    // es un metodo POST (verbo)
    server.on("/api/connections/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostWifi); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // URL: /api/connections/wifi-scan
    // es un metodo GET
    server.on("/api/connections/wifi-scan", HTTP_GET, handleApiWifiScan); // Escaneo de redes WIFI

    // URL: /api/connections/mqtt
    // es un metodo GET
    server.on("/api/connections/mqtt", HTTP_GET, handleApiMQTT);

    // URL: /api/device/alarms
    // es un metodo GET
    server.on("/api/device/alarms", HTTP_GET, handleApiGetAlarms);

    // URL: /api/device/relay1
    // es un metodo GET
    server.on("/api/device/relay1", HTTP_GET, handleApiGetRelay1);

    // URL: /api/device/relay2
    // es un metodo GET
    server.on("/api/device/relay2", HTTP_GET, handleApiGetRelay2);

    // URL: /api/device/alarms
    // es un metodo POST
    server.on("/api/device/alarms", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostAlarms);

    // URL: /api/device/relays
    // es un metodo POST
    server.on("/api/device/relay1", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostRelay1);

    // URL: /api/device/relays
    // es un metodo POST
    server.on("/api/device/relay2", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostRelay2);

    // URL: /api/device/temperaturas
    // es un metodo GET
    server.on("/api/device/temperaturas", HTTP_GET, handleApiGetTemperaturas);

    // URL: /api/connections/mqtt (ENDPOINT)
    // es un metodo POST (verbo)
    server.on("/api/connections/mqtt", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostMQTT); // handleApiIndex es una función que se encuentra en el archivo api.hpp

    // URL: /api/device/status
    // method: GET
    server.on("/api/device/status", HTTP_GET, handleApiGetStatus);

    // URL: /api/device/status
    // method: POST
    server.on("/api/device/status", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostFecha);

    // URL: /api/device/telegram
    // method: GET
    server.on("/api/device/telegram", HTTP_GET, handleApiGetTelegram);

    // URL: /api/device/telegram
    // method: POST
    server.on("/api/device/telegram", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostTelegram);

    // URL: /api/device/restart
    // method: POST
    server.on("/api/device/restart", HTTP_POST, handleApiPostRestart);

    // URL: /api/device/restore
    // method: POST
    server.on("/api/device/restore", HTTP_POST, handleApiPostRestore);

    // URL: /api/device/settings-download
    // method: GET
    server.on("/api/device/settings-download", HTTP_GET, handleApiDownload);

    // URL: /api/device/alarmas-download
    // method: GET
    server.on("/api/device/alarmas-download", HTTP_GET, handleApiDownloadAlarmas);

    // URL: /api/device/alarmas-historial
    // method: GET
    server.on("/api/device/alarmas-historial", HTTP_GET, handleApiHistorialAlarmas);

    //---------------------------------------------------------------------------------
    // -------------------------------------------------------------------
    // Manejo de la carga del archivo settings.json
    // url: "/api/device/upload"
    // Método: POST
    // -------------------------------------------------------------------
    server.on(
        "/api/device/upload", HTTP_POST, [](AsyncWebServerRequest *request)
        { opened = false; },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            handleApiUpload(request, filename, index, data, len, final);
        });

    // URL: /api/device/settings-firmware
    // method: POST
    server.on("/api/device/settings-firmware", HTTP_POST, [](AsyncWebServerRequest *request) {}, [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
              { handleApiFirmware(request, filename, index, data, len, final); });

    // URL: /api/device/control
    // method: POST
    server.on("/api/device/control", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostControl);

    // URL: /api/device/user
    // method: POST
    server.on("/api/device/user", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleApiPostUser);

    // URL: /api/device/logout
    // method: POST
    server.on("/api/device/logout", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  // validadacion del usuario y contraseña
                  validateUserAndPasswordResponse(request);
                  request->addInterestingHeader(HEADER_TEXT);
                  request->send(401, dataTypeJson, "{\"sesion\":false,\"msg\":\"Sesion cerrada correctamente\"}"); });

    //-------------------------------------------------------------------------------------------
    /*
    FIXME: NO FUNCIONO con la validacion
    // URL: /api/device/settings-upload
    // method: POST
    server.on("/api/device/settings-upload", HTTP_POST, [](AsyncWebServerRequest *request)
              { opened = false; }, [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
              { handleApiUpload(request, filename, index, data, len, final); });
    */
    // url: /api/device/settings-firmware
    // method: POST

    // Servidor web
    // para quitar errores de CORS en las peticiones POST de la pagina web
    // manejo del Error 404 pagina no encontrada - Desde VUE JS
    // url: "desconocido"
    server.onNotFound([](AsyncWebServerRequest *request)
                      {
                        validateUserAndPasswordResponse(request);//validacion user and password
                        /*request->addInterestingHeader(HEADER_TEXT);
                        AsyncWebServerResponse *response = request->beginResponse(200,dataTypeJson,"{\"msg\":\"Error 404\"}");
                        request->send(response); */
                        AsyncWebServerResponse *response = request->beginResponse_P(200,dataTypeHTML, index_html, index_html_length);
                        response->addHeader("Content-Encoding",dataEncoding);
                        request->send(response); });

    // Servidor web

    // estructura para las rutas
    struct FileRote
    {
        const char *path;
        const uint8_t *data;
        size_t length;
        const char *contentType;
        const char *encoding;
    };

    // definicion de las rutas
    // archivos js {29, 33, 68, 73, 135, 183, 261, 360, 414, 436, 491, 599, 642, 649, 671, 724, 855,}
    FileRote routes[] = {
        {"/", index_html, index_html_length, dataTypeHTML, dataEncoding},
        {"/favicon.png", favicon_png, favicon_png_length, dataTypePNG, dataEncoding},
        {"/js/app.js", app_js, app_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/chunk-vendors.js", chunk_vendors_js, chunk_vendors_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/23.js", file_23_js, file_23_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/29.js", file_29_js, file_29_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/33.js", file_33_js, file_33_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/68.js", file_68_js, file_68_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/73.js", file_73_js, file_73_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/135.js", file_135_js, file_135_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/183.js", file_183_js, file_183_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/261.js", file_261_js, file_261_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/289.js", file_289_js, file_289_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/338.js", file_338_js, file_338_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/333.js", file_333_js, file_333_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/360.js", file_360_js, file_360_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/414.js", file_414_js, file_414_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/436.js", file_436_js, file_436_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/487.js", file_487_js, file_487_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/491.js", file_491_js, file_491_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/573.js", file_573_js, file_573_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/599.js", file_599_js, file_599_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/642.js", file_642_js, file_642_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/649.js", file_649_js, file_649_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/671.js", file_671_js, file_671_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/724.js", file_724_js, file_724_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/837.js", file_837_js, file_837_js_length, dataTypeJavaScripts, dataEncoding},
        //{"/js/855.js", file_855_js, file_855_js_length, dataTypeJavaScripts, dataEncoding},
        {"/js/887.js", file_887_js, file_887_js_length, dataTypeJavaScripts, dataEncoding},
        {"/fonts/bootstrap-icons.woff", bootstrap_icons_woff, bootstrap_icons_woff_length, dataTypeWOFF, dataEncoding},
        {"/fonts/bootstrap-icons.woff2", bootstrap_icons_woff2, bootstrap_icons_woff2_length, dataTypeWOFF2, dataEncoding},

    };

    // Manejo de las peticiones
    for (const auto &route : routes)
    {
        server.on(route.path, HTTP_GET, [route](AsyncWebServerRequest *request)
                  {
                      validateUserAndPasswordResponse(request);
                      if (route.encoding)
                      {
                          AsyncWebServerResponse *response = request->beginResponse_P(200, route.contentType, route.data, route.length);
                          response->addHeader("Content-Encoding", route.encoding);
                          request->send(response);
                      } });
    }

    // inicializacion del servidor
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Method", "PUT, POST, GET, DELETE, OPTIONS");
    server.begin(); // iniciamos el servidor
    myLog("INFO", "server.hpp", "initServer()", "Servidor iniciado");
}