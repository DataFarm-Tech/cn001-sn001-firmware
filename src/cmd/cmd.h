#pragma once

#include <Arduino.h>

typedef enum {
    CMD_HELP,
    CMD_EXIT,
    CMD_REBOOT,
    CMD_QUEUE,
    CMD_PING,
    CMD_CLEAR,
    CMD_THREADS,
    CMD_TIME,
    CMD_UNKNOWN
} cli_cmd;

void cmd_help();
void cmd_exit();
void cmd_reboot();
void cmd_queue();
void cmd_ping(const char* host);
void cmd_clear();
void cmd_threads();
void cmd_time();