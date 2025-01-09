// asigna las variables de inicia de la bitácora
void bitacoraReset()
{
    nombretec = "Bot ESP32S3";
    fecha = getDateTime(); // no requiere formato
    tipo = "inicio";
    info = "Inicia bitácora";
    myLog("INFO", "spiffasBitacora.hpp", "bitacoraReset()", "creando variables de la bitácora");
}

// verificar que exita el archivo de la bitácora
boolean existeBitacora()
{
    JsonDocument jsonbitacora;                            // objeto jsonbitacora donde se leeran las configuraciones iniciales
    File file = SPIFFS.open("/bitacora.json", FILE_READ); // modo lectura antes o tambien en vez de FILE_READ , "r"

    if (!file)
    {
        // si no existe
        myLog("ERROR", "spiffsBitacora.hpp", "existeBitacora()", "Error al abrir el archivo bitacora.json o este no existe");
        // llamar a la función de iniciar el json de fabrica bitacora
        bitacoraReset();
        return false;
    }
    // Deserealizacion del archivo bitacora.json osea, lo que esta en el archivo file
    // lo pazamos a la variable jsonbitacora gracias a la funcion deserializeJson() y
    // si eso no sucede dará un error
    // la deseralizeJson() si todo sale ok devuelve un 0 o false indicando que no hay error
    // en caso contrario da
    DeserializationError error = deserializeJson(jsonbitacora, file);
    // cerramos el archivo este paso es necesario ya que ya no o usaremos y con esto liberamos memoria
    file.close();
    // si se presenta un error
    if (error)
    {
        myLog("ERROR", "spiffsBitacora.hpp", "existeBitacora()", "Fallo la deserealización del archivo bitacora.json");
        bitacoraReset();
        return false;
    }
    myLog("INFO", "spiffsBitacora.hpp", "existeBitacora()", "Se procede a asignar valores almacenados a las variables");
    // si ya pasamos todos esos errores, pasaremos a copiar los valores del archivo json a las variables globales
    // como son de tipo char

    return true;
}

// crear el archivo Json de la bitácora
boolean crearBitacora()
{
    // definicion del json
    JsonDocument jsonbitacora; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales
    jsonbitacora["nombretec"] = nombretec; // nombre del tecnico
    jsonbitacora["fecha"] = fecha;
    jsonbitacora["tipo"] = tipo;
    jsonbitacora["info"] = info;

    // guardar el json en spiffs
    File file = SPIFFS.open("/bitacora.json", "w"); // modo escritura sustituye la informacion (borra)
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "spifffsBitacora.hpp", "crearBitacora()", "Error en abrir el archivo de configuraciones bitacora.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsonbitacora, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "spifffsBitacora.hpp", "serializeJson(jsonbitacora,file)", "Error al serializar el archivo bitacora.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "spifffsBitacora.hpp", "crearBitacora()", "Se guardaron las configuraciones en el archivo bitacora.json");
    // mostrar el bitacora.json en el monitor
    // serializeJsonPretty(jsonbitacora, Serial);
    return true;
}

boolean salvarBitacora(String nombretecnico, String fechaB, String tipoD, String infoB)
{
    // definicion del json
    JsonDocument jsonbitacora; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales

    jsonbitacora["nombretec"] = nombretecnico;
    jsonbitacora["fecha"] = fechaB; // fecha de bitacora
    jsonbitacora["tipo"] = tipoD;   // tipo de dato
    jsonbitacora["info"] = infoB;   // info de bitacora

    // guardar el json en spiffs
    // https://esp8266-arduino-spanish.readthedocs.io/es/latest/filesystem.html
    File file = SPIFFS.open("/bitacora.json", "a+"); // modo escritura para añadir
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "spiffsBitacora.hpp", "salvarBitacora()", "Error en abrir el archivo de configuraciones bitacora.json al guardar la información");
        return false;
    }
    file.print(','); // TODO: para agregar una ,
    //   guardando y buscando que no se presenten errores
    if (serializeJson(jsonbitacora, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "spiffsBitacora.hpp", "serializeJson(jsonbitacora,file)", "Error al serializar el archivo bitacora.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "spiffsBitacora.hpp", "salvarBitacora()", "Se guardaron las configuraciones en el archivo bitacora.json");
    // mostrar el bitacora.json en el monitor
    // serializeJsonPretty(jsonbitacora, Serial);
    return true;
}

// eliminar bitácora
boolean borrarBitacora()
{
    // definicion del json
    JsonDocument jsonbitacora; // se crea una variable tipo JSON
    // asignando en el JSON las variables que estan en memoria
    // generales
    jsonbitacora["nombretec"] = "Bot ESP32S3";
    jsonbitacora["fecha"] = getDateTime();
    jsonbitacora["tipo"] = "inicio";
    jsonbitacora["info"] = "Bitacora borrada";

    // guardar el json en spiffs
    File file = SPIFFS.open("/bitacora.json", "w"); // modo escritura sustituye la informacion (borra)
    if (!file)
    { // verificamos que el archivo se pueda abrir
        myLog("ERROR", "spiffsBitacora.hpp", "borrarBitacora()", "Error en abrir el archivo de informacion bitacora.json al guardar la información");
        return false;
    }
    // guardando y buscando que no se presenten errores
    if (serializeJson(jsonbitacora, file) == 0)
    { // manda un numero en caso de no poder realizar la función
        myLog("ERROR", "spiffsBitacora.hpp", "serializeJson(jsonbitacora,file)", "Error al serializar el archivo bitacora.json");
        file.close();
        return false;
    }
    file.close();
    myLog("INFO", "spiffsBitacora.hpp", "borrarBitacora()", "Se guardaron la informacion en el archivo bitacora.json");
    // mostrar el bitacora.json en el monitor
    // serializeJsonPretty(jsonbitacora, Serial);
    return true;
}