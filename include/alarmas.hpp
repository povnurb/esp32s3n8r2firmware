void alarmaPresente(String alarma, String fecha, bool presente);

// para telegram
void enviarMensaje(String mensaje);
// para whatsapp
void enviarMensajeWhatsapp(String message);

//   -------------------------------------------------------------------
struct Alarma
{
    uint8_t PIN; // 35 al 42
    String NAME;
    bool LOGICA;
    int MODE;
};
// -------------------------------------------------------------------
// DEFINICION DEL PIN (alarma1)
// PIN: GPIO35 | LOGICA: 0 | MODE: INPUT
// -------------------------------------------------------------------
Alarma alarma1;
Alarma alarma2;
Alarma alarma3;
Alarma alarma4;
Alarma alarma5;
Alarma alarma6;
Alarma alarma7;
Alarma alarma8;

// -------------------------------------------------------------
// SETUP DEL PIN ALARMAS
// -------------------------------------------------------------
void setupPinAlarma1()
{
    alarma1 = {ALARMPIN1, ALARM_NAME1, ALARM_LOGICA1, INPUT_PULLUP};
    if (!ALARM_LOGICA1) // logica es false
    {

        pinMode(alarma1.PIN, alarma1.MODE);
        if (digitalRead(alarma1.PIN))
        {
            ALARM_STATUS1 = true;
        }
        else
        {
            ALARM_STATUS1 = false;
        }
    }
    else // logica true se activa con tierra por lo tanto lo normal es dejar up hacia un voltaje
    {

        pinMode(alarma1.PIN, alarma1.MODE);
        if (digitalRead(alarma1.PIN))
        {
            ALARM_STATUS1 = false;
        }
        else
        {
            ALARM_STATUS1 = true;
        }
    }
    // log("[ INFO ] ALARMA 1 CONFIGURADA: "+ ALARM_NAME1);
}

void setupPinAlarma2()
{
    alarma2 = {ALARMPIN2, ALARM_NAME2, ALARM_LOGICA2, INPUT_PULLUP};
    if (!ALARM_LOGICA2)
    {

        pinMode(alarma2.PIN, alarma2.MODE);
        if (digitalRead(alarma2.PIN))
        {
            ALARM_STATUS2 = true;
        }
        else
        {
            ALARM_STATUS2 = false;
        }
    }
    else
    {
        pinMode(alarma2.PIN, alarma2.MODE);
        if (digitalRead(alarma2.PIN))
        {
            ALARM_STATUS2 = false;
        }
        else
        {
            ALARM_STATUS2 = true;
        }
    }
    // log("[ INFO ] ALARMA 2 CONFIGURADA: "+ ALARM_NAME2);
}

void setupPinAlarma3()
{
    alarma3 = {ALARMPIN3, ALARM_NAME3, ALARM_LOGICA3, INPUT_PULLUP};
    if (!ALARM_LOGICA3)
    {
        pinMode(alarma3.PIN, alarma3.MODE);
        if (digitalRead(alarma3.PIN))
        {
            ALARM_STATUS3 = true;
        }
        else
        {
            ALARM_STATUS3 = false;
        }
    }
    else
    {
        pinMode(alarma3.PIN, alarma3.MODE);
        if (digitalRead(alarma3.PIN))
        {
            ALARM_STATUS3 = false;
        }
        else
        {
            ALARM_STATUS3 = true;
        }
    }
    // log("[ INFO ] ALARMA 3 CONFIGURADA: "+ ALARM_NAME3);
}

void setupPinAlarma4()
{
    alarma4 = {ALARMPIN4, ALARM_NAME4, ALARM_LOGICA4, INPUT_PULLUP};
    if (!ALARM_LOGICA4)
    {
        pinMode(alarma4.PIN, alarma4.MODE);
        if (digitalRead(alarma4.PIN))
        {
            ALARM_STATUS4 = true;
        }
        else
        {
            ALARM_STATUS4 = false;
        }
    }
    else
    {
        pinMode(alarma4.PIN, alarma4.MODE);
        if (digitalRead(alarma4.PIN))
        {
            ALARM_STATUS4 = false;
        }
        else
        {
            ALARM_STATUS4 = true;
        }
    }
    // log("[ INFO ] ALARMA 4 CONFIGURADA: "+ ALARM_NAME4);
}

