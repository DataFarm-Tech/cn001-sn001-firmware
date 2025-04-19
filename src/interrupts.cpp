#include "interrupts.h"
#include "init.h"
#include <Arduino.h>

volatile unsigned long last_interrupt_time = 0;
volatile bool state_change_detected = false;

/**
 * @brief Interrupt service routine for state change detection
 * Keeps processing minimal to ensure quick execution
 */
void IRAM_ATTR switch_state() {
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
void process_state_change() {
  if(!state_change_detected) return;
  
  // Check pin states to determine mode
  const int sensor_pin = digitalRead(INT_STATE_PIN);
  const int controller_pin = digitalRead(INT_STATE_PIN_2);
  
  // Logic as requested: Sensor mode when pin 32 LOW & 33 HIGH
  // Controller mode for all other combinations (including both LOW)
  if(sensor_pin == LOW && controller_pin == HIGH) {
    switch_to_sensor_state();
  } else {
    // All other combinations default to controller mode
    switch_to_controller_state();
  }
  
  state_change_detected = false;
}

// State transition functions
void switch_to_sensor_state() {
  if(current_state == SENSOR_STATE) return;
  
  Serial.println("Transitioning to sensor state");
  
  // Stop controller task
  if(controllerTaskHandle != NULL) {
    vTaskDelete(controllerTaskHandle);
    controllerTaskHandle = NULL;
    Serial.println("Controller task stopped");
  }
  
  // Start sensor task
  if(sensorTaskHandle == NULL) {
    xTaskCreate(sensor_task, "Sensor", 2048, NULL, 1, &sensorTaskHandle);
    Serial.println("Sensor task started");
  }
  
  current_state = SENSOR_STATE;
}

void switch_to_controller_state() {
  if(current_state == CONTROLLER_STATE) return;
  
  Serial.println("Transitioning to controller state");
  
  // Stop sensor task
  if(sensorTaskHandle != NULL) {
    vTaskDelete(sensorTaskHandle);
    sensorTaskHandle = NULL;
    Serial.println("Sensor task stopped");
  }
  
  // Start controller task
  if(controllerTaskHandle == NULL) {
    xTaskCreate(controller_task, "Controller", 2048, NULL, 1, &controllerTaskHandle);
    Serial.println("Controller task started");
  }
  
  current_state = CONTROLLER_STATE;
}
