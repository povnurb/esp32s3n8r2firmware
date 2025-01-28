// Definir la dirección del I2C

#define BME280_ADDRESS 0x76 // si lo pones sale un warning

// Crear una instancia del BME280
Adafruit_BME280 bme;

float tempbme280()
{

    // Leer la temperatura
    float temperature = bme.readTemperature() + ajTmpDht22;
    Serial.print("Temperatura del BME280: ");
    Serial.print(temperature);
    float min = temperature;

    if (min <= 1)
    {
        min2 == min2;
    }
    else if (min < min2)
    {
        min2 = min;
    }
    device_tempMinima = String(min2);
    float max = temperature;
    if (max > max2)
    {
        max2 = max;
    }
    device_tempMaxima = String(max2);

    return temperature;
}
float humBme280()
{

    // Leer la humedad
    float humidity = bme.readHumidity();
    Serial.print("Humedad del BME280: ");
    Serial.print(humidity);
    Serial.println(" %");
    return humidity;
}

float presionBme280()
{

    // Leer la presión
    float pressure = bme.readPressure() / 100.0F; // Pasar a hPa
    Serial.print("Presión: ");
    Serial.print(pressure);
    Serial.println(" hPa");
    return pressure;
}