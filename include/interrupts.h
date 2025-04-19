#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <Arduino.h>

void switch_state() IRAM_ATTR;
void process_state_change();
void switch_to_sensor_state();
void switch_to_controller_state();

#endif // INTERRUPTS_H
