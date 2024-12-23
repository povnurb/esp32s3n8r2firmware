// definir funciones
bool dataGraficasSave();
bool validateNestedKeys(JsonObject &obj, const char *keys[], size_t keysSize);
// realizar un log en el puerto serial
void myLog(const char *type, const char *arch, const char *func, const char *msg)
{
    // type: [DEBUG, INFO, WARN, ERROR]
    Serial.printf("[%s] [%s] [%s] %s\n", type, arch, func, msg);
}

// platform
const char *platform()
{
#if defined(ARDUINO_ESP32_DEV)
    return "ESP32";
#else
    return "ESP32-S3";
#endif
}

String hexStr(const unsigned long &h, const byte &l = 8)
{
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - l);
    return s;
}
// UniqueID
String UniqueID()
{
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    String id = String(chip, HEX);
    id.toUpperCase();

    while (id.length() < 4)
    {
        id = "0" + id;
    }
    return id;
}

// Device ID unico
String DeviceID()
{
    return platform() + hexStr(ESP.getEfuseMac(), 8) + UniqueID();
}
String DeviceSerial()
{
    return hexStr(ESP.getEfuseMac(), 8) + UniqueID();
}

// Crear topico para MQTT
//  //usuario/+/id/# => /lalo/vue32/ESP32299b1c52100C3D/status = true/false
String PathMqttTopic(const String &topic)
{
    // Serial.println(gerencia);//TODO: si se arma el path
    return String(gerencia + "/" + ciudad + "/" + central + "/" + sala + "/" + String(UniqueID()) + "/" + topic);
}
String Willmsg(const String name)
{
    return "{\"DeviceID\":\"" + name + "\", \"connected\":false}";
}

// se definen los dos pines de salida de los relays
const int outputsPins[NUM_RELAYS] = {RELAY1, RELAY2};
// se define las ocho pines de alarmas
const int inputsPins[NUM_ALARMS] = {ALARMPIN1, ALARMPIN2, ALARMPIN3, ALARMPIN4, ALARMPIN5, ALARMPIN6, ALARMPIN7, ALARMPIN8};
// apagar todas las salidas
void turnOffOutputs()
{
    for (int i = 0; i < NUM_RELAYS; i++)
    {
        digitalWrite(outputsPins[i], LOW);
    }
}
// definir pines
void gpioDefineOut() // pines de salida
{
    pinMode(BUZZER, OUTPUT);
    for (int i = 0; i < NUM_RELAYS; i++)
    {
        pinMode(outputsPins[i], OUTPUT);
    }
    // PWM
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(DIMMER, ledChannel);
    // setear todo a 0 o apagado
    turnOffOutputs();
    ledcWrite(ledChannel, 0);
}
/*void gpioDefineIn() // pines de entrada
{
    for (int i = 0; i < NUM_ALARMS; i++)
    {
        pinMode(inputsPins[i], INPUT_PULLUP);
    }
}*/
// APAGAR o ENCENDER UNA SALIDA
void setOnOffSingle(int _pin, bool status)
{
    digitalWrite(_pin, status);
}
// char to ip address
IPAddress CharToIP(const char *str)
{
    uint8_t ip[4] = {0};
    int parseOctets = sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    if (parseOctets == 4)
    {
        return IPAddress(ip[0], ip[1], ip[2], ip[3]);
    }
    else
    {
        return IPAddress(0, 0, 0, 0);
    }
}
// NeoLedPixel: valores de 0 a 255
void NeoLedPixel(int brillo, int rojo, int verde, int azul)
{
    NeoPixel.clear(); // apaga el pixel
    // brillo
    NeoPixel.setBrightness(brillo); // el valor va de 0 a 255
    // configuracion del pixel
    NeoPixel.setPixelColor(0, rojo, verde, azul); // posicion es el primer pixel de la NeoPixel, R,G,B(Rojo, Verde, Azul)
    NeoPixel.show();                              // muestra la configuración
}
// conviente una ip a string
String ipToStr(const IPAddress &ip)
{
    String sFn = "";
    for (byte bFn = 0; bFn < 3; bFn++)
    {
        sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
    }
    sFn += String(((ip >> 8 * 3)) & 0xFF);
    return sFn;
}
// pestañeo del NeoPixel tiempo random con color rojo, verde, amarillo, azul, blanco
void blinkRandomNeoPixel(int minTime, int maxTime, String color)
{
    static unsigned long lastBlinkTime = 0;
    static bool ioBlink = false;
    static unsigned long rndTemp = 0;

    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= rndTemp)
    {
        lastBlinkTime = currentMillis;
        ioBlink = !ioBlink;
        if (ioBlink)
        {
            // myLog("INFO", "funtions.hpp", "blinkRandomNeoPixel()", "NeoPixel(Enciende)");
            if (color == "verde")
                NeoLedPixel(50, 0, 255, 0);
            if (color == "amarillo")
                NeoLedPixel(50, 255, 255, 0);
            if (color == "verde")
                NeoLedPixel(50, 0, 255, 0);
            if (color == "azul")
                NeoLedPixel(50, 0, 0, 255);
            if (color == "blanco")
                NeoLedPixel(50, 255, 255, 255);
        }
        else
        {
            // myLog("INFO", "funtions.hpp", "blinkRandomNeoPixel()", "NeoPixel(Apaga)");
            NeoLedPixel(0, 0, 0, 0);
        }
        rndTemp = random(minTime, maxTime);
    }
}

