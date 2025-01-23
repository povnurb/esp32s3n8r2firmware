// PINOUT
/*
No usar los gpio 26,27,28,29,30,31,32 pines conectados a la memoria flash spi
usaremos los GPIO 08(SDA) y 09(SCL) para el display OLED
*/

// boton de interrupción para la activacion de alarmas
#define tiempoDeRebote 1500 // sirve para omitir rebotes

//----------------------------------------------------------------
// para el sensor de pozo
// https://www.instructables.com/Temperatura-con-Termistor-NTC-10k-y-Arduino/

#define REFERENCE_RESISTANCE 10000.0
#define NOMINAL_RESISTANCE 10000.0
#define NOMINAL_TEMPERATURE 298.00
#define B_VALUE 3950.0
#define ADC_RESOLUTION 4096.0
// cantidades--------------------------------
#define NUM_RELAYS 2 // numero de relays que tiene la placa
#define NUM_ALARMS 8 // numero de alarmas que tiene la placa
// otro sensor de temperatura
// https://www.electrontools.com/Home/WP/sensor-de-temperatura-ds1820/
// para el sensor LM35 que es mi caso
// https://esp32io.com/tutorials/esp32-lm35-temperature-sensor
// https:// www.makerguides.com/interfacing-esp32-and-lm35-temperature-sensor/
#define ADC_VREF_mV 3300.0 // in millivolt podria utilizar un ajuste con un potenciometro
#define ADC_RESOLUTION 4096.0
#define LM35 1 //  sensor de temperatura de 0°C a 150°C
// pines------------------------------------------
#define BTNMENU 45          // TODO:boton que dara acceso a mostrar mas informacion
#define BUZZER 21           // GPIO 21 ZUMBADOR
#define TMOSFET1 10         // TODO: salidas quedan pendientes para despues ver que hacer
#define TMOSFET2 11         // TODO: salidas
#define DHTPIN 2           // GPIO 12 entrada sensor dht22 (temperatura y humedad)
#define BTNRST 15           // boton que reseteara o restaurar el dispositivo
#define BTN_ENABLE 16       // TODO:boton que habilita o desabilita alguna función
#define SWITCHTESTALARM 13  // SWITCH de prueba de las alarmas en local se activa con una tierra
#define RELAY1 17           // GPIO 17 Relevador1
#define ACTRELE1 47         // boton de prueba del relay1
#define LEDNEOPIXEL 48      // neopixel
#define RELAY2 18           // GPIO 18 Relevador2
#define ACTRELE2 46         // boton de prueba del relay2
#define DIMMER 19           // led DIMMER
#define RELOJSDA 4          // pala el RTC y OLED
#define RELOJSCL 5          // para el RTC y OLED
#define SENSOREVAPORADOR 6  // pin analogo
#define SENSORCONDENSADOR 7 // pin analogo
#define ACTFECHA 20         // pin para actualizar la fecha
// el WIFI led es el 48 VERDE
// el MQTT led es el 48 AZUL
// el DIMER led es el 48 ROJO
#define ALARMPIN1 35 // Entrada de la alarma1
#define ALARMPIN2 36 // Entrada de la alarma2
#define ALARMPIN3 37 // Entrada de la alarma3
#define ALARMPIN4 38 // Entrada de la alarma4
#define ALARMPIN5 39 // Entrada de la alarma5
#define ALARMPIN6 40 // Entrada de la alarma6
#define ALARMPIN7 41 // Entrada de la alarma7
#define ALARMPIN8 42 // Entrada de la alarma8
//-----------------------------------------------------------------------------------------------
// VERSION DE FIRMWARE (Por documentación) se toman de las variables de entorno de platformio.ini
//-----------------------------------------------------------------------------------------------
#define TEXTTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTTIFY(A)
String device_fw_version = ESCAPEQUOTE(BUILD_TAG);

