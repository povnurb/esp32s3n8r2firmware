#include <HTTPClient.h>

//-----------------------------------------------------------
// Configuración del bot de Telegram
// https://tfmingenieria.com/enviar-mensajes-desde-un-esp32-a-telegram/
//-----------------------------------------------------------
void enviarMensaje(String mensaje)
{
    if (telegram) // si telegram esta activo
    {
        HTTPClient http;
        String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + mensaje;

        http.begin(url);           // Iniciar la conexión HTTP
        int httpCode = http.GET(); // Realizar la solicitud GET

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.print("Mensaje enviado, respuesta del servidor telegram: ");
            Serial.println(payload);
        }
        else
        {
            Serial.print("Error en la solicitud HTTP: ");
            Serial.println(http.errorToString(httpCode).c_str());
        }

        http.end(); // Finalizar la conexión HTTP
    }
}