// pestañeo de Neopixel cada determinado tiempo color verde,rojo,azul,blanco,amarillo
void NeoPixelBlink(int speed, String color)
{
    static unsigned long lastBlinkTime = 0; // tiempo de ultimo parpadeo
    static bool ioBlink = false;
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= speed)
    {
        lastBlinkTime = currentMillis;
        ioBlink = !ioBlink;
        if (ioBlink)
        {
            myLog("DEBUG", "funtions.hpp", "NeoBlinkSingle()", "NeoPixel(Enciende)");
            if (color == "verde")
                NeoLedPixel(50, 0, 255, 0);
            if (color == "amarillo")
                NeoLedPixel(50, 255, 255, 0);
            if (color == "verde")
                NeoLedPixel(50, 0, 255, 0);
            if (color == "azul")
                NeoLedPixel(50, 0, 0, 255);
            if (color == "blanco")
                NeoLedPixel(50, 255, 255, 255);
            if (color == "naranja")
                NeoLedPixel(50, 255, 165, 0);
            if (color == "morado")
                NeoLedPixel(50, 255, 0, 255); // MORADO
        }
        else
        {
            myLog("DEBUG", "funtions.hpp", "NeoBlinkSingle()", "NeoPixel(Apaga)");
            NeoLedPixel(0, 0, 0, 0);
        }
    }
}

// encender y apagar el NeoPixel por tiempo en color verde,rojo,amarillo,azul,blanco,naranja,morado
void blinkNeoPixelAsy(int timeHigh, int timeLow, String color)
{
    static unsigned long lastBlinkTime = 0; // tiempo de ultimo parpadeo
    static bool ioBlink = false;
    static int currentTime = 0; // tiempo actual
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= currentTime)
    {
        lastBlinkTime = currentMillis;
        ioBlink = !ioBlink;
        if (ioBlink)
        {
            // myLog("DEBUG", "funtions.hpp", "blinkNeoPixelAsy()", "NeoPixel(Enciende)"); //ok
            if (color == "verde")
                NeoLedPixel(50, 0, 255, 0); // WIFI
            if (color == "amarillo")
                NeoLedPixel(50, 255, 255, 0);
            if (color == "azul")
                NeoLedPixel(50, 0, 0, 255);
            if (color == "blanco")
                NeoLedPixel(50, 255, 255, 255);
            if (color == "rojo")
                NeoLedPixel(50, 255, 0, 0);
            if (color == "morado")
                NeoLedPixel(50, 255, 0, 255); // AP
            if (color == "naranja")
                NeoLedPixel(50, 255, 165, 0); // intentando conexion al MQTT
        }
        else
        {
            // myLog("DEBUG", "funtions.hpp", "blinkNeoPixelAsy()", "NeoPixel(Apaga)"); //ok
            NeoLedPixel(0, 0, 0, 0);
        }
        currentTime = ioBlink ? timeHigh : timeLow;
    }
}

