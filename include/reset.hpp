#include <Arduino.h>
/***
 * Interrupciones de hardware e interrupciones de software.
 * Las interrupciones por hardware ocurren en respuesta de un evento externo. Por ejemplo:
 * Interrupcion GPIO (cuando se presiona un boton) o Interrupcion táctil (cuando se detecta un toque).
 *
 * Las Interrupciones de Software ocurren en respuesta a una instruccion de software.
 * Por ejemplo, una interrupción de temporizacion simple o interrupcion de vigilancia
 */
// ---------------------------------------------------------------
// Estructura Restore PIN
// ----------------------------------------------------------------
#define TREST 10 // tiempo que pasara para restaurar a fabricar
struct Restore   // como una estancia
{
    uint8_t PIN;
    bool active;
};
Restore reset_pin; // objeto reset_pin tiene la estructuta Restore

int pin_active = 0;

// ---------------------------------------------------------
// IRAM_ATTR In1
// ---------------------------------------------------------
void IRAM_ATTR int_restore() // esta funcion se mete a la memoria IRAM_ATTR que es mas rapida necesaria para una interrupcion
{
    reset_pin.active = true; // cuando se activa la funcion int_restore()el valor  del atributo active cambia true
}

// ---------------------------------------------------------
// setup del PIN
// ---------------------------------------------------------
void setupPintRestore()
{
    reset_pin = {BTNRST, false};
    pinMode(reset_pin.PIN, INPUT_PULLUP);
    attachInterrupt(reset_pin.PIN, int_restore, FALLING); // interrupcion de Alto a bajo
    myLog("INFO", "reset.hpp", "setupPintRestore()", "Configuración del PINREST");
}
/* Tipos de Interrupciones
LOW     Las interrupciones se dan cuando el pin está BAJO
HIGH    Las interrupciones de dan cuando el pin está ALTO
CHANGE  El pin cambia de valor ya sea de ALTO a BAJO o de BAJO a ALTO
FALLING El pin pasa de ALTO a BAJO
RISING  El pin para de BAJO a ALTO
*/

byte conteo = 0;
void resetIntLoop()
{
    if (reset_pin.active)
    {
        static uint32_t lastMillis = 0;
        // incrementar conteo en 1 cada segundo
        if (millis() - lastMillis > 1000)
        {
            lastMillis = millis();
            conteo++;
            Serial.println(conteo);
            //---------------------------------------
            OLED.clearDisplay();
            OLED.setTextSize(1);
            OLED.setTextColor(WHITE);
            OLED.setCursor(0, 0);
            OLED.println("RESTAURANDO EN:");
            OLED.setTextSize(2);
            OLED.println(TREST - conteo);
            OLED.display();

            //---------------------------------------
        }
        pin_active = digitalRead(reset_pin.PIN); // En la interrupcion capturatmos el estado de ese PIN
        if (pin_active == LOW && conteo == TREST)
        {
            // reseteo a Fabrica
            myLog("INFO", "functions.hpp", "resetIntLoop()", "Borrando datos de temperatura y humedad");
            dataGraficasReset();
            settingsReset(); // verificar por que no lo pone en modo ap
            if (settingsSave())
            {
                conteo = 0;
                reset_pin.active = false;
                myLog("INFO", "reset.hpp", "resetIntLoop()", "Todos los datos se establecen de fábrica por interrupción");
                OLED.clearDisplay();
                OLED.setTextSize(2);
                OLED.setTextColor(WHITE);
                OLED.setCursor(0, 0);
                OLED.println("Restaurado");
                OLED.display();
                Serial.flush();
                ESP.restart();
            }
            else
            {
                Serial.println("errror en salvar los cambios de RESTAURACION");
            }
        }
        else if (pin_active == HIGH && conteo > 1)
        {
            // llamada a reiniciar
            conteo = 0;
            reset_pin.active = false;
            myLog("INFO", "reset.hpp", "resetIntLoop()", "Restauracion cancelada");
            Serial.flush();
            ESP.restart();
        }
    }
}