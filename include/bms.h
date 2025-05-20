#pragma once

typedef enum
{
    BAT_CHARGE_UNDEFINED = 0,
    BAT_CHARGE_FULL = 1,        // > 75%
    BAT_CHARGE_HIGH = 2,        // 50%–75%
    BAT_CHARGE_MEDIUM = 3,      // 25%–50%
    BAT_CHARGE_LOW = 4,         // 3%–25%
    BAT_CHARGE_CRITICAL = 5     // < 3%
} bat_charge_state_t;

extern volatile bat_charge_state_t battery_state;

void set_battery_state();