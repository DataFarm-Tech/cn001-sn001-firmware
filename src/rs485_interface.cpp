#include "rs485_interface.h"
#include <Arduino.h>
#include "utils.h"
#include "pack_def.h"
#include "hw.h"

/**
 * @brief Simulates reading data from an RS485 interface.
 * @param buf Pointer to the buffer where the data will be stored.
 * @param buf_len Length of the buffer.
 * @note This function fills the buffer with random data for simulation purposes.
 *       In a real implementation, this function would read data from the RS485 interface.
 * @return EXIT_SUCCESS, SN001_ERR_RSP_CODE_A, SN001_ERR_RSP_CODE_B
 */
uint8_t read_sensor(char data[DATA_SIZE])
{
    bool rs485_disconnect = true;

    for (size_t i = 0; i < DATA_SIZE; i++)
    {
        data[i] = (char)random(0, 255); // Simulate reading a byte from RS485

        if (data[i] != 1)
        {
            rs485_disconnect = false;
        }
    }

    /** TODO: check lower and upper bounds of data. If it
     * is outside that range return SN001_ERR_RSP_CODE_B.
     */
    if (rs485_disconnect)
    {
        PRINT_ERROR("All RS485 data values were 1. Invalid response.");
        return SN001_ERR_RSP_CODE_A;
    }

    return EXIT_SUCCESS;
}