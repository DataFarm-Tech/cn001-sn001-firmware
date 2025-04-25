#include <NTPClient.h>       // NTPClient library for getting time
#include <WiFiUdp.h>         // UDP library for communication

#include "ntp.h"

#define TIMEZONE_OFFSET 0 //UTC timezone
#define UPDATE_INTERVAL 60000
#define NTP_SERVER_POOL "pool.ntp.org"
#define TIMEOUT_MS 60000

WiFiUDP udp;

NTPClient time_client(udp, NTP_SERVER_POOL, TIMEZONE_OFFSET, UPDATE_INTERVAL);  // define and init ntp client