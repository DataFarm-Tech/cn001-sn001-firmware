#include "init.h"
#include "interrupts.h"
#include <Arduino.h>

void setup() {
  init_p();
}

void loop() {
  process_state_change();
  delay(10);  // Small delay to prevent tight loop
}

void sensor_task(void *pvParameters) {
  while(1) {
    Serial.println("Sensor task running...");
    delay(1000);
  }
}

void controller_task(void *pvParameters) {
  while(1) {
    Serial.println("Controller task running...");
    delay(1000);
  }
}
