#include <Arduino.h>

#include "init.h"
#include "interrupts.h"
#include "queue_m.h"


std::queue<msg> internal_msg_q;  // Define the msg queue here

void setup() {
  init_p();
}

void loop() {
  //Do nothing
}