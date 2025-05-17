#include "rs485_interface.h"
#include <Arduino.h>
#include "utils.h"
#include "pack_def.h"
#include "hw.h"

#define READ_DATA_MSG_LEN 8
/* RS485 Modbus RTU Frame (Read NPK)*/
uint8_t read_data_msg[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08 };

/**
 * @brief Simulates reading data from an RS485 interface.
 * @param buf Pointer to the buffer where the data will be stored.
 * @param buf_len Length of the buffer.
 * @note This function fills the buffer with random data for simulation purposes.
 *       In a real implementation, this function would read data from the RS485 interface.
 */
uint8_t read_rs485(char * data, uint8_t buf_len)
{
    if (buf_len < DATA_SIZE)
    {
        PRINT_ERROR("Buffer length is too small");
        return EXIT_FAILURE;
    }

    bool rs485_disconnect = true;

    for (int i = 0; i < buf_len; i++)
    {
        data[i] = (char)random(0, 255); // Simulate reading a byte from RS485

        if (data[i] != 1)
        {
            rs485_disconnect = false;
        }
    }

    if (rs485_disconnect)
    {
        PRINT_ERROR("All RS485 data values were 1. Invalid response.");
        return SN001_ERR_RSP_CODE_A;
    }

    return EXIT_SUCCESS;
}


void sn001_read_sensor(char * data_to_populate, uint8_t buf_len)
{
    uint8_t bytes_recv = 0;
    uint32_t start_time = millis();

    open_rs485_comms();

    Serial2.write(read_data_msg, READ_DATA_MSG_LEN);
    Serial2.flush();

    close_rs485_comms();

    while (((millis() - start_time) < 1000) && (bytes_recv < DATA_SIZE))
    {
        if (Serial2.available())
        {
            data_to_populate[bytes_recv] = Serial2.read();
            bytes_recv++;
        }
    }

    /** TODO: Check that response is correct with CRC */

    if (bytes_recv != DATA_SIZE)
    {
        /** Bad data from rs485 */
        return;
    }

    


    
    


    
    
    



}