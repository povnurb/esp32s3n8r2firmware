
bool dataGraficasSave()
{
    // definicion del json
    JsonDocument jsonDataTime; // se crea una variable tipo JSON
    // jsonData es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará data
    File dataTime = SPIFFS.open("/dataTime.json", "w+"); // modo escritura (w+) con el path / que es en raiz y el nombre es dataTime con la extención json
    // cuando se abre asi se borra el contenido y se sobre escribe
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    if (dataTime)
    { // si se puede abrir el archivo en modo escritura
        /********************************************************************************
         * Zona configuracion Dispositivo dataTime.json asignando nuevo valores         *
         * a las variables que se encuentran en el archivo file                         *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            jsonDataTime[i] = vTime[i];
        }
        serializeJsonPretty(jsonDataTime, dataTime); // escribe en el archivo file
        // serializeJsonPretty(jsonDataTime, Serial);
        dataTime.close(); // ya que todo se leera se cierra el archivo
        // Serial.println("*******************************");
        // myLog("INFO", "spiffsGraficas.hpp", "dataGraficasSave()", "valor de Timeeratura guardada en dataTime.Json");
        Serial.flush();
        // serializeJsonPretty(jsonSettings,Serial); //lo escribe en el monitor serial
    }
    else
    {
        myLog("ERROR", "spiffsGraficas.hpp", "dataGraficasSave()", "no se puede obteber informacion de la data de Timeeraturas o no existe");
        return false;
    }

    return true;
}
// definiciones
void dataGraficasReset()
{
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasReset()", "Reiniciando valores");
    for (int i = 0; i < NUM_VALORES; i++)
    {
        vTemp[i] = 0;
    }
    for (int i = 0; i < NUM_VALORES; i++)
    {
        vHum[i] = 0;
    }
    for (int i = 0; i < NUM_VALORES; i++)
    {
        vSensorPozo1[i] = 0;
    }
    for (int i = 0; i < NUM_VALORES; i++)
    {
        vSensorPozo2[i] = 0;
    }
    for (int i = 0; i < NUM_VALORES; i++)
    {
        vSLm35[i] = 0;
    }
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasReset()", "Guardando valores de 0 en Temperatura y humedad");
    dataGraficasSave();
}

bool dataGraficasRead()
{
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Entrando a leer los datos de temperatura y humedad");
    JsonDocument jsonDataTemp; // se crea una variable tipo JSON // capacidad definida en globales.hpp (512B)
    // jsonDataTemp es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará file
    File dataTemp = SPIFFS.open("/dataTemp.json", "r"); // modo lectura (r) con el path / que es en raiz y el nombre es settings con la extención json
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    // se realizara una comparacion if la cual la su interpretación sería, si no se puede deserializar el file en el documento jsonStting
    // la cual sucede la primera vez por que no existe informacion en file
    if (deserializeJson(jsonDataTemp, dataTemp))
    {                                                                         // si no se puede asi funciona esa libreria (deserilizejson) osea si todo ok manda un 0
        dataGraficasReset();                                                  // se ejecuta esta funcion la cual formatea a los valores de fabrica en este caso a 0
        DeserializationError error = deserializeJson(jsonDataTemp, dataTemp); // arroja un error y es guardado como error de DeserializationError
        myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", "Falló la lectura de las temperaturas, tomando valores por defecto");
        if (error)
        {                                                                              // si hay un error nos indicara el error (por que fallo)
            myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", error.c_str()); // c_str() es un metodo que convierte a String
        }
        return false; // retornamos un valor de false de que no fue posible la deserializacion
    }
    else
    {
        // osea, que si fue posible deserializar de dataTemp el archivo en la memoria ()-> jsonDataTemp
        // asignamos lo que esta en jsonDataTemp a las variable en memorias en tiempo de ejecucion.
        /********************************************************************************
         * Zona recuperacion de temperaturas de dataTemp.json asignando nuevo valores         *
         * a las variables que se encuentran en las variables de vTemp                 *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            // jsonDataTemp[i] = vTemp[i]; //error
            vTemp[i] = jsonDataTemp[i];
        }
    }
    dataTemp.close(); // ya que todo se leera se cierra el archivo
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Lectura de las temperaturas DHT22 OK.");
    Serial.flush();

    JsonDocument jsonDataHum; // capacidad definida en globales.hpp (512B)
    // jsonSettings es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará file
    File dataHum = SPIFFS.open("/dataHum.json", "r"); // modo lectura (r) con el path / que es en raiz y el nombre es settings con la extención json
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    // se realizara una comparacion if la cual la su interpretación sería, si no se puede deserializar el file en el documento jsonStting
    // la cual sucede la primera vez por que no existe informacion en file
    if (deserializeJson(jsonDataHum, dataHum))
    {                                                                       // si no se puede asi funciona esa libreria (deserilizejson) osea si todo ok manda un 0
        dataGraficasReset();                                                // se ejecuta esta funcion la cual formatea a los valores de fabrica en este caso a 0
        DeserializationError error = deserializeJson(jsonDataHum, dataHum); // arroja un error y es guardado como error de DeserializationError
        myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", "Falló la lectura de las Humedades, tomando valores por defecto");
        if (error)
        {                                                                              // si hay un error nos indicara el error (por que fallo)
            myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", error.c_str()); // c_str() es un metodo que convierte a String
        }
        return false; // retornamos un valor de false de que no fue posible la deserializacion
    }
    else
    {
        // osea, que si fue posible deserializar de dataTemp el archivo en la memoria ()-> jsonDataHum
        // asignamos lo que esta en jsonDataHum a las variable en memorias en tiempo de ejecucion.
        /********************************************************************************
         * Zona recuperacion de temperaturas de dataHum.json asignando nuevo valores         *
         * a las variables que se encuentran en las variables de vHum                 *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            // jsonDataHum[i] = vHum[i];
            vHum[i] = jsonDataHum[i];
        }
    }

    dataHum.close(); // ya que todo se leera se cierra el archivo
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Lectura de las Humedades DHT22 OK.");
    Serial.flush();

    JsonDocument jsonDataTempP1; // se crea una variable tipo JSON // capacidad definida en globales.hpp (512B)
    // jsonDataTempP1 es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará file
    File dataTempP1 = SPIFFS.open("/dataTempP1.json", "r"); // modo lectura (r) con el path / que es en raiz y el nombre es settings con la extención json
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    // se realizara una comparacion if la cual la su interpretación sería, si no se puede deserializar el file en el documento jsonStting
    // la cual sucede la primera vez por que no existe informacion en file
    if (deserializeJson(jsonDataTempP1, dataTempP1))
    {                                                                             // si no se puede asi funciona esa libreria (deserilizejson) osea si todo ok manda un 0
        dataGraficasReset();                                                      // se ejecuta esta funcion la cual formatea a los valores de fabrica en este caso a 0
        DeserializationError error = deserializeJson(jsonDataTempP1, dataTempP1); // arroja un error y es guardado como error de DeserializationError
        myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", "Falló la lectura de las temperaturas, tomando valores por defecto");
        if (error)
        {                                                                              // si hay un error nos indicara el error (por que fallo)
            myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", error.c_str()); // c_str() es un metodo que convierte a String
        }
        return false; // retornamos un valor de false de que no fue posible la deserializacion
    }
    else
    {
        // osea, que si fue posible deserializar de dataTemp el archivo en la memoria ()-> jsonDataTempP1
        // asignamos lo que esta en jsonDataTempP1 a las variable en memorias en tiempo de ejecucion.
        /********************************************************************************
         * Zona recuperacion de temperaturas de dataTemp.json asignando nuevo valores         *
         * a las variables que se encuentran en las variables de vSensorPozo1                 *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            // jsonDataTempP1[i] = vSensorPozo1[i]; //error
            vSensorPozo1[i] = jsonDataTempP1[i];
        }
    }
    dataTempP1.close(); // ya que todo se leera se cierra el archivo
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Lectura de las temperaturas Poso1 OK.");
    Serial.flush();

    JsonDocument jsonDataTempP2; // se crea una variable tipo JSON // capacidad definida en globales.hpp (512B)
    // jsonDataTempP2 es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará file
    File dataTempP2 = SPIFFS.open("/dataTempP2.json", "r"); // modo lectura (r) con el path / que es en raiz y el nombre es settings con la extención json
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    // se realizara una comparacion if la cual la su interpretación sería, si no se puede deserializar el file en el documento jsonStting
    // la cual sucede la primera vez por que no existe informacion en file
    if (deserializeJson(jsonDataTempP2, dataTempP2))
    {                                                                             // si no se puede asi funciona esa libreria (deserilizejson) osea si todo ok manda un 0
        dataGraficasReset();                                                      // se ejecuta esta funcion la cual formatea a los valores de fabrica en este caso a 0
        DeserializationError error = deserializeJson(jsonDataTempP2, dataTempP2); // arroja un error y es guardado como error de DeserializationError
        myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", "Falló la lectura de las temperaturas, tomando valores por defecto");
        if (error)
        {                                                                              // si hay un error nos indicara el error (por que fallo)
            myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", error.c_str()); // c_str() es un metodo que convierte a String
        }
        return false; // retornamos un valor de false de que no fue posible la deserializacion
    }
    else
    {
        // osea, que si fue posible deserializar de dataTemp el archivo en la memoria ()-> jsonDataTempP2
        // asignamos lo que esta en jsonDataTempP2 a las variable en memorias en tiempo de ejecucion.
        /********************************************************************************
         * Zona recuperacion de temperaturas de dataTemp.json asignando nuevo valores         *
         * a las variables que se encuentran en las variables de vSensorPozo2                 *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            // jsonDataTempP2[i] = vSensorPozo2[i]; //error
            vSensorPozo2[i] = jsonDataTempP2[i];
        }
    }
    dataTempP2.close(); // ya que todo se leera se cierra el archivo
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Lectura de las temperaturas Poso2 OK.");
    Serial.flush();
    // LM35
    JsonDocument jsonDataTempLm35; // se crea una variable tipo JSON // se crea una variable tipo JSON // capacidad definida en globales.hpp (512B)
    // jsonDataTempP2 es el nombre del documento JSON
    // acontinuacion una variable de tipo File que se llama de la libreria SPIFFS que se llamará file
    File dataTempLm35 = SPIFFS.open("/dataTempLm35.json", "r"); // modo lectura (r) con el path / que es en raiz y el nombre es settings con la extención json
    // mas info sobre SPIFFS https://www.diarioelectronicohoy.com/blog/funcionalidad-de-spiffs
    // se realizara una comparacion if la cual la su interpretación sería, si no se puede deserializar el file en el documento jsonStting
    // la cual sucede la primera vez por que no existe informacion en file
    if (deserializeJson(jsonDataTempLm35, dataTempLm35))
    {                                                                                 // si no se puede asi funciona esa libreria (deserilizejson) osea si todo ok manda un 0
        dataGraficasReset();                                                          // se ejecuta esta funcion la cual formatea a los valores de fabrica en este caso a 0
        DeserializationError error = deserializeJson(jsonDataTempLm35, dataTempLm35); // arroja un error y es guardado como error de DeserializationError
        myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", "Falló la lectura de las temperaturas, tomando valores por defecto");
        if (error)
        {                                                                              // si hay un error nos indicara el error (por que fallo)
            myLog("ERROR", "spiffsGraficas.hhp", "dataGraficasRead()", error.c_str()); // c_str() es un metodo que convierte a String
        }
        return false; // retornamos un valor de false de que no fue posible la deserializacion
    }
    else
    {
        // osea, que si fue posible deserializar de dataTemp el archivo en la memoria ()-> jsonDataTempLm35
        // asignamos lo que esta en jsonDataTempLm35 a las variable en memorias en tiempo de ejecucion.
        /********************************************************************************
         * Zona recuperacion de temperaturas de dataTemp.json asignando nuevo valores         *
         * a las variables que se encuentran en las variables de vSensorPozo2                 *
         *******************************************************************************/
        for (int i = 0; i < NUM_VALORES; i++)
        {
            // jsonDataTempLm35[i] = vSensorPozo2[i]; //error
            vSLm35[i] = jsonDataTempLm35[i];
        }
    }
    dataTempLm35.close(); // ya que todo se leera se cierra el archivo
    myLog("INFO", "spiffsGraficas.hpp", "dataGraficasRead()", "Lectura de las temperaturas LM35 OK.");
    Serial.flush();
    // final
    // mostrarValoresTemp();
    // mostrarValoresHum();
    return true;
}