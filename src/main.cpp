// Definiciones necesarias
#include <Arduino.h>           // para que entienda codigo
#include <Adafruit_NeoPixel.h> // utilizar el NEOPIXEL
#include <EEPROM.h>            // para manejar la memoria EEPROM del esp32 (no volatil)
#include <SPIFFS.h>            // para manejos de archivos que se almacenas en la memoria no volatil
#include <ESPmDNS.h>           // para direcciones ip con nombre
#include <ArduinoJson.h>       // manejo de archivos JSON para las variables
#include <TimeLib.h>           // para las fechas
#include <RTClib.h>            // para el reloj en tiempo real
#include <Wire.h>              // Necesaria para el reloj
// Libreria para el RTC del ESP32
#include <ESP32Time.h>         //ayuda a calcular la fecha
#include <PubSubClient.h>      // para el MQTT
#include <ESPAsyncWebServer.h> // para el servidor dentro del esp32
#include <WiFi.h>              // para conectarse a un red Wifi
#include <WiFiMulti.h>
#include <ESPmDNS.h>   // para DNS
#include <DNSServer.h> // para DNS

/*Librerias agregadas para nuevas funcionalidades y obligatorias (LALO)*/
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>    //libreria para el sensor bme280
#include <DHT.h>                // para la temperatura y humedad by Adafruit
#include <Adafruit_I2CDevice.h> //Para la comunicaciones serial by Adafruit
#include <Adafruit_GFX.h>       // para la pantalla de lcd de gráficos (cuadrados circulos toda una especializacion)
#include <Adafruit_SSD1306.h>   // para la pantalla de lcd by Adafruit
#include <Ticker.h>             //para los timer de los relays

// Archivos *.hpp Fragmentación del codigo
#include "definiciones.hpp"
#include "var_entorno.hpp"
#include "functions.hpp"
#include "settings.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
#include "spiffsGraficas.hpp" //archivo donde se guardan en json los datos de muestreo
#include "spiffsTime.hpp"     //archiva dond se guarda la fecha de muestreo
#include "spiffsBitacora.hpp" //archiva donde se guarda la informacion de la bitácora
// metemos la libreria del servidor
#include "server.hpp"
#include "websockets.hpp"
#include "tareas.hpp"
#include "reloj.hpp" //tiempo
#include "alarmas.hpp"
#include "setupvar.hpp"
#include "reset.hpp"
#include "telegram.hpp"
#include "whatsapp.hpp"
#include "sensorbme280.hpp"