void setupPinAlarma5()
{
    alarma5 = {ALARMPIN5, ALARM_NAME5, ALARM_LOGICA5, INPUT_PULLUP};
    if (!ALARM_LOGICA5)
    {
        pinMode(alarma5.PIN, alarma5.MODE);
        if (digitalRead(alarma5.PIN))
        {
            ALARM_STATUS5 = true;
        }
        else
        {
            ALARM_STATUS5 = false;
        }
    }
    else
    {
        pinMode(alarma5.PIN, alarma5.MODE);
        if (digitalRead(alarma5.PIN))
        {
            ALARM_STATUS5 = false;
        }
        else
        {
            ALARM_STATUS5 = true;
        }
    }
    // log("[ INFO ] ALARMA 5 CONFIGURADA: "+ ALARM_NAME5);
}

void setupPinAlarma6()
{
    alarma6 = {ALARMPIN6, ALARM_NAME6, ALARM_LOGICA6, INPUT_PULLUP};
    if (!ALARM_LOGICA6)
    {
        pinMode(alarma6.PIN, alarma6.MODE);
        if (digitalRead(alarma6.PIN))
        {
            ALARM_STATUS6 = true;
        }
        else
        {
            ALARM_STATUS6 = false;
        }
    }
    else
    {
        pinMode(alarma6.PIN, alarma6.MODE);
        if (digitalRead(alarma6.PIN))
        {
            ALARM_STATUS6 = false;
        }
        else
        {
            ALARM_STATUS6 = true;
        }
    }
    // log("[ INFO ] ALARMA 6 CONFIGURADA: "+ ALARM_NAME6);
}

void setupPinAlarma7()
{
    alarma7 = {ALARMPIN7, ALARM_NAME7, ALARM_LOGICA7, INPUT_PULLUP};
    if (!ALARM_LOGICA7)
    {
        pinMode(alarma7.PIN, alarma7.MODE);
        if (digitalRead(alarma7.PIN))
        {
            ALARM_STATUS7 = true;
        }
        else
        {
            ALARM_STATUS7 = false;
        }
    }
    else
    {
        pinMode(alarma7.PIN, alarma7.MODE);
        if (digitalRead(alarma7.PIN))
        {
            ALARM_STATUS7 = false;
        }
        else
        {
            ALARM_STATUS7 = true;
        }
    }
    // log("[ INFO ] ALARMA 7 CONFIGURADA: "+ ALARM_NAME7);
}

void setupPinAlarma8()
{
    alarma8 = {ALARMPIN8, ALARM_NAME8, ALARM_LOGICA8, INPUT_PULLUP};
    if (!ALARM_LOGICA8)
    {
        pinMode(alarma8.PIN, alarma8.MODE);
        if (digitalRead(alarma8.PIN))
        {
            ALARM_STATUS8 = true;
        }
        else
        {
            ALARM_STATUS8 = false;
        }
    }
    else
    {
        pinMode(alarma8.PIN, alarma8.MODE);
        if (digitalRead(alarma8.PIN))
        {
            ALARM_STATUS8 = false;
        }
        else
        {
            ALARM_STATUS8 = true;
        }
    }
    // log("[ INFO ] ALARMA 8 CONFIGURADA: "+ ALARM_NAME8);
}
// seteamos el GPIO13 que es un swith que activa todas las alarmas de manera local
void setupPinTestAlarmas()
{
    pinMode(SWITCHTESTALARM, INPUT_PULLUP);
}
void setupPinAlarmas()
{
    setupPinAlarma1();
    setupPinAlarma2();
    setupPinAlarma3();
    setupPinAlarma4();
    setupPinAlarma5();
    setupPinAlarma6();
    setupPinAlarma7();
    setupPinAlarma8();
    setupPinTestAlarmas();
}