//----------------------------------------------------
// Zons firmware
//------------------------------------------------------
size_t content_len;
#define U_PART U_SPIFFS // para actualizacion del firmare via remota
//-----------------------------------------------------------------------
// HARDWARE AND MANUFACTURE Y FABRICANTE
//-----------------------------------------------------------------------
#define device_hw_version "ADMINESP32 v1 0000" // version del Hardware
#define device_manufacture "IOTMX"             // Fabricante

//-----------------------------------------------------------------------------
// ZONA DE CONFIGURACION DEL DISPOSITIVO (variables globales)
//-----------------------------------------------------------------------------
char device_id[30];       // número identificador
char device_name[30];     // nombre del dispositivo ejemplo: es el lugar HGO3PISO
char device_user[16];     // usuario web
char device_password[16]; // para logearse en el servidor web
u_int16_t device_restart; // numero entero positivo que va hasta 65535
boolean modoSta;
//-------------------------------------------------------------------
// Zona configuración WIFI en modo Cliente (Variables)
//-------------------------------------------------------------------
int wifi_app = WIFI_AP;
boolean wifi_mode;       // true = cliente de un modem, false = ap(accespoint)
char wifi_ssid[63];      // nombre de modem
char wifi_ssid2[63];     // nombre de modem2
char wifi_password[63];  // su password
char wifi_password2[63]; // su password
boolean wifi_ip_static;  // ip fijo si es false o es asignada una ip automaticamente si es true
char wifi_ipv4[16];
char wifi_gateway[16];
char wifi_subnet[16];
char wifi_dns_primary[16];
char wifi_dns_secondary[16];

//------------------------------------------------------------
// Zona configuración wifi AP(AccesPOINT)
//-----------------------------------------------------------
char ap_ssid[63];     // Nombre del SSID modo AP máximo 63
char ap_password[63]; // Contraseña del modo AP mínimo 8 NULL red abierta
int ap_channel;       // 1-13 el canal de la red wifi
bool ap_oculto;       // 0 visible y 1 oculto por documentación FIXME: parece existir un error verificar es enrealidad oculto
int ap_connect;       // 0 - 8 numero de clientes

//---------------------------------------------------------------
// Zona configuración MQTT
//---------------------------------------------------------------
boolean mqtt_enabled;
char mqtt_server[39];
int mqtt_port;
boolean mqtt_retain;
int mqtt_qos;
char mqtt_id[63];
char mqtt_user[63];
char mqtt_password[63];
boolean mqtt_clean_session;
char mqtt_commandTopic[150];
char mqtt_sendTopic[150];
char mqtt_willTopic[150];
char mqtt_willMessage[150];
int mqtt_willQos;
boolean mqtt_willRetain;
boolean mqtt_time_send;
int mqtt_time_interval;
boolean mqtt_status_send;

//---------------------------------------------------
// Zona EEPROM para contador de reinicios por documentación
//----------------------------------------------------
#define Start_Address 0
#define Restart_Address Start_Address + sizeof(int)

//----------------------------------------------------
// otras variables globales a utilizar
//----------------------------------------------------
float tempPozoC, tempPozof; // centigrados y fahrenheit