// mensajes de error del broker mqtt
String getErrorMqttMessage(int errorCode)
{
    switch (errorCode)
    {
    case -4:
        return "Tiempo de conexión agotado - el servidor no respondió dentro del tiempo.";
    case -3:
        return "Conexión perdida - la conexion de red se rompió.";
    case -2:
        return "Fallo en la conexión - la conexion de red Falló.";
    case -1:
        return "Desconectado - el cliente se desconectó correctamente.";
    case 0:
        return "Conectado - el cliente está conectado.";
    case 1:
        return "Protocolo incorrecto - el servidor no soporta la versión solicitada de MQTT.";
    case 2:
        return "ID de cliente incorrecto - el servidor rechazó el identificador del cliente.";
    case 3:
        return "Conexión no disponible - el servidor no pudo aceptar la conexión.";
    case 4:
        return "Credenciales incorrectas - el nombre de usuario/contraseña fueron rechazados.";
    case 5:
        return "No autorizado - el cliente no está autorizado para conectarse.";
    default:
        return "Código de error desconocido.";
    }
}

// temperatura del sensor de pozo
float readSensorPozo(int pinsensor)
{
    float resistencia = 0; // resistencia sensor NTC 10K 3950
    float invTemperatura = 0;
    float temperatura = 0;
    // realiza la lectura de la resistencia del sensor NTC
    resistencia = REFERENCE_RESISTANCE / ((ADC_RESOLUTION / analogRead(pinsensor)) - 1);
    // Serial.print("Valor de la resistencia: ");
    // Serial.println(resistencia);
    //  temperatura = (B_VALUE/(log(resistencia/NOMINAL_RESISTANCE)+(B_VALUE/NOMINAL_TEMPERATURE)))-273;
    //  Convierte la resistencia a temperatura
    temperatura = (1.0 / ((log(resistencia / NOMINAL_RESISTANCE) / B_VALUE) + (1.0 / NOMINAL_TEMPERATURE))) - 273;
    // myLog("INFO", "functions.hpp", "readSensorPozo(),", "Temperatura-->");
    // Serial.print(temperatura);
    return temperatura;
}

// --------------------------------------------------------------
// Temperaturas y humedad
//  objeto DHT
DHT dht(DHTPIN, DHT22);
float Temperatura() // para la otra mandar un String
{
    if (isnan(dht.readTemperature()))
    {
        tempC = 0;
    }
    else
    {
        tempC = dht.readTemperature();
    }
    // Serial.print(tempC + ajTmpDht22);
    return (tempC + ajTmpDht22); // un ajuste de calibracion segun el criterio
}
float Humedad()
{
    if (isnan(dht.readHumidity()))
    {
        humedad = 0;
    }
    else
    {
        humedad = dht.readHumidity();
    }
    return humedad;
}
float tempMin()
{

    float min = Temperatura();
    if (min <= 1)
    {
        min2 == min2;
    }
    else if (min < min2)
    {
        min2 = min;
    }

    return min2;
}
float tempMax()
{

    float max = Temperatura();
    if (max > max2)
    {
        max2 = max;
    }
    // Serial.println(max2);
    return max2;
}