bool statusAlarm1()
{
    alarma1 = {ALARMPIN1, ALARM_NAME1, ALARM_LOGICA1, INPUT_PULLUP};
    if (!ALARM_LOGICA1)
    {
        pinMode(alarma1.PIN, alarma1.MODE);
        if (digitalRead(alarma1.PIN))
        {
            ALARM_STATUS1 = true;
        }
        else
        {
            ALARM_STATUS1 = false;
        }
    }
    else // funciona con tierra
    // https://www.upesy.com/blogs/tutorials/how-to-use-gpio-pins-of-esp32-with-arduino#
    {
        pinMode(alarma1.PIN, alarma1.MODE);
        if (digitalRead(alarma1.PIN))
        {
            ALARM_STATUS1 = false;
        }
        else
        {
            ALARM_STATUS1 = true;
        }
    }
    return ALARM_STATUS1;
}

bool statusAlarm2()
{
    alarma2 = {ALARMPIN2, ALARM_NAME2, ALARM_LOGICA2, INPUT_PULLUP};
    if (!ALARM_LOGICA2)
    {
        pinMode(alarma2.PIN, alarma2.MODE);
        if (digitalRead(alarma2.PIN))
        {
            ALARM_STATUS2 = true;
        }
        else
        {
            ALARM_STATUS2 = false;
        }
    }
    else
    {
        pinMode(alarma2.PIN, alarma2.MODE);
        if (digitalRead(alarma2.PIN))
        {
            ALARM_STATUS2 = false;
        }
        else
        {
            ALARM_STATUS2 = true;
        }
    }
    return ALARM_STATUS2;
}

bool statusAlarm3()
{
    alarma3 = {ALARMPIN3, ALARM_NAME3, ALARM_LOGICA3, INPUT_PULLUP};
    if (!ALARM_LOGICA3)
    {
        pinMode(alarma3.PIN, alarma3.MODE);
        if (digitalRead(alarma3.PIN))
        {
            ALARM_STATUS3 = true;
        }
        else
        {
            ALARM_STATUS3 = false;
        }
    }
    else
    {
        pinMode(alarma3.PIN, alarma3.MODE);
        if (digitalRead(alarma3.PIN))
        {
            ALARM_STATUS3 = false;
        }
        else
        {
            ALARM_STATUS3 = true;
        }
    }
    return ALARM_STATUS3;
}

bool statusAlarm4()
{
    alarma4 = {ALARMPIN4, ALARM_NAME4, ALARM_LOGICA4, INPUT_PULLUP};
    if (!ALARM_LOGICA4)
    {
        pinMode(alarma4.PIN, alarma4.MODE);
        if (digitalRead(alarma4.PIN))
        {
            ALARM_STATUS4 = true;
        }
        else
        {
            ALARM_STATUS4 = false;
        }
    }
    else
    {
        pinMode(alarma4.PIN, alarma4.MODE);
        if (digitalRead(alarma4.PIN))
        {
            ALARM_STATUS4 = false;
        }
        else
        {
            ALARM_STATUS4 = true;
        }
    }
    return ALARM_STATUS4;
}
bool statusAlarm5()
{
    alarma5 = {ALARMPIN5, ALARM_NAME5, ALARM_LOGICA5, INPUT_PULLUP};
    if (!ALARM_LOGICA5)
    {
        pinMode(alarma5.PIN, alarma5.MODE);
        if (digitalRead(alarma5.PIN))
        {
            ALARM_STATUS5 = true;
        }
        else
        {
            ALARM_STATUS5 = false;
        }
    }
    else
    {
        pinMode(alarma5.PIN, alarma5.MODE);
        if (digitalRead(alarma5.PIN))
        {
            ALARM_STATUS5 = false;
        }
        else
        {
            ALARM_STATUS5 = true;
        }
    }
    return ALARM_STATUS5;
}
bool statusAlarm6()
{
    alarma6 = {ALARMPIN6, ALARM_NAME6, ALARM_LOGICA6, INPUT_PULLUP};
    if (!ALARM_LOGICA6)
    {
        pinMode(alarma6.PIN, alarma6.MODE);
        if (digitalRead(alarma6.PIN))
        {
            ALARM_STATUS6 = true;
        }
        else
        {
            ALARM_STATUS6 = false;
        }
    }
    else
    {
        pinMode(alarma6.PIN, alarma6.MODE);
        if (digitalRead(alarma6.PIN))
        {
            ALARM_STATUS6 = false;
        }
        else
        {
            ALARM_STATUS6 = true;
        }
    }
    return ALARM_STATUS6;
}
bool statusAlarm7()
{
    alarma7 = {ALARMPIN7, ALARM_NAME7, ALARM_LOGICA7, INPUT_PULLUP};
    if (!ALARM_LOGICA7)
    {
        pinMode(alarma7.PIN, alarma7.MODE);
        if (digitalRead(alarma7.PIN))
        {
            ALARM_STATUS7 = true;
        }
        else
        {
            ALARM_STATUS7 = false;
        }
    }
    else
    {
        pinMode(alarma7.PIN, alarma7.MODE);
        if (digitalRead(alarma7.PIN))
        {
            ALARM_STATUS7 = false;
        }
        else
        {
            ALARM_STATUS7 = true;
        }
    }
    return ALARM_STATUS7;
}
bool statusAlarm8()
{
    alarma8 = {ALARMPIN8, ALARM_NAME8, ALARM_LOGICA8, INPUT_PULLUP};
    if (!ALARM_LOGICA8)
    {
        pinMode(alarma8.PIN, alarma8.MODE);
        if (digitalRead(alarma8.PIN))
        {
            ALARM_STATUS8 = true;
        }
        else
        {
            ALARM_STATUS8 = false;
        }
    }
    else
    {
        pinMode(alarma8.PIN, alarma8.MODE);
        if (digitalRead(alarma8.PIN))
        {
            ALARM_STATUS8 = false;
        }
        else
        {
            ALARM_STATUS8 = true;
        }
    }
    return ALARM_STATUS8;
}

