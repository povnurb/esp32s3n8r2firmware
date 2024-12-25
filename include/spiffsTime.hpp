// definicion de funciones
boolean timeRead();
boolean timeSave();
void timeReset();

// funcion que leera el archivo time.json devolviendo un true o false
//  documentación www.arduinojson.org/v7/
//  la nueva version de JSON ya calcula el tamaño (excelente)

boolean timeRead()
{
    JsonDocument jsontime;                            // objeto jsontime donde se leeran las configuraciones iniciales
    File file = SPIFFS.open("/time.json", FILE_READ); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    { // instalar la extencion TODO Highlight
        // FIXME: Serial.println("[WARNING][spiffsTime.hpp][timeRead()]Falla en abrir el archivo time.json");
        // dejare ese FIXME como ejemplo pero la siguiente linea es la solucion
        myLog("ERROR", "spiffsTime.hpp", "timeRead()", "Error al abrir el archivo time.json o este no existe");
        // TODO: llamar a la función de iniciar el json de fabrica (HECHO)
        timeReset();
        return false;
    }
    // Deserealizacion del archivo time.json osea, lo que esta en el archivo file
    // lo pazamos a la variable jsontime gracias a la funcion deserializeJson() y
    // si eso no sucede dará un error
    // la deseralizeJson() si todo sale ok devuelve un 0 o false indicando que no hay error
    // en caso contrario da
    DeserializationError error = deserializeJson(jsontime, file);
    // cerramos el archivo este paso es necesario ya que ya no o usaremos y con esto liberamos memoria
    file.close();
    // si se presenta un error
    if (error)
    {
        myLog("ERROR", "spiffsTime.hpp", "timeRead()", "Fallo la deserealización del archivo time.json");
        timeReset();
        return false;
    }
    myLog("INFO", "spiffsTime.hpp", "timeRead()", "Se procede a asignar valores almacenados a las variables");
    // si ya pasamos todos esos errores, pasaremos a copiar los valores del archivo json a las variables globales
    // como son de tipo char

    // TODO: funcion for que lee los valores del archivo y los asigna a probar
    for (int i = 0; i < NUM_VALORES; i++)
    {

        // strlcpy( vTime[i], jsontime[i] | "", sizeof(mqtt_id));
        strlcpy(vTime[i], jsontime[i], sizeof(vTime[i]));
        // vTime[i] = jsontime[i].as<String>();
    }

    myLog("INFO", "spiffsTime.hpp", "timeRead()", "lecturas de las configuraciones correctamente");
    // Serial.print("ajuste DHT22: ");
    // Serial.println(ajTmpDht22);
    return true;
}

// Funcion que resetea a los valores de fabrica
void timeReset()
{
    myLog("INFO", "spiffsTime.hpp", "timeReset()", "Se procede a asignar valores de fabrica");

    // TODO:funcion que asigna el valor inicial de restauracion
    for (int i = 0; i < NUM_VALORES; i++)
    {
        strcpy(vTime[i], "|");
    }

    myLog("INFO", "spiffsTime.hpp", "timeReset()", "Se reiniciaron todos los valores por defecto de fabrica");
}
// hasta el momento todo esto solo pone en la memoria la información pero aun no se salva y toda la configuración se pierde al reiniciar el dispositivo
// Guardar configuraciones en el json
boolean timeSave()
{
    // definicion del json
    JsonDocument jsontime; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    File dataTime = SPIFFS.open("/time.json", "w+"); // modo escritura (w+) con el path / que es en raiz y el nombre es dataTemp con la extención json

        // TODO: funcion que guarda los valores
    for (int i = 0; i < NUM_VALORES; i++)
    {
        // jsontime[i] = vTime[i];
        jsontime[i] = vTime[i]; // ␘
    }

    //  guardar el json en spiffs
    File file = SPIFFS.open("/time.json", "w"); // modo escritura
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "spiffsTime.hpp", "timeSave()", "Error en abrir el archivo de configuraciones time.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsontime, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "spiffsTime.hpp", "serializeJson(jsontime,file)", "Error al serializar el archivo time.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "spiffsTime.hpp", "timeSave()", "Se guardaron las configuraciones en el archivo time.json");
    // mostrar el time.json en el monitor
    // serializeJsonPretty(jsontime, Serial);
    return true;
}