// entrega el tiempo que ha transcurrido o tiempo de funcionamiento del dispositivo
String longTimeStr(const time_t &t)
{
    int days = t / SECS_PER_DAY;
    int hours = hour(t);
    int minutes = minute(t);
    int seconds = second(t);
    String str = String(days) + ":" +
                 (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
    return str;
}

// calidad de señal
int getRSSIasQuality(const int RSSI)
{
    if (RSSI <= -100)
    {
        return 0;
    }
    else if (RSSI >= -50)
    {
        return 100;
    }
    else
    {
        return (RSSI + 100) * 2;
    }
}

// -------------------------------------------------------------------
// Fecha y Hora del Sistema
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Función para seteo de Día, Mes y Año a las variables
// -------------------------------------------------------------------
void setDyMsYr()
{
    if (cambiarfecha)
    {
        // 2022-09-07T23:47
        String str_date = nuevaFecha;
        // Serial.println(time_date);
        // Serial.println(String(str_date));
        time_sc = 0;
        time_mn = str_date.substring(14, 16).toInt(); // -2
        time_hr = str_date.substring(11, 13).toInt(); // -2
        time_dy = str_date.substring(8, 10).toInt();  //-2
        time_mt = str_date.substring(5, 7).toInt();   //-2
        time_yr = str_date.substring(0, 4).toInt();   // 2023  -2
        rtc.adjust(DateTime(time_yr, time_mt, time_dy, time_hr, time_mn, 0));
        // tm.Hour = str_date.substring(11, 13).toInt(); // lo pasan a numeros enteros
        // tm.Minute = str_date.substring(14, 16).toInt();
        // tm.Second = 0;
        // tm.Day = str_date.substring(8, 10).toInt();
        // tm.Month = str_date.substring(5, 7).toInt();
        // tm.Year = str_date.substring(0, 4).toInt();
        // tm.Year = CalendarYrToTm(Year);
        cambiarfecha = false;
    }
}
//------------------------------------------------------------------
// Setup de fecha y Hora Auto / Manual FIXME: de preferencia en un archivo aparte
//------------------------------------------------------------------
void timeSetup()
{ // verificar si esta tomando la fecha de manera automatica si no quitar la fecha automatica
    // y deja la pura manual

    setDyMsYr();

    // rtc.adjust(DateTime(time_yr, time_mt, time_dy, time_hr, time_mn, 0));
    myLog("INFO", "functions.hpp", "timeSetup()", "RTC set OK en Manual");
    // datos desde el Internet
    /*
    tm.Hour = str_date.substring(11, 13).toInt();
    tm.Minute = str_date.substring(14, 16).toInt();
    tm.Second = 0;
    tm.Day = str_date.substring(8, 10).toInt();
    tm.Month = str_date.substring(5, 7).toInt();
    tm.Year = str_date.substring(0, 4).toInt();
    */
}

String getDateTime() // antes getDateTime()
{
    if (rtcOk)
    {
        tm = rtc.now();
    }
    char fecha[20];
    int dia;
    int mes;
    long anio;
    int hora;
    int minuto;
    int segundo;

    dia = tm.day();
    // Serial.println(dia);
    mes = tm.month();
    // Serial.println(mes);
    anio = tm.year(); // no se porque pero se tiene que restar solo tm.year da 72
    // Serial.println(anio);
    hora = tm.hour();
    // Serial.println(hora);
    minuto = tm.minute();
    // Serial.println(minuto);
    segundo = tm.second();

    // sprintf( fecha, "%.2d-%.2d-%.4d %.2d:%.2d:%.2d", dia, mes, anio, hora, minuto, segundo);
    sprintf(fecha, "%.2d-%.2d-%.2d %.2d:%.2d", dia, mes, anio, hora, minuto);
    return String(fecha);
}
// FIXME:

String fechaActual()
{
    if (rtcOk)
    {
        tm = rtc.now();
    }
    char fecha[20];
    int dia;
    int mes;
    long anio;
    int hora;
    int minuto;
    int segundo;
    dia = tm.day();
    // Serial.println(dia);
    mes = tm.month();
    // Serial.println(mes);
    anio = tm.year(); // no se porque pero se tiene que restar solo tm.year da 72
    // Serial.println(anio);
    hora = tm.hour();
    // Serial.println(hora);
    minuto = tm.minute();
    // Serial.println(minuto);
    segundo = tm.second();
    // 2024-04-08T14:47:00
    sprintf(fecha, "%.4d-%.2d-%.2dT%.2d:%.2d", anio, mes, dia, hora, minuto);
    return String(fecha);
}

// -------------------------------------------------------------------
// retorna el contenido del body en formato string
String GetBodyContent(uint8_t *data, size_t len)
{
    // convierte la data que viene en un formato uint8 en caracteres de String
    return String(reinterpret_cast<char *>(data), len);
}
// validar la informacion del wifi
bool validationWifiParam(JsonDocument &jsonDoc)
{
    const char *wifiKeys[] = {
        "wifi_mode",
        "wifi_ssid",
        "wifi_password",
        "wifi_ssid2",
        "wifi_password2",
        "wifi_ip_static",
        "wifi_ipv4",
        "wifi_gateway", // invertir
        "wifi_subnet",  // invertir
        "wifi_dns_primary",
        "wifi_dns_secondary",
        "ap_ssid",
        "ap_password",
        "ap_channel", // invertido
        "ap_oculto",  // invertido
        "ap_connect"};
    JsonObject jsonObj = jsonDoc.as<JsonObject>();
    // validar que no llegue una ip no soportada
    for (JsonPair keyValue : jsonObj)
    {
        int i = 0;
        bool keyFound = false;
        for (const char *allowedKey : wifiKeys)
        {
            i++;
            if (strcmp(keyValue.key().c_str(), allowedKey) == 0)
            {
                Serial.println(wifiKeys[i]);

                keyFound = true;
                break;
            }
        }
        if (!keyFound)
        {
            Serial.println(keyValue.key().c_str());
            return false;
        }
    }
    // que las permitidas con las que tiene el doc por POST
    // Opcionales
    for (const char *key : wifiKeys)
    {
        if (jsonDoc[key].is<bool>())
        {
            return true;
        }
    }
    return false;
}

// devuelve la encriptación de la red
String EncryptionType(int encryptionType)
{
    const char *encryptionTypes[] = {
        "Open",
        "WEP",
        "WPA-PSK",
        "WPA2-PSK",
        "WPA_WPA2-PSK",
        "WPA2-ENTERPRISE",
        //"WPA3-PSK",
        //"WPA3-EAP",
        "UNKNOWN"};
    if (encryptionType >= 0 && encryptionType <= 5)
    {
        return encryptionTypes[encryptionType];
    }
    return encryptionTypes[6]; // retornar el desconocido
}

//---------------------------------------------------------------------------------------------------------------
// FIXME:
// validar parametros post mqtt
bool validationMqttParam(JsonDocument &jsonDoc)
{ // posiblemente el & es por que es un objeto
    const char *mqttKeys[] = {
        "mqtt_enabled",
        "mqtt_server",
        "mqtt_port",
        "mqtt_retain",
        "mqtt_qos",
        "mqtt_id",
        "mqtt_user",
        "mqtt_password",
        "mqtt_clean_session",
        "mqtt_time_send",
        "mqtt_time_interval",
        "mqtt_status_send",
        "mqtt_commandTopic_gerencia",
        "mqtt_commandTopic_ciudad",
        "mqtt_commandTopic_central",
        "mqtt_commandTopic_sala",
        "mqtt_IDlast4",
        "mqtt_commandTopic",
        "mqtt_sendTopic",
        "mqtt_willTopic",
        "mqtt_willMessage",
        "mqtt_willQos",
        "mqtt_willRetain"};
    JsonObject jsonObj = jsonDoc.as<JsonObject>();
    // validar que no llegue una ip no soportada
    // myLog("INFO", "functions.hpp", "valiadtionMqttParam()", "jsonObj");
    // Serial.println(jsonObj);
    // myLog("INFO", "functions.hpp", "valiadtionMqttParam()", "Pretty");
    // serializeJsonPretty(jsonDoc, Serial);
    for (JsonPair keyValue : jsonObj)
    {
        int i = 0;
        bool keyFound = false;
        for (const char *allowedKey : mqttKeys)
        {
            i++;
            if (strcmp(keyValue.key().c_str(), allowedKey) == 0)
            {
                Serial.println(mqttKeys[i]); // para comprobar cuales si estan
                // si todo esta ok la comparacion es identica
                keyFound = true;
                break;
            }
        }
        if (!keyFound)
        {
            // si algo salio mal existe una key diferente
            Serial.println("no se encontro");
            Serial.println(keyValue.key().c_str());
            return false;
        }
    }
    // que las permitidas con las que tiene el doc por POST
    // Opcionales
    for (const char *key : mqttKeys)
    {
        if (jsonDoc[key].is<bool>())
        // if (jsonDoc.containsKey(key))
        {
            myLog("INFO", "functions.hpp", "validationMqttParam()", "OK con las keys");
            return true;
        }
    }
    return false;
}

// validar los comandos
bool validateCommandJsonKeys(const String &jsonData)
{
    const char *requiredKeys[] = {
        "protocol",
        "output",
        "value"};
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonData);
    if (error)
    {
        String message = "Error al deserializar el JSON:" + String(error.c_str());
        myLog("ERROR", "functions.hpp", "validateCommandJsonKeys()", message.c_str());
        return false;
    }
    // validar las claves del JSON
    for (const char *key : requiredKeys)
    { // verifica las claves protocol,output y value
        if (!jsonDoc.containsKey(key))
        { // TODO: verificar si esto esta ok y FIXME:
            String message = "Falta la clave requerida: " + String(key);
            myLog("ERROR", "functions.hpp", "validateCommandJsonKeys()", message.c_str());
            return false;
        }
    }
    // comprobar que no hay claves no permitidas
    if (jsonDoc.size() != sizeof(requiredKeys) / sizeof(requiredKeys[0]))
    {
        myLog("Error", "functions.hpp", "if(jsonDoc.size() != sizeof(requiredKeys)/sizeof(requiredKeys[0]))", "El JSON contiene claves adicionales o no permitidas");
        return false;
    }
    return true;
}
//--------------------------------ejemplo LM35-------------------------------
float sensorLm35()
{
    // read the ADC value from the temperature sensor
    int adcVal = analogRead(LM35);
    // convert the ADC value to voltage in millivolt
    float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
    // convert the voltage to the temperature in °C
    float tempClm35 = (milliVolt / 10) + ajTmpLm35;
    // convert the °C to °F
    float tempF = tempClm35 * 9 / 5 + 32;

    // print the temperature in the Serial Monitor:
    // Serial.print("Temperature: ");
    // Serial.print(tempClm35); // print the temperature in °C
    // Serial.print("°C");
    // Serial.print("  ~  "); // separator between °C and °F
    // Serial.print(tempF);   // print the temperature in °F
    // Serial.println("°F");
    return tempClm35;
}