// contador de alarmas
void contadorAlarmas()
{
    int pines[NUM_ALARMS] = {ALARMPIN1, ALARMPIN2, ALARMPIN3, ALARMPIN4, ALARMPIN5, ALARMPIN6, ALARMPIN7, ALARMPIN8};
    logicas[0] = ALARM_LOGICA1;
    logicas[1] = ALARM_LOGICA2;
    logicas[2] = ALARM_LOGICA3;
    logicas[3] = ALARM_LOGICA4;
    logicas[4] = ALARM_LOGICA5;
    logicas[5] = ALARM_LOGICA6;
    logicas[6] = ALARM_LOGICA7;
    logicas[7] = ALARM_LOGICA8;
    bool reconocer[NUM_ALARMS] = {ALARM_RECONOCIDA1, ALARM_RECONOCIDA2, ALARM_RECONOCIDA3, ALARM_RECONOCIDA4, ALARM_RECONOCIDA5, ALARM_RECONOCIDA6, ALARM_RECONOCIDA7, ALARM_RECONOCIDA8};
    String names[NUM_ALARMS] = {ALARM_NAME1, ALARM_NAME2, ALARM_NAME3, ALARM_NAME4, ALARM_NAME5, ALARM_NAME6, ALARM_NAME7, ALARM_NAME8};
    for (int i = 0; i < NUM_ALARMS; i++)
    {
        if (!logicas[i])
        { // si la logica es normal
            // ALARM_STATUS[i] = digitalRead(pines[i]);
            if (digitalRead(pines[i]) && !cambiar[i])
            {
                cont[i]++;
                fechaAct[i] = getDateTime();
                fechaClear[i] = "";
                cambiar[i] = true;
                // TODO: reconocer es para registrar la alarma
                if (reconocer[i])
                {

                    alarmaPresente(names[i], fechaAct[i], true);
                }
            }
            else if (!digitalRead(pines[i]) && cambiar[i])
            {
                cambiar[i] = false;
                fechaClear[i] = getDateTime();
                // TODO:
                if (reconocer[i])
                {

                    alarmaPresente(names[i], fechaAct[i], false);
                }
            }
        }
        else
        { // si la logica es invertida
            if (!digitalRead(pines[i]) && !cambiar[i])
            {
                cont[i]++;
                fechaAct[i] = getDateTime();
                fechaClear[i] = "";
                cambiar[i] = true;
                // TODO:
                if (reconocer[i])
                {

                    alarmaPresente(names[i], fechaAct[i], true);
                }
            }
            else if (digitalRead(pines[i]) && cambiar[i])
            {
                cambiar[i] = false;
                fechaClear[i] = getDateTime();
                // TODO:
                if (reconocer[i])
                {

                    alarmaPresente(names[i], fechaClear[i], false);
                }
            }
        }
        //}
    }
    ALARM_CONT1 = cont[0];
    ALARM_CONT2 = cont[1];
    ALARM_CONT3 = cont[2];
    ALARM_CONT4 = cont[3];
    ALARM_CONT5 = cont[4];
    ALARM_CONT6 = cont[5];
    ALARM_CONT7 = cont[6];
    ALARM_CONT8 = cont[7];
    ALARM_TIMEON1 = fechaAct[0];
    ALARM_TIMEON2 = fechaAct[1];
    ALARM_TIMEON3 = fechaAct[2];
    ALARM_TIMEON4 = fechaAct[3];
    ALARM_TIMEON5 = fechaAct[4];
    ALARM_TIMEON6 = fechaAct[5];
    ALARM_TIMEON7 = fechaAct[6];
    ALARM_TIMEON8 = fechaAct[7];
    ALARM_TIMEOFF1 = fechaClear[0];
    ALARM_TIMEOFF2 = fechaClear[1];
    ALARM_TIMEOFF3 = fechaClear[2];
    ALARM_TIMEOFF4 = fechaClear[3];
    ALARM_TIMEOFF5 = fechaClear[4];
    ALARM_TIMEOFF6 = fechaClear[5];
    ALARM_TIMEOFF7 = fechaClear[6];
    ALARM_TIMEOFF8 = fechaClear[7];
    ALARM_NAME[0] = ALARM_NAME1;
    ALARM_NAME[1] = ALARM_NAME2;
    ALARM_NAME[2] = ALARM_NAME3;
    ALARM_NAME[3] = ALARM_NAME4;
    ALARM_NAME[4] = ALARM_NAME5;
    ALARM_NAME[5] = ALARM_NAME6;
    ALARM_NAME[6] = ALARM_NAME7;
    ALARM_NAME[7] = ALARM_NAME8;
};

