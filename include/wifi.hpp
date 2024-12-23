WiFiMulti wifiMulti;

const char *esp_hostname = device_name;
bool wifi_change = false;          // para monitoriar si se conecto a la red WIFI
IPAddress ap_IPv4(192, 168, 4, 1); // por defecto es la IP del ESP32 en modo AP
IPAddress ap_subnet(255, 255, 255, 0);

constexpr byte DNSSERVER_PORT = 53;
DNSServer dnsServer;

constexpr unsigned long intervalWIFI = 30000; // 30 segundos

byte a = 0; // a debe ser menor a 255
byte w = 0;

// definicion de la funcion
String apiGetWifiScan();

void startAP()
{
    WiFi.disconnect(true);        // el true es para forzar
    NeoLedPixel(50, 255, 0, 255); // encendemos
    vTaskDelay(250);
    NeoLedPixel(0, 0, 0, 0); // apagamos
    vTaskDelay(250);
    myLog("INFO", "wifi.hpp", "startAP()", "WiFi en Modo AP ...");
    WiFi.softAPConfig(ap_IPv4, ap_IPv4, ap_subnet);
    WiFi.hostname(esp_hostname);
    WiFi.softAP(ap_ssid, ap_password, ap_channel, ap_oculto, ap_connect); // ok
    String message = "WiFi en modo AP " + String(ap_ssid) + " => IPv4 " + ipToStr(WiFi.softAPIP());
    myLog("INFO", "wifi.hpp", "startAP()", message.c_str());
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNSSERVER_PORT, "*", ap_IPv4);
    // IpMqtt = ipStr(WiFi.localIP()); //si no esta en WIFI
    wifi_app = WIFI_AP;
}

// funcion del modo cliente o STA
void startSTA()
{
    myLog("INFO", "wifi.hpp", "startSTA()", "Iniciando en modo Estación (STA) ...");
    NeoLedPixel(50, 255, 255, 255); // encendemos
    vTaskDelay(250);
    NeoLedPixel(0, 0, 0, 0); // apagamos
    vTaskDelay(250);
    WiFi.mode(WIFI_STA); // TODO: verificar si cambio todos lo WIFI_STA a WIFI_AP_STA en tareas.hpp y en wifi.hpp
    myLog("INFO", "wifi.hpp", "startSTA()", "Modo Estación (STA) ...");
    wifi_change = true;
    // verificar si el DHCP esta activado
    if (wifi_ip_static)
    { // si no viene el DHCP se utilizara la ip fija
        // IP fija
        myLog("INFO", "wifi.hpp", "!WiFi.config()", "Configurando IP ");
        if (!WiFi.config(CharToIP(wifi_ipv4), CharToIP(wifi_gateway), CharToIP(wifi_subnet), CharToIP(wifi_dns_primary), CharToIP(wifi_dns_secondary)))
        {
            myLog("ERROR", "wifi.hpp", "!WiFi.config()", "Fallo la configuración del modo Estación (STA) ...");
        }
    }
    else
    {
        myLog("INFO", "wifi.hpp", "!WiFi.config()", "DHCP Habilitado ");
    }
    myLog("INFO", "wifi.hpp", "startSTA()", "Iniciando en modo DHCP ...");
    WiFi.hostname(esp_hostname);
    // WiFi.begin(wifi_ssid, wifi_password);
    wifiMulti.addAP(wifi_ssid, wifi_password);
    wifiMulti.addAP(wifi_ssid2, wifi_password2);
    wifiMulti.run();
    String message = "Conectado al SSID " + String(WiFi.SSID());
    myLog("INFO", "wifi.hpp", "startSTA()", message.c_str());
    // FIXME: donde era 60 se puso 30
    for (byte attempts = 0; attempts < 30 && WiFi.status() != WL_CONNECTED; ++attempts)
    {
        myLog("ALERTA", "wifi.hpp", "startSTA()", "Intentando Conexión WIFI ....");
        NeoLedPixel(50, 255, 255, 0); // encendemos
        vTaskDelay(250);
        NeoLedPixel(0, 0, 0, 0); // apagamos
        vTaskDelay(250);
    }
    if (wifiMulti.run() == WL_CONNECTED)
    {
        String message = "Wifi conectado (" + String(WiFi.RSSI()) + " ) dBm => IPv4 " + ipToStr(WiFi.localIP()); // TODO: no muestra los valores de localIP
        myLog("INFO", "wifi.hpp", "startSTA()", message.c_str());
        // pestañep del NeoPixel
        blinkRandomNeoPixel(100, 1000, "amarillo"); // TODO: no hay parpadeo por que no hay un ciclo de repetición
        IpMqtt = ipToStr(WiFi.localIP());           // variable que se puede mandar por MQTT o WEBSOCKET
        wifi_app = WIFI_STA;
    }
    else
    {
        myLog("ERROR", "wifi.hpp", "startClientSTA()", "No se pudo Conectar a la red WIFI");
        blinkRandomNeoPixel(10, 1000, "rojo");

        startAP(); // llamamos al modo AP
    }
}