//--------------------------------ejemplo   LM35-------------------------------

//-------------------------------------------------------------------------------
// funciones necesarias para guarda un array de la temperaturas y humedades
// --------------------------------------------------------------------------------

// Función para mostrar los últimos valores de temperatura
void mostrarValoresTemp()
{
    Serial.printf("Valores de temperatura: ");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        Serial.printf("%d ", vTemp[i]);
    }
    Serial.printf("\n");
}
//----------------------------------------------------------------
// Función para guardar la lectura del sensor de temperatura

bool pruebaTc()
{
    int nuevaTemperatura = Temperatura();
    // Serial.println(tempC);
    if (-5 < nuevaTemperatura < 99)
    {

        // Almacenar la nueva temperatura en el array y actualizar el índice
        vTemp[0] = nuevaTemperatura;
        // mostrarValoresTemp();
        for (int i = NUM_VALORES; i >= 1; i--) // este hace el corrimiento
        {
            vTemp[i] = vTemp[i - 1];
        }
        return false;
    }
    else
    {
        return true;
    }
}
void ejecutarTc()
{
    if (pruebaTc())
    {
        int nuevaTemperatura = Temperatura();
        Serial.println(nuevaTemperatura);
        myLog("Error", "functions.hpp", "ejecutarTc()", "Error con la medición de la temperatura");
    }
}