// ----------------------------------------------------------------------------------
// Activar Alarmas por medio de un switch
// ----------------------------------------------------------------------------------
void activarAlarma()
{ // para provar despues en casa
    int pines[NUM_ALARMS] = {ALARMPIN1, ALARMPIN2, ALARMPIN3, ALARMPIN4, ALARMPIN5, ALARMPIN6, ALARMPIN7, ALARMPIN8};
    bool logicas[NUM_ALARMS] = {ALARM_LOGICA1, ALARM_LOGICA2, ALARM_LOGICA3, ALARM_LOGICA4, ALARM_LOGICA5, ALARM_LOGICA6, ALARM_LOGICA7, ALARM_LOGICA8};
    if (!remotas) // si se manda el comando via remota cambia la condicion de las alarmas
    {
        if (!digitalRead(SWITCHTESTALARM))
        {
            Serial.println("Activando Alarmas");
            prueba = true; // prueba de alarmas envio por mqtt
            // digitalWrite(32,HIGH);
            normalizar = false; // variable necesaria para hacer la conmutacion
            for (int i = 0; i < NUM_ALARMS; i++)
            {
                if (logicas[i])
                {
                    pinMode(pines[i], OUTPUT); // activa las alarmas de manera remota
                    digitalWrite(pines[i], LOW);
                    cont[i] = 0;
                }
                else
                {
                    pinMode(pines[i], INPUT_PULLUP);
                    digitalWrite(pines[i], HIGH); // tenia 1
                    cont[i] = 0;
                }
            }
        }
        else if (digitalRead(SWITCHTESTALARM) && !normalizar)
        {
            Serial.println("Desactivando Alarmas");
            normalizar = true;
            setupPinAlarmas(); // ok
            prueba = false;    // se quita la prueba de alarmas por mqtt
            for (int i = 0; i < NUM_ALARMS; i++)
            {
                cont[i] = 0;
            }
        }
    }
    else
    {
        if (digitalRead(SWITCHTESTALARM))
        {
            Serial.println("Activando Alarmas");
            prueba = true; // prueba de alarmas envio por mqtt
            // digitalWrite(32,HIGH);
            normalizar = false; // variable necesaria para hacer la conmutacion
            for (int i = 0; i < NUM_ALARMS; i++)
            {
                if (logicas[i])
                {
                    pinMode(pines[i], OUTPUT); // activa las alarmas de manera remota
                    digitalWrite(pines[i], LOW);
                    cont[i] = 0;
                }
                else
                {
                    pinMode(pines[i], INPUT_PULLUP);
                    digitalWrite(pines[i], HIGH); // tenia 1
                    cont[i] = 0;
                }
            }
        }
        else if (digitalRead(SWITCHTESTALARM) && !normalizar)
        {
            Serial.println("Desactivando Alarmas");
            normalizar = true;
            setupPinAlarmas(); // ok
            prueba = false;    // se quita la prueba de alarmas por mqtt
            for (int i = 0; i < NUM_ALARMS; i++)
            {
                cont[i] = 0;
            }
        }
    }
}

