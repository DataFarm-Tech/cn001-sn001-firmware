
#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

//SIZES
#define ADDRESS_SIZE 6
#define LORA_MSG_LEN 21
#define MAX_NODES_LIST 10

typedef struct {
    uint8_t rs485_humidity;
    uint8_t rs485_temp;
    uint8_t rs485_con;
    uint8_t rs485_ph;
    uint8_t rs485_nit;
    uint8_t rs485_phos;
    uint8_t rs485_pot;
} rs485_data;

typedef struct {
    String src_node;
    String des_node;
    rs485_data data;    
} msg;
    

#endif //TYPES_H