//---------------------------------------------------
// Zona Relay originales
//-------------------------------------------------
bool RELAY1_STATUS;   // estado del relay1
bool CRELAY1 = false; // para que conmute en las fechas
bool RELAY2_STATUS;   // estado del relay2
bool CRELAY2 = false; // para que conmute en las fechas
//------------------------------------------------
// Zona de la grafica
// Variables necesarias para registrar temperaturas y humedades por 48 hrs
//------------------------------------------------------------------------------
#define NUM_VALORES 24 // es el numero de muestras que quiero registrar esta no se mueve siempre sera 24 por que asi esta en la pagina web
int conteografica = 1; // para que lleve el conteo del tiempo
int maxTimeMin = 60;   // maximo tiempo en minutos que toma una muestra de los valores de temp y humedad
int tgrafica;          // tiempo en minutos que indica cada cuantos minutos toma una muestra y guardar en minutos
// FIXME: parece que esta invadiendo espacio de memoria
char vTime[NUM_VALORES][7];    // Array para almacenar los valores de fecha con hasta 10 caracteres para que guarde espacio
int vTemp[NUM_VALORES];        // Array para almacenar los valores de temperatura
int vHum[NUM_VALORES];         // Array para almacenar los valores de humedad
int vSensorPozo1[NUM_VALORES]; // Array para almacenar los valores de temperatura del pozo1
int vSensorPozo2[NUM_VALORES]; // Array para almacenar los valores de temperatura del pozo2
int vSLm35[NUM_VALORES];       // Array para almacenar los valores de temperatura del pozo2
//-------------------------------------------------
// zona PWM
//---------------------------------------------------
const int freq = 1000;    // frecuencia de trabajo de 1khz
const int ledChannel = 0; // Definicion del Canal
const int resolution = 8; // 8 bits lo cual corresponde a 255 bit de resolución
int dim;                  // un valor que va de 0 a 255 a enviar

// NeoPixel
// primer parámetro = cantidad de pixeles en la tira
// Segundo parámetro = pin de conexión a Arduino en nuestro caso el pin 48
// tercer parámetro es la suma de: NEO_GRB + NEO_KHZ800
// Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(# de NEOPIXEL, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(1, LEDNEOPIXEL, NEO_GRB + NEO_KHZ800);
// put function declarations here:

// variables para las temperaturas y humedades
float humedad, temp1, temp2, templm35, tempC, tempCMasAjuste; // Humedad dht22, temperatura pozo1, temperatura pozo2 , temperatura lm35,Temperatura del DHT22
float min2 = 100;
float max2;
// ajustes minimos de sensores de temperatura
float ajTmpDht22;
float ajTmpP1;
float ajTmpP2;
float ajTmpLm35;

// ---------------------------------------------------
// Zona de alarmas
// ---------------------------------------------------
bool ALARM_STATUS1, ALARM_STATUS2, ALARM_STATUS3, ALARM_STATUS4, ALARM_STATUS5, ALARM_STATUS6, ALARM_STATUS7, ALARM_STATUS8;
// aqui hay que cambiar a false o quitar el valor
bool ALARM_RECONOCIDA1, ALARM_RECONOCIDA2, ALARM_RECONOCIDA3, ALARM_RECONOCIDA4, ALARM_RECONOCIDA5, ALARM_RECONOCIDA6, ALARM_RECONOCIDA7, ALARM_RECONOCIDA8;
bool ALARM_STATUS[NUM_ALARMS];
bool reconocidas[NUM_ALARMS] = {ALARM_RECONOCIDA1, ALARM_RECONOCIDA2, ALARM_RECONOCIDA3, ALARM_RECONOCIDA4, ALARM_RECONOCIDA5, ALARM_RECONOCIDA6, ALARM_RECONOCIDA7, ALARM_RECONOCIDA8};
//  guardar la logica, el pin y el nombre en el settings
bool ALARM_LOGICA1, ALARM_LOGICA2, ALARM_LOGICA3, ALARM_LOGICA4, ALARM_LOGICA5, ALARM_LOGICA6, ALARM_LOGICA7, ALARM_LOGICA8;
bool logicas[NUM_ALARMS];
String ALARM_NAME1, ALARM_NAME2, ALARM_NAME3, ALARM_NAME4, ALARM_NAME5, ALARM_NAME6, ALARM_NAME7, ALARM_NAME8;
String ALARM_NAME[NUM_ALARMS] = {ALARM_NAME1, ALARM_NAME2, ALARM_NAME3, ALARM_NAME4, ALARM_NAME5, ALARM_NAME6, ALARM_NAME7, ALARM_NAME8};
String ALARM_TIMEON1, ALARM_TIMEON2, ALARM_TIMEON3, ALARM_TIMEON4, ALARM_TIMEON5, ALARM_TIMEON6, ALARM_TIMEON7, ALARM_TIMEON8;
String ALARM_TIMEOFF1, ALARM_TIMEOFF2, ALARM_TIMEOFF3, ALARM_TIMEOFF4, ALARM_TIMEOFF5, ALARM_TIMEOFF6, ALARM_TIMEOFF7, ALARM_TIMEOFF8;
String fechaAct[NUM_ALARMS] = {ALARM_TIMEON1, ALARM_TIMEON2, ALARM_TIMEON3, ALARM_TIMEON4, ALARM_TIMEON5, ALARM_TIMEON6, ALARM_TIMEON7, ALARM_TIMEON8};
String fechaClear[NUM_ALARMS] = {ALARM_TIMEOFF1, ALARM_TIMEOFF2, ALARM_TIMEOFF3, ALARM_TIMEOFF4, ALARM_TIMEOFF5, ALARM_TIMEOFF6, ALARM_TIMEOFF7, ALARM_TIMEOFF8};
int ALARM_CONT1, ALARM_CONT2, ALARM_CONT3, ALARM_CONT4, ALARM_CONT5, ALARM_CONT6, ALARM_CONT7, ALARM_CONT8; // registra el valor del contador de alarmas
// contador de alarmas indica cuantas veces se a producido esa alarma
int cont[NUM_ALARMS] = {ALARM_CONT1, ALARM_CONT2, ALARM_CONT3, ALARM_CONT4, ALARM_CONT5, ALARM_CONT6, ALARM_CONT7, ALARM_CONT8}; // lleva la contavilizacion en un for
bool cambiar[NUM_ALARMS] = {false, false, false, false, false, false, false, false};