// fechador de relays
void fechaRlay()
{

    if (RELAY1_STATUS && !CRELAY1)
    {

        FECHAON1 = fechaActual(); // por el formato
        FECHAOFF1 = "";
        CRELAY1 = true;
        if (programado1)
        {
            enviarMensaje("El relevador " + String(R_NAME1) + " de la sala " + sala + " esta activado");
            enviarMensajeWhatsapp("El relevador " + String(R_NAME1) + " de la sala " + sala + " esta activado");
        }
    }
    else if (!RELAY1_STATUS && CRELAY1)
    {
        CRELAY1 = false;
        FECHAOFF1 = fechaActual(); // por el formato
        if (programado1)
        {
            enviarMensaje("El relevador " + String(R_NAME1) + " de la sala " + sala + " esta desactivado");
            enviarMensajeWhatsapp("El relevador " + String(R_NAME1) + " de la sala " + sala + " esta desactivado");
        }
    }
    if (RELAY2_STATUS && !CRELAY2)
    {

        FECHAON2 = fechaActual(); // por el formato
        FECHAOFF2 = "";
        CRELAY2 = true;
        if (programado2)
        {
            enviarMensaje("El relevador " + String(R_NAME2) + " de la sala " + sala + " esta activado");
            enviarMensajeWhatsapp("El relevador " + String(R_NAME2) + " de la sala " + sala + " esta activado");
        }
    }
    else if (!RELAY2_STATUS && CRELAY2)
    {
        CRELAY2 = false;
        FECHAOFF2 = fechaActual(); // por el formato
        if (programado2)
        {
            enviarMensaje("El relevador " + String(R_NAME2) + " de la sala " + sala + " esta desactivado");
            enviarMensajeWhatsapp("El relevador " + String(R_NAME2) + " de la sala " + sala + " esta desactivado");
        }
    }
};
//---------------------------------------------------------------------------------
// zona para guardar alarmas en un spiffs
// ----------------------------------------------------------------------------------
void alarmasReset()
{
    alarma = "El historial de alarmas inicia";
    fechas = getDateTime(); // no requiere formato
    status = true;
    myLog("INFO", "alarmas.hpp", "alarmasReset()", "creando archivo de alarmas");
}

boolean exiteArchivoAlarmas()
{
    JsonDocument jsonalarmas;                            // objeto jsonalarmas donde se leeran las configuraciones iniciales
    File file = SPIFFS.open("/alarmas.json", FILE_READ); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    { // instalar la extencion TODO Highlight
        // FIXME: Serial.println("[WARNING][alarmas.hpp][alarmasRead()]Falla en abrir el archivo alarmas.json");
        // dejare ese FIXME como ejemplo pero la siguiente linea es la solucion
        myLog("ERROR", "alarmas.hpp", "exiteArchivoAlarmas()", "Error al abrir el archivo alarmas.json o este no existe");
        // TODO: llamar a la función de iniciar el json de fabrica (HECHO)
        alarmasReset();
        return false;
    }
    // Deserealizacion del archivo alarmas.json osea, lo que esta en el archivo file
    // lo pazamos a la variable jsonalarmas gracias a la funcion deserializeJson() y
    // si eso no sucede dará un error
    // la deseralizeJson() si todo sale ok devuelve un 0 o false indicando que no hay error
    // en caso contrario da
    DeserializationError error = deserializeJson(jsonalarmas, file);
    // cerramos el archivo este paso es necesario ya que ya no o usaremos y con esto liberamos memoria
    file.close();
    // si se presenta un error
    if (error)
    {
        myLog("ERROR", "alarmas.hpp", "exiteArchivoAlarmas()", "Fallo la deserealización del archivo alarmas.json");
        alarmasReset();
        return false;
    }
    myLog("INFO", "alarmas.hpp", "exiteArchivoAlarmas()", "Se procede a asignar valores almacenados a las variables");
    // si ya pasamos todos esos errores, pasaremos a copiar los valores del archivo json a las variables globales
    // como son de tipo char

    return true;
}

