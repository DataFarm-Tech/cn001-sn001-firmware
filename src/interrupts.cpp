#include "interrupts.h"
#include "config.h"
#include "Arduino.h"

typedef enum
{
    SENSOR_STATE = 1,
    CONTROLLER_STATE = 2,
} device_state_t;

volatile device_state_t interrupt_state = (device_state_t)0;

/**
 * @brief This function is called when an interrupt occurs on the INT_STATE_PIN or INT_STATE_PIN_2.
 * It checks the state of the device and sets the appropriate state.
 * If the interrupt is triggered on INT_STATE_PIN, it sets the state to SENSOR_STATE.
 * If the interrupt is triggered on INT_STATE_PIN_2, it sets the state to CONTROLLER_STATE.
 * It will print the state to the console.
 */
void IRAM_ATTR switch_state()
{
    Serial.printf("HELLO\n");

    while (1)
    {
        if (digitalRead(INT_STATE_PIN) == HIGH)
        {
            Serial.println("Interrupt on INT_STATE_PIN");
            interrupt_state = SENSOR_STATE;
            Serial.println("Switching to sensor state...");
            break;
        }
        else if (digitalRead(INT_STATE_PIN_2) == HIGH)
        {
            Serial.println("Interrupt on INT_STATE_PIN_2");
            interrupt_state = CONTROLLER_STATE;
            Serial.println("Switching to controller state...");
            break;
        }
        
        interrupt_state = (device_state_t)0; // Reset flag

        // printf("(A1): Interupt error: repeating init state.");
        delay(100);
    }

    
    return;
}