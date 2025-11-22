#pragma once
#include "driver/uart.h"
#include <string>

/**
 * @class   UARTConsole
 * @brief   Lightweight UART wrapper for console I/O.
 *
 * This class provides simple formatted printing and byte-level input handling
 * over the ESP-IDF UART driver. It is designed to be used by the CLI system
 * or any component that requires basic UART console functionality.
 *
 * The console must be initialized with `init()` before any read or write
 * operations are performed.
 */
class UARTConsole {
public:
    /**
     * @brief Initialize the UART console.
     *
     * Configures the selected UART (typically UART0) for console communication
     * using the specified baud rate. This function sets up RX/TX pins,
     * buffer sizes, UART parameters (8N1), and installs the UART driver.
     *
     * @param baud Baud rate (e.g., 115200)
     */
    static void init(int baud);

    /**
     * @brief Write a null-terminated string to the UART.
     *
     * Sends the provided text exactly as-is (no formatting), blocking until
     * the data has been queued for transmission.
     *
     * @param text C-string to write.
     */
    static void write(const char* text);

    /**
     * @brief Write formatted text to the UART.
     *
     * Works like `printf()`, supporting full `printf` formatting options.
     * Data is formatted into an internal buffer before being written.
     *
     * @param fmt Format string.
     * @param ... Additional arguments for the format string.
     */
    static void writef(const char* fmt, ...);

    /**
     * @brief Read a single byte from the UART (non-blocking).
     *
     * Attempts to read 1 byte from the UART RX buffer.  
     * If a byte is available, it is written into `out` and the function
     * returns `1`.  
     * If no data is available, the function returns `0`.  
     * A negative return value indicates a UART driver error.
     *
     * @param out Reference where the read byte will be stored.
     * @return Number of bytes read:  
     *         - `1` = byte read  
     *         - `0` = no data  
     *         - `<0` = error
     */
    static int readByte(uint8_t &out);
};