boolean crearArchivoAlarmas()
{
    // definicion del json
    JsonDocument jsonalarmas; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales
    jsonalarmas["alarma"] = alarma;
    jsonalarmas["fechas"] = fechas;
    jsonalarmas["status"] = status;

    // guardar el json en spiffs
    File file = SPIFFS.open("/alarmas.json", "w"); // modo escritura sustituye la informacion (borra)
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "alarmas.hpp", "crearArchivoAlarmas()", "Error en abrir el archivo de configuraciones alarmas.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsonalarmas, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "alarmas.hpp", "serializeJson(jsonalarmas,file)", "Error al serializar el archivo alarmas.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "alarmas.hpp", "crearArchivoAlarmas()", "Se guardaron las configuraciones en el archivo alarmas.json");
    // mostrar el alarmas.json en el monitor
    // serializeJsonPretty(jsonalarmas, Serial);
    return true;
}
boolean salvarAlarmaPresente(String alarma, String fechas, bool status)
{
    // definicion del json
    JsonDocument jsonalarmas; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales

    jsonalarmas["alarma"] = alarma;
    jsonalarmas["fechas"] = fechas;
    jsonalarmas["status"] = status;

    // guardar el json en spiffs
    // https://esp8266-arduino-spanish.readthedocs.io/es/latest/filesystem.html
    File file = SPIFFS.open("/alarmas.json", "a+"); // modo escritura para añadir
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "alarmas.hpp", "alarmasSave()", "Error en abrir el archivo de configuraciones alarmas.json al guardar la información");
        return false;
    }
    file.print(','); // TODO: para agregar una ,
    //   guardando y buscando que no se presenten errores
    if (serializeJson(jsonalarmas, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "alarmas.hpp", "serializeJson(jsonalarmas,file)", "Error al serializar el archivo alarmas.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "alarmas.hpp", "alarmasSave()", "Se guardaron las configuraciones en el archivo alarmas.json");
    // mostrar el alarmas.json en el monitor
    // serializeJsonPretty(jsonalarmas, Serial);
    return true;
}

void alarmaPresente(String alarma, String fecha, bool presente)
{
    if (salvarAlarmaPresente(alarma, fecha, presente))
    {
        myLog("INFO", "alarmas.hpp", "alarmaPresente()", "añadiendo alarmas presentes");

        if (presente)
        {

            enviarMensaje("Alarma presente en " + sala + ": " + alarma);
            enviarMensajeWhatsapp("Alarma presente en " + sala + ": " + alarma);
        }
        else
        {
            enviarMensaje("Alarma clareada en " + sala + ": " + alarma);
            enviarMensajeWhatsapp("Alarma clareada en " + sala + ": " + alarma);
        }
    }
    else
    {
        myLog("ERROR", "alarmas.hpp", "alarmaPresente()", "No se guardo archivo de alarmas");
    }
}

boolean deleteArchivoAlarmas()
{
    // definicion del json
    JsonDocument jsonalarmas; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales
    jsonalarmas["alarma"] = "Archivo de alarmas borrado";
    jsonalarmas["fechas"] = getDateTime();
    jsonalarmas["status"] = false;

    // guardar el json en spiffs
    File file = SPIFFS.open("/alarmas.json", "w"); // modo escritura sustituye la informacion (borra)
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "alarmas.hpp", "crearArchivoAlarmas()", "Error en abrir el archivo de configuraciones alarmas.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsonalarmas, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "alarmas.hpp", "serializeJson(jsonalarmas,file)", "Error al serializar el archivo alarmas.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "alarmas.hpp", "crearArchivoAlarmas()", "Se guardaron las configuraciones en el archivo alarmas.json");
    // mostrar el alarmas.json en el monitor
    // serializeJsonPretty(jsonalarmas, Serial);
    return true;
}