void wifiSetup()
{
    // primero hay que desconectarse
    WiFi.disconnect(true);
    myLog("DEBUG", "wifi.hpp", "wifiSetup()", "Desconexion...");
    // segun el modo hago el setup
    if (wifi_mode) // modo Estación
    {
        // myLog("DEBUG", "wifi.hpp", "wifiSetup()", "wifi_mode esta en true");
        startSTA();
        if (WiFi.status() == WL_CONNECTED)
        {
            WifiSsidMqtt = String(WiFi.SSID()); // para mandar la informacion por MQTT
            myLog("INFO", "wifi.hpp", "wifiSetup()", "WiFi en modo Estación (STA)");
            apiGetWifiScan(); // NOTE:llama al escaneo de las redes wifi
        }
    }
    else
    { // AP
        myLog("DEBUG", "wifi.hpp", "wifiSetup()", "wifi_mode esta en false");
        startAP();
        myLog("INFO", "wifi.hpp", "wifiSetup()", "WiFi en modo Punto de Acceso (AP)");
    }
    // iniciar el MDNS
    if (wifi_app = WIFI_STA || wifi_app == WIFI_AP)
    {
        if (MDNS.begin(esp_hostname))
        {
            MDNS.addService("http", "tcp", 80); // configuracion del MDNS escuchando el protocolo tcp sobre el puerto 80
        }
    }
}

// loop STA
void wifiSTALoop()
{
    static unsigned long previousMillisWIFI = 0;
    unsigned long currentMillis = millis();

    if (WiFi.status() != WL_CONNECTED && (currentMillis - previousMillisWIFI >= intervalWIFI))
    {
        NeoPixelBlink(1000, "naranja");
        WiFi.disconnect(true); // nos desconectamos
        WiFi.reconnect();
        previousMillisWIFI = currentMillis;
        if (++w == 2) // cada 60 segundos
        {
            myLog("WARNING", "wifi.hpp", "wifiSTALoop()", "Cambiando a Modo (AP)....");
            wifi_change = true;
            startAP();
            w = 0;
        }
        else
        {
            String message = "SSID " + String(WiFi.SSID()) + " desconectado";
            myLog("WARNING", "wifi.hpp", "wifiSTALoop()", message.c_str());
        }
    }
    else
    {
        blinkNeoPixelAsy(10, 1000, "amarillo"); // indica que esta conectado
    }
}

// loop AP
void wifiAPLoop()
{
    static unsigned long previousMillisAP = 0;
    unsigned long currentMillis = millis();

    blinkNeoPixelAsy(1000, 500, "morado"); // morado para AP
    dnsServer.processNextRequest();        // para que procese la nueva solicitud

    if (currentMillis - previousMillisAP >= intervalWIFI && wifi_change)
    {
        previousMillisAP = currentMillis;
        if (++a >= 4) // 4 = a 2 min, 6 = a 3 min
        {
            myLog("WARNING", "wifi.hpp", "wifiAPLoop()", "Cambiando a Modo estacion (STA)");
            wifi_change = false;
            startSTA();
            a = 0;
        }
    }
}