//----------------------------------------------------
//   Zona time
//----------------------------------------------------
RTC_DS1307 rtc; // Clase RTC_DS1307
DateTime tm;    // objeto de tiempo para el modulo RTC DS1307
bool rtcOk;     // indica si existe dispositivo time
int Hour, Minute, Second, Day, Month, Year;

// bool time_ajuste;   // 1- Manual , 0- Automático internet SRV NTP
char time_date[18]; // 2022-09-07T23:47
// char time_dateSalvado[18]; // 2022-09-07T23:47 para usar en spiffsDate.hpp
// long time_z_horaria;  // Zona Horaria GMT 0 = 0 -GMT +1 = 3600 - GMT -1 = -3600
// char time_server[39]; // Servidor NTP Elemplo: time.mist.gov

int time_hr; // Hora 0 -23
int time_mn; // Minutos 0 - 59
int time_sc; // Segundos 0-59
int time_dy; // Días 1 - 31
int time_mt; // Meses 1- 12
int time_yr; // Año 2023
String hora;

// declaracion de algunas variables que no se podran modificar
String UltimaConexion;
String FechaMsgMqtt; // no modificables
String WifiSsidMqtt;
String IpMqtt;
String deviceSerial;
String deviceManofactura;
String hwVersion;
String sdkVersion;
// declaracion de algunas variables que se modifican cada 1 segundo en automatico
// para envio por MQTT
int device_dimmer;
String device_lm35;
String device_tempEvaporador;
String device_tempCondensador;
String device_tempDHT;
String device_humedadDHT;
String device_tempMinima;
String device_tempMaxima;
String device_ativeTime;
int device_rssi;
int device_quality;
//------------------------------------------------------------------------------
// Zona RELAY agregada
//------------------------------------------------------------------------------

bool device_relay01;   // estado del relay1
char R_NAME1[20];      // nombre de los Relays
bool R_TIMERON1;       // indica si se activa el timer del relevador1
int R_TIMER1;          // contador por segundo
String R_DESCRIPTION1; // indica que es lo que controla
bool TEMPORIZADOR1;    // indica si hay un control por tiempo
String TIMEONRELAY1A;  // indica a que horas se prende
String TIMEOFFRELAY1A; // indica a que horas se apaga
String TIMEONRELAY1B;  // indica a que horas se prende
String TIMEOFFRELAY1B; // indica a que horas se apaga
String FECHAON1 = "";  // muestra la FECHA que se encendio
String FECHAOFF1 = ""; // muestra la FECHA que se apago
bool programado1;      // muestra si esta programado el relay1 con fecha
bool LUNES1A, MARTES1A, MIERCOLES1A, JUEVES1A, VIERNES1A, SABADO1A, DOMINGO1A;
bool LUNES1B, MARTES1B, MIERCOLES1B, JUEVES1B, VIERNES1B, SABADO1B, DOMINGO1B;