void setup()
{
  // LED NEOPIXEL
  NeoPixel.begin(); // inicializacion del neopixel
  NeoPixel.show();  // apagar el NeoPixel ya que no hay valores asignados para mostrar
  // Monitor Serial
  Serial.begin(115200);
  myLog("INFO", "main.cpp", "setup()", "Iniciando..."); // para que el programa no avance hasta que termine de transmitir los datos que se encuentran en el buffer
  Serial.flush();
  if (!SPIFFS.begin(true))
  {
    // esto indica que si no logra inicializar el SPIFFS continuara este buclu hasta que el resultado sea un true
    myLog("ERROR", "main.cpp", "SPIFFS.begin(true)", "Falla al inicializar SPIFSS"); // para que el programa no avance hasta que termine de transmitir los datos que se encuentran en el buffer
    while (true)
      ;
  }
  // contador de reinicios
  EEPROM.begin(256); // bytes
  EEPROM.get(Restart_Address, device_restart);
  device_restart++;

  // verificar que no exeda de 65535 en ese caso se iguala a 0
  if (device_restart > 65535)
  {
    device_restart = 0;
  }
  EEPROM.put(Restart_Address, device_restart);
  EEPROM.commit(); // guardamos la información
  EEPROM.end();
  String message = "Cantidad de reinicios: " + String(device_restart);

  myLog("INFO", "main.cpp", "EEPROM", message.c_str());
  // leer las configuraciones del settings.json
  if (!settingsRead())
  { // al negarlo se vuelve un verdadero
    settingsSave();
  }
  if (!timeRead()) // para las graficas
  {                // al negarlo se vuelve un verdadero
    timeSave();
  }
  // Data Graficas
  //  traer las datas resientes desde json
  if (!dataGraficasRead()) // si no hay
  {                        // se crea el archivo con los valores iniciales de reset
    dataGraficasSave();    // aqui guardamos
    myLog("INFO", "main.hpp", "!dataGraficasRead()", "Información general Salvada.");
    // encaso que el dataRead se a podido leer no se guarda nada para no andar tocando la memoria
  }
  // data alarmas
  myLog("INFO", "alarmas.hpp", "alarmaPresente()", "alarma presente verificando archivo de alarmas.json");
  // comunicacion I2C https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
  Wire.begin(RELOJSDA, RELOJSCL); // para declara cualquier pin I2C SDA = DATOS, SCL = RELOJ
  relojSetup();                   // conectar el temposizador
  // definir los pines de entrada y salida
  setupPinAlarmas();  // pines de alarmas entrada
  setupPinBtnes();    // botones de entrada
  setupPintRestore(); // funcion para el pin de reset y restaurar
  // boton de interrupcion
  // setupPinActivarAlarmas();//se declara un boton de interrupcion
  gpioDefineOut(); // pines de salida
  // despues de definir los pines ya podemor configurar las salidas segun lo guardado en settings.json
  setOnOffSingle(RELAY1, RELAY1_STATUS);
  setOnOffSingle(RELAY2, RELAY1_STATUS);
  setOnOffSingle(BUZZER, LOW); // Este valor no se guarda en settings
  // archivo de registro de alarmas del dispositivo
  if (!exiteArchivoAlarmas()) // pregunta si existe un archivo de alarmas?, si no, lo crea
  {                           // en este punto no el archivo ya existe se procede a salvarlo
    myLog("INFO", "main.cpp", "exiteArchivoAlarmas()", "archivo alarmas.json no existe creando archivo ...");
    if (crearArchivoAlarmas())
    {
      myLog("INFO", "main.hpp", "exiteArchivoAlarmas()", "archivo alarmas.json creado");
    }
    else
    {
      myLog("ERROR", "main.cpp", "crearArchivoAlarmas()", "Falla al crear el archivo alarmas.json");
    }
  }
  if (!existeBitacora()) // pregunta si existe un archivo de bitacora?, si no, lo crea
  {                      // en este punto no el archivo ya existe se procede a salvarlo
    myLog("INFO", "main.cpp", "existeBitacora()", "archivo bitacora.json no existe creando archivo ...");
    if (crearBitacora())
    {
      myLog("INFO", "main.hpp", "crearBitacora()", "archivo bitacora.json creado");
    }
    else
    {
      myLog("ERROR", "main.cpp", "crearBitacora()", "Falla al crear el archivo bitacora.json");
    }
  }
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // 0x3C alimentación a 3.3 volts
  {                                            // Dirección 0x3C
    myLog("INFO", "main.cpp", "OLED.begin()", "OLED no encontrado");
    // while(true);
  }
  else
  {
    myLog("INFO", "main.cpp", "OLED.begin()", "OLED OK");
    OLED.clearDisplay();
  }
  if (!setupBME280())
  {
    Serial.println("No se pudo encontrar un sensor BME280 válido, compruebe la conexión.");
    Serial.println("inicializando con dht.");
    dht.begin(); // su funcionalidad se encuentra en functions.hpp
    sensorTempBME280 = false;
  }
  else
  {
    sensorTempBME280 = true;
  }
  // dht.begin(); // su funcionalidad se encuentra en functions.hpp

  // PWM
  ledcWrite(ledChannel, dim * 2.55); // 8 bits 255 dim es = 0-100 para el dimer
  // iniciar el setup del wiffi
  wifiSetup();
  // setearemos algunas variables que solo se ejecutan una sola vez para no solicitar mas informacion despues la intencion es liberar memoria
  setupVar();
  // iniciar el setup del servidor WEB
  initServer();
  // inicializar el WebSocket
  initWebSockets();
  // llamar a la actualización del proceso del firmware -enviar por ws y sacar por serial
  Update.onProgress(printFirmwareProgress);
  // tarea que muestra en el LCD informacion
  TaskHandle_t lcdTaskHandle;
  xTaskCreatePinnedToCore(TaskLCD, "TaskLCD", 1024 * 4, NULL, 1, &lcdTaskHandle, 0); // tarea no ten importante min 4 megas
  // crear tarea para la reconexión del WIFI
  TaskHandle_t wifiTaskHandle;
  xTaskCreatePinnedToCore(TaskWifiReconnect, "TaskWifiReconnect", 1024 * 10, NULL, 1, &wifiTaskHandle, 1);
  // Crear tarea para la reconexión de MQTT
  TaskHandle_t mqttTaskHandle;
  xTaskCreatePinnedToCore(TaskMQTTReconnect, "TaskMQTTReconnect", 1024 * 10, NULL, 1, &mqttTaskHandle, 1); // tareas importantes core1
  // pestañeo del LED MQTT (AZUL)
  TaskHandle_t LedMqttTaskHandle;
  xTaskCreatePinnedToCore(TaskLedMqttReconnect, "TaskLedMqttReconnect", 1024 * 3, NULL, 1, &LedMqttTaskHandle, 0); // tarea no tan importante, paso al core 0
  // tarea que actualiza varias variables cada 1 segundo
  TaskHandle_t Actualiza1segTaskHandle;
  xTaskCreatePinnedToCore(TaskActualiza1seg, "TaskActualiza1seg", 1024 * 10, NULL, 1, &Actualiza1segTaskHandle, 1);
  // tarea que actualiza varias variables cada 2 segundo para que funcione correctamente el DHT22
  TaskHandle_t Actualiza2segTaskHandle;
  xTaskCreatePinnedToCore(TaskActualiza2seg, "TaskActualiza2seg", 1024 * 12, NULL, 1, &Actualiza2segTaskHandle, 1); // FIXME: tarea en el core 1 para ver si es la del problema
  // tarea que actualiza la variable cada 2 segundo de humedad para que funcione correctamente el DHT22
  // TaskHandle_t Actualiza2segHumTaskHandle;
  // xTaskCreatePinnedToCore(TaskActualiza2Humseg, "TaskActualiza2seg", 1024 * 12, NULL, 1, &Actualiza2segHumTaskHandle, 0); // FIXME: tarea en el core 1 para ver si es la del problema
  //  tarea que actualiza varias variables cada 10 segundo
  TaskHandle_t Actualiza10segTaskHandle;
  xTaskCreatePinnedToCore(TaskActualiza10seg, "TaskActualiza10seg", 1024 * 8, NULL, 1, &Actualiza10segTaskHandle, 1);
  // tarea que toma la muestras para la grafica
  TaskHandle_t timeGraficaTaskHandle;
  xTaskCreatePinnedToCore(TaskTimeGrafica, "TaskTimeGrafica", 1024 * 12, NULL, 1, &timeGraficaTaskHandle, 1);
  // Crear una tarea verifique el boton d15 si se requiere una restauracion de fabrica o restart
  TaskHandle_t taskRestoreTaskHandle;
  xTaskCreatePinnedToCore(TaskRestore, "TaskRestore", 1024 * 8, NULL, 1, &taskRestoreTaskHandle, 1); // no creo que sea esta
  // WebSocket envio de mensajes
  TaskHandle_t wsTaskHandle;
  xTaskCreatePinnedToCore(TaskWsSend, "TaskWsSend", 1024 * 8, NULL, 1, &wsTaskHandle, 0); // 0 el core donde estara trabajando
}
void loop()
{

  // contadorAlarmas(); // cuenta la cantidad de alarmas y le pone la fecha
}
