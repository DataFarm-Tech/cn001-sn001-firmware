#pragma once

#include <Arduino.h>

void calc_crc(uint8_t *buffer, size_t length);
uint16_t calc_crc_16_ccitt_false(uint8_t *data, size_t length);
