#pragma once

#include <NTPClient.h>
#include <Arduino.h>

void init_time_client_mutex();
bool start_sys_time();
bool get_sys_time(uint32_t * currentTime);
void close_sys_time();