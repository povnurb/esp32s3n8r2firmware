// aqui se encuentran las principales variables del entorno a modificar segun las necesidades
//  Accseso al dispositivo para el topico

String gerencia;
String ciudad;                    // de 6 digitos minuscula en settingsreset()
String central;                   // de 6 digitos minuscula
String sala;                      // de 7 digitos minuscula
String DeviceName = "HGO-PTTI-2"; // donde se encuentra el dispositivo instalado
String Administrador = "admin";
String Contra = "personal"; // tanto para modo AP
// Acceso WiFi
String SSID = "INFINITUMD378";       //"INFINITUMD378"(hogar), "INFINITUM3213"(trabajo)
String SSID2 = "INFINITUM3213";      //"INFINITUMD378"(hogar), "INFINITUM3213"(trabajo)
String WifiPassword = "Pm2Kj1Jg6j";  //"Pm2Kj1Jg6j"(hogar),"6cyH34xK4f"(trabajo)
String WifiPassword2 = "6cyH34xK4f"; //"Pm2Kj1Jg6j"(hogar),"6cyH34xK4f"(trabajo)
boolean WifiMode = true;            // true es igual modo estacion osea false en modo AP
// boolean switchModeAP = true;        // ejemplo de sustitucion de un swithch que cambia el modo de manera manual false = AP
//  la variable switchModeAp en true significa que no cambia a modo stacion por default en false
boolean DHCP = true;
String Ipv4 = "10.4.30.15";
String SubRed = "255.255.255.0";
String Gateway = "10.4.30.254";
String DnsPrimary = "10.192.10.1";
String DnsSecondary = "10.106.10.2";
// mqtt acceso
boolean MqttAcceso = true;
String MqttServer = "140.84.188.157"; // iotmx.com pero se puede perder el dominio
int MqttPort = 1883;
String MqttUser = "asignar";
String MqttPassword = "public";

// si dese desea cambiar el topico es en la funcion PathMqttTopic() en el archivo functions.hpp

//-----------------------------------------------------------
// Configuración del bot de Telegram
// https://tfmingenieria.com/enviar-mensajes-desde-un-esp32-a-telegram/
boolean telegram;
String botToken; // Reemplaza con tu token de bot
String chatID;
// el chat ID se le pregunta al bot GET MY ID se busca en telegram

// solo para probar mientras
boolean whatsapp = true;
String MobileNumber = "ADD_YOUR_PHONE_NUMBER";
String APIKey = "ADD_YOUR_API_KEY";