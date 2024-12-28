// seteamos el time
// FIXME: timeSetup(); //funciones.hpp
// wire.begin(SDA, SCL). 4 y 5

void relojSetup()
{
    if (!rtc.begin())
    {
        rtcOk = false;
        myLog("INFO", "reloj.hpp", "relojSetup", "No se encontro el RTC (ds3231) o hay un problema en la conexión, verifica y reiniciar");
        rtc.adjust(DateTime("Nov 16 2024", "08:31:08"));
    }
    else
    {
        rtcOk = true;
        // rtc.adjust(DateTime(__DATE__, __TIME__)); // entrega la hora de la PC al dispositivo (ds3231)
        myLog("INFO", "main.cpp", "rtc.adjust()", "RTC ENCONTRADO FECHA:");
        Serial.println(getDateTime());
    }
}