// Función para mostrar los últimos valores de Humedad
void mostrarValoresHum()
{
    Serial.printf("Valores de Humedad: ");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        Serial.printf("%d ", vHum[i]);
    }
    Serial.printf("\n");
}
// Función para guardar la lectura del sensor de humedad

bool pruebaHum()
{
    if (10 < humedad < 99)
    {
        int nuevaHum = humedad;
        // Almacenar la nueva temperatura en el array y actualizar el índice
        vHum[0] = nuevaHum;
        // mostrarValoresHum();
        for (int i = NUM_VALORES; i >= 1; i--) // este hace el corrimiento
        {
            vHum[i] = vHum[i - 1];
        }
        return false;
    }
    else
    {
        return true;
    }
}
void ejecutarHum()
{

    if (pruebaHum())
    {
        Serial.println(humedad);
        myLog("Error", "functions.hpp", "ejecutarHum()", "Error con la medición de la humedad");
    }
}
//-------------------------------------------------------------------------------
// funciones necesarias para guarda un array de la temperatura pozo1
// --------------------------------------------------------------------------------

// Función para mostrar los últimos valores de temperatura
void mostrarValoresTmp1()
{
    Serial.printf("Valores de temperatura del pozo1: ");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        Serial.printf("%d ", vSensorPozo1[i]);
    }
    Serial.printf("\n");
}
//----------------------------------------------------------------
// Función para guardar la lectura del sensor de temperatura

bool pruebaTmp1()
{
    // Serial.println(temp1);
    if (0 < temp1 < 99)
    {
        int nuevaTemperatura = temp1;
        // Almacenar la nueva temperatura en el array y actualizar el índice
        vSensorPozo1[0] = nuevaTemperatura;
        // mostrarValoresTmp1();
        for (int i = NUM_VALORES; i >= 1; i--) // este hace el corrimiento
        {
            vSensorPozo1[i] = vSensorPozo1[i - 1];
        }
        return false;
    }
    else
    {
        myLog("ERROR", "functions.hpp", "pruebaTmp1()", "sensor de pozo 1 fuera de Rango");
        return true;
    }
}
void ejecutarTmp1()
{
    if (pruebaTmp1())
    {
        Serial.println(temp1);
        myLog("Error", "functions.hpp", "ejecutarTmp1()", "Error con la medición de la temperatura");
    }
}
//-------------------------------------------------------------------------------
// funciones necesarias para guarda un array de la temperatura pozo2
// --------------------------------------------------------------------------------

