#include "interrupts.h"
#include "init.h"
#include <Arduino.h>

volatile unsigned long last_interrupt_time = 0;
volatile bool state_change_detected = false;

/**
 * @brief Interrupt service routine for state change detection
 * Keeps processing minimal to ensure quick execution
 */
void IRAM_ATTR has_state_changed() {
  // Simple debouncing to prevent multiple triggers
  if ((millis() - last_interrupt_time) > 50) {
    state_change_detected = true;
  }
  last_interrupt_time = millis();
}

/**
 * @brief Process state changes based on pin conditions
 * This is called from the main loop to handle actual state transitions
 */
void process_state_change(void *param) {
  while (1)
  {
    if(state_change_detected)
    {
      const int sensor_pin = digitalRead(INT_STATE_PIN);
      const int controller_pin = digitalRead(INT_STATE_PIN_2);
      
      if(sensor_pin == LOW && controller_pin == HIGH) 
      {
        switch_to_sensor_state();
      } 
      else 
      {
        switch_to_controller_state();
      }
      
      state_change_detected = false;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Add this line to yield every 10ms
  }

  vTaskDelete(NULL); // Delete the task when done
  
}

// State transition functions
void switch_to_sensor_state() {
  if(current_state == SENSOR_STATE) return;
  printf("Transitioning to sensor state\n");
  current_state = SENSOR_STATE;
}

void switch_to_controller_state() {
  if(current_state == CONTROLLER_STATE) return;
  printf("Transitioning to controller state\n");
  current_state = CONTROLLER_STATE;
}
