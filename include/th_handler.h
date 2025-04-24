#ifndef TH_HANDLER_H
#define TH_HANDLER_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t read_serial_cli_th; //thread handler for CLI thread
extern TaskHandle_t process_state_ch_th;
extern TaskHandle_t lora_listener_th;
extern TaskHandle_t main_app_th;
extern TaskHandle_t http_th;


void create_th(TaskFunction_t func, const char* name, TaskHandle_t* th, int core);
void delete_th(TaskHandle_t th);

#endif // TH_HANDLER_H