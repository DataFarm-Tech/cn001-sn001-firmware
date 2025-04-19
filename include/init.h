#ifndef INIT_H
#define INIT_H

#include <Arduino.h>
#include "config.h"

typedef enum {
  SENSOR_STATE = 1,
  CONTROLLER_STATE = 2,
} device_state_t;

// Function declarations
void init_p();
void sensor_task(void *pvParameters);
void controller_task(void *pvParameters);

// External variables
extern TaskHandle_t read_serial_cli_th;
extern volatile device_state_t current_state;
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t controllerTaskHandle;

#endif // INIT_H
