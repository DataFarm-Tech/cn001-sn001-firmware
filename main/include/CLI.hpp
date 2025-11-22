#pragma once

/**
 * @class   CLI
 * @brief   Provides a UART-based interactive command-line interface.
 *
 * The CLI class initializes and starts a FreeRTOS task that reads characters
 * from the UART, parses commands, handles backspace, history navigation,
 * and dispatches registered commands to their respective handlers.
 *
 * Typical usage:
 * @code
 *     CLI::start();
 * @endcode
 */
class CLI {
public:
    /**
     * @brief Start the CLI task.
     *
     * Creates and launches the UART CLI FreeRTOS task. Once started, the task
     * continuously reads input from the console, handles editing (backspace,
     * history, arrow keys), and executes commands according to the command
     * table defined in the implementation.
     *
     * This function should be called once during system startup.
     */
    static void start();
};