// Función para mostrar los últimos valores de temperatura
void mostrarValoresTmp2()
{
    Serial.printf("Valores de temperatura del pozo2: ");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        Serial.printf("%d ", vSensorPozo2[i]);
    }
    Serial.printf("\n");
}
//----------------------------------------------------------------
// Función para guardar la lectura del sensor de temperatura

bool pruebaTmp2()
{
    // Serial.println(temp2);
    if (0 < temp2 < 99)
    {
        int nuevaTemperatura = temp2;
        // Almacenar la nueva temperatura en el array y actualizar el índice
        vSensorPozo2[0] = nuevaTemperatura;
        // mostrarValoresTmp2();
        for (int i = NUM_VALORES; i >= 1; i--) // este hace el corrimiento
        {
            vSensorPozo2[i] = vSensorPozo2[i - 1];
        }
        return false;
    }
    else
    {
        myLog("ERROR", "functions.hpp", "pruebaTmp1()", "sensor de pozo 2 fuera de Rango");
        return true;
    }
}
void ejecutarTmp2()
{
    if (pruebaTmp2())
    {
        Serial.println(temp2);
        myLog("Error", "functions.hpp", "ejecutarTmp2()", "Error con la medición de la temperatura");
    }
}
//-------------------------------------------------------------------------------
// funciones necesarias para guarda un array de la temperatura del LM35
// --------------------------------------------------------------------------------

// Función para mostrar los últimos valores de temperatura
void mostrarValoresLm35()
{
    Serial.printf("Valores de temperatura del Lm35: ");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        Serial.printf("%d ", vSLm35[i]);
    }
    Serial.printf("\n");
}
//----------------------------------------------------------------
// Función para guardar la lectura del sensor de temperatura

bool pruebaLm35()
{
    int nuevaTemperatura = sensorLm35();
    // Serial.println(temp2);
    if (-50 < nuevaTemperatura < 150)
    {
        // Almacenar la nueva temperatura en el array y actualizar el índice
        vSLm35[0] = nuevaTemperatura;
        // mostrarValoresLm35();
        for (int i = NUM_VALORES; i >= 1; i--) // este hace el corrimiento
        {
            vSLm35[i] = vSLm35[i - 1];
        }
        return false;
    }
    else
    {
        myLog("ERROR", "functions.hpp", "pruebaTmp1()", "sensor LM35 fuera de Rango");
        return true;
    }
}
void ejecutarLm35()
{
    if (pruebaLm35())
    {
        Serial.println(templm35);
        myLog("Error", "functions.hpp", "ejecutarLm35()", "Error con la medición de la temperatura");
    }
}

//----------------------------------------------------------------------------------------------
// muestreo de temperatura y humedad
//----------------------------------------------------------------------------------------------
void muestra() // esta funcion toma una muestra de la temperatura y la humedad
{
    if (conteografica >= tgrafica)
    {
        conteografica = 0;
        ejecutarTc();
        ejecutarHum();
        ejecutarTmp1();
        ejecutarTmp2();
        ejecutarLm35();
        if ((2 < tempC < 99) && (2 < humedad < 99) && (0 < temp1 < 99) && (0 < temp2 < 99) && (-50 < templm35 < 150))
        {
            dataGraficasSave(); //
        }
    }
    else
    {
        conteografica++;
    }
}

void setupPinBtnes()
{
    pinMode(ACTRELE1, INPUT_PULLUP); // GPIO 46
    pinMode(ACTRELE2, INPUT_PULLUP); // GPIO 47
    pinMode(ACTFECHA, INPUT_PULLUP); // GPIO 20 seteo de la fecha
}
//------------------------------INTERRUPCIONES-------------------------------------------------
// Funcion para activar y desactivar los relevadores de manera local
//-------------------------------------------------------------------------------
void actRele()
{
    //-----------------------------------------------------------
    if (!digitalRead(RELAY1) && !digitalRead(ACTRELE1) && togle1) //! digitalRead(RELAY1) verifica el estado del relay
    {
        togle1 = !togle1;
        digitalWrite(RELAY1, true);
        digitalWrite(TMOSFET1, true); // TODO: quitar
        Serial.println("activar relay1 ");
        RELAY1_STATUS = true;
    }
    else if (digitalRead(RELAY1) && !digitalRead(ACTRELE1) && togle1) // digitalRead(RELAY1) verifica el estado del realy
    {
        togle1 = !togle1;
        digitalWrite(RELAY1, false);
        digitalWrite(TMOSFET1, false); // TODO: quitar
        Serial.println("desactivar relay1 ");
        RELAY1_STATUS = false;
    }
    else
    {

        togle1 = true;
    }
    //-----------------------------------------------------------
    if (!digitalRead(RELAY2) && !digitalRead(ACTRELE2) && togle2)
    {
        togle2 = !togle2;
        digitalWrite(RELAY2, true);
        digitalWrite(TMOSFET2, true); // TODO: check
        Serial.println("activar relay2 ");
        RELAY2_STATUS = true;
    }
    else if (digitalRead(RELAY2) && !digitalRead(ACTRELE2) && togle2)
    {
        togle2 = !togle2;
        digitalWrite(RELAY2, false);
        digitalWrite(TMOSFET2, false); // TODO:
        Serial.println("desactivar relay2 ");
        RELAY2_STATUS = false;
    }
    else
    {

        togle2 = true;
    }
}

