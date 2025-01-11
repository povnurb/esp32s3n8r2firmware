
#include <HTTPClient.h>
#include <UrlEncode.h>

// https://www.electronicwings.com/esp32/send-a-whatsapp-message-using-esp32
// https://www.callmebot.com/blog/free-api-whatsapp-messages/
// https://microcontrollerslab.com/esp32-send-whatsapp-messages/

void enviarMensajeWhatsapp(String message)
{
    if (whatsapp)
    {
        String url = "https://api.callmebot.com/whatsapp.php?phone=" + MobileNumber + "&apikey=" + APIKey + "&text=" + urlEncode(message);
        HTTPClient http;
        http.begin(url);

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(url);
        if (httpResponseCode == 200)
        {
            Serial.print("Mensaje enviado por WhatsApp");
        }
        else
        {
            Serial.println("Error sending the message");
            Serial.print("HTTP response code: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
}