bool device_relay02;   // estado del relay2
char R_NAME2[20];      // nombre de los Relays
bool R_TIMERON2;       // indica si se activa el timer del relevador2
int R_TIMER2;          // contador por segundo
String R_DESCRIPTION2; // breve descripcion
bool TEMPORIZADOR2;    // indica si hay un control por tiempo
String TIMEONRELAY2A;  // indica a que horas se prende
String TIMEOFFRELAY2A; // indica a que horas se apaga
String TIMEONRELAY2B;  // indica a que horas se prende
String TIMEOFFRELAY2B; // indica a que horas se apaga
String FECHAON2 = "";  // muestra la FECHA que se encendio
String FECHAOFF2 = ""; // muestra la FECHA que se apago
bool programado2;      // muestra si esta programado el relay2 con fecha
bool LUNES2A, MARTES2A, MIERCOLES2A, JUEVES2A, VIERNES2A, SABADO2A, DOMINGO2A;
bool LUNES2B, MARTES2B, MIERCOLES2B, JUEVES2B, VIERNES2B, SABADO2B, DOMINGO2B;
bool BUZZER_STATUS; // estado del buzzer

String device_fecha; // muestra la fecha actual del dispositivo a la api

//------------------------------------------------------------------------
//  OLED TODO:
//  -----------------------------------------------------------------------
// SDA gpio 8
// scl gpio 9
#define SCREEN_WIDTH 128 // Ancho   128 o 128
#define SCREEN_HEIGHT 32 // Alto     32 0 64

// Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT);
Adafruit_SSD1306 OLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// -----------------------------------------------------------------------
// Activacion de Alarmas de manera local atravez de la switch (GPIO 13) SWITCHTESTALARM
// -----------------------------------------------------------------------
bool normalizar;      // variable necesaria para que funcione Normalizar alarmas
bool prueba = false;  // indica por MQTT y api si esta activada la prueba de las alarmas en caso de requerir hacer una prueba remota envia por mqtt que se hace una prueba de alarmas
bool remotas = false; // indica por MQTT y api si esta activada

// -----------------------------------------------------------------------
// Activacion de los relecadores de manera local atravez de la interupcion (boton 46 y 47)
// -----------------------------------------------------------------------

volatile bool togle = true;
volatile bool togle1 = true; // para el relevador1 en local
volatile bool togle2 = true; // para el relevador2 en local

//-------------variables para cambiar la fecha------------------------------------------------
bool cambiarfecha = false;
String nuevaFecha;

//----------------------------------------------------------------
// Variables para guardar un historial de alarmas
//----------------------------------------------------------------
String alarma; // alamacena momentaneamente el nombre de la alarma
String fechas; // alamacena en global momentaneamente fechas para el archivo de alarmas
bool status;   // almacena momentaneamente el estado de la alarma

//----------------------------------------------------------------
// Variables para guardar la bitácora
//----------------------------------------------------------------
String nombretec; // alamacena momentaneamente el nombre del tecnico
String fecha;     // alamacena en global momentaneamente la fecha para el archivo de la bitácora
String info;      // almacena momentaneamente la informacion en la bitácora
String tipo;      // almacena el tipo de comentario

//---------------------------------------------------------------
//----------------------------------------------------------------------------
// maneja los relay desde la api y verificara el estado por minuto
// Funcion para operar los Relays de forma Global -> API
//----------------------------------------------------------------------------
// releTime estas variables son para indicar que no esta programado el y evitan que vuelvan a programarse
bool releprog1 = false;
bool releprog2 = false;