//----------------------------------------------------------------
// funcion que indica que hora es con esta podemos compara la hr y realizar la accion
//----------------------------------------------------------------
String releTime()
{

    char horas[5];
    int hora = 0;
    int minuto = 0;

    hora = tm.hour();
    minuto = tm.minute();

    // sprintf( hora, "%.2d-%.2d-%.4d %.2d:%.2d:%.2d", dia, mes, anio, hora, minuto, segundo);
    sprintf(horas, "%.2d:%.2d", hora, minuto);
    return String(horas);
}
//----------------------------------------------------------------
// funcion que indica cuando deben encender y que horario los relays
//----------------------------------------------------------------
void programaRelays() // TODO:
{
    int weekDay = tm.dayOfTheWeek();
    // 1A
    if (DOMINGO1A && weekDay == 0)
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (LUNES1A && (weekDay == 1))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (MARTES1A && (weekDay == 2))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (MIERCOLES1A && (weekDay == 3))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (JUEVES1A && (weekDay == 2))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (VIERNES1A && (weekDay == 2))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (SABADO1A && (weekDay == 2))
    {
        if (TIMEONRELAY1A == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1A == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    // 1B
    if (DOMINGO1B && weekDay == 0)
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (LUNES1B && (weekDay == 1))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (MARTES1B && (weekDay == 2))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (MIERCOLES1B && (weekDay == 3))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (JUEVES1B && (weekDay == 2))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (VIERNES1B && (weekDay == 2))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    if (SABADO1B && (weekDay == 2))
    {
        if (TIMEONRELAY1B == releTime())
        {
            digitalWrite(RELAY1, HIGH);
            // digitalWrite(TMOSFET1, HIGH); // TODO: check
            RELAY1_STATUS = true;
        }
        if (TIMEOFFRELAY1B == releTime())
        {
            digitalWrite(RELAY1, LOW);
            // digitalWrite(TMOSFET1, LOW); // TODO: check
            RELAY1_STATUS = false;
        }
    }
    // 2A
    if (DOMINGO2A && weekDay == 0)
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (LUNES2A && (weekDay == 1))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (MARTES2A && (weekDay == 2))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (MIERCOLES2A && (weekDay == 3))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (JUEVES2A && (weekDay == 2))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (VIERNES2A && (weekDay == 2))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (SABADO2A && (weekDay == 2))
    {
        if (TIMEONRELAY2A == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2A == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    // 2B
    if (DOMINGO2B && weekDay == 0)
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (LUNES2B && (weekDay == 1))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (MARTES2B && (weekDay == 2))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (MIERCOLES2B && (weekDay == 3))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (JUEVES2B && (weekDay == 2))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (VIERNES2B && (weekDay == 2))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
    if (SABADO2B && (weekDay == 2))
    {
        if (TIMEONRELAY2B == releTime())
        {
            digitalWrite(RELAY2, HIGH);
            // digitalWrite(TMOSFET2, HIGH); // TODO: check
            RELAY2_STATUS = true;
        }
        if (TIMEOFFRELAY2B == releTime())
        {
            digitalWrite(RELAY2, LOW);
            // digitalWrite(TMOSFET2, LOW); // TODO: check
            RELAY2_STATUS = false;
        }
    }
}

// funciones para WebSocket devuelve un json
String getSendJson(String type, String msg)
{
    String response = "";
    JsonDocument doc;
    doc["type"] = type;
    doc["msg"] = msg;
    serializeJson(doc, response);
    return response;
}