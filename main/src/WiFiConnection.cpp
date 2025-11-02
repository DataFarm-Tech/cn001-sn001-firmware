#include "comm/WiFiConnection.hpp"
#include "esp_wifi.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "esp_netif_ip_addr.h"

#define WIFI_SSID "NETGEAR77"
#define WIFI_PASS "aquaticcarrot628"


void WifiConnection::wifi_event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void * event_data) 
{
    WifiConnection * self = static_cast<WifiConnection*>(arg);

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        xEventGroupSetBits(self->wifi_event_group, BIT0);
    }
}

bool WifiConnection::connect() 
{
    esp_netif_t * netif = nullptr;
    esp_netif_ip_info_t ip_info;
    esp_netif_dns_info_t dns;
    wifi_init_config_t cfg;
    wifi_config_t wifi_config = {};
    EventBits_t bits;
    
    wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    esp_event_loop_create_default();
    netif = esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_netif_dhcpc_stop(netif)); // Stop DHCP client before assigning static IP

    ip_info.ip.addr = esp_ip4addr_aton("192.168.1.123");
    ip_info.gw.addr = esp_ip4addr_aton("192.168.1.1");
    ip_info.netmask.addr = esp_ip4addr_aton("255.255.255.0");
    ESP_ERROR_CHECK(esp_netif_set_ip_info(netif, &ip_info));

    dns.ip.u_addr.ip4.addr = esp_ip4addr_aton("8.8.8.8");
    dns.ip.type = ESP_IPADDR_TYPE_V4;
    ESP_ERROR_CHECK(esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns));

    cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiConnection::wifi_event_handler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiConnection::wifi_event_handler, this);

    strncpy((char*)wifi_config.sta.ssid, WIFI_SSID, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, WIFI_PASS, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    bits = xEventGroupWaitBits(wifi_event_group, BIT0, pdFALSE, pdTRUE, pdMS_TO_TICKS(10000));
    return bits & BIT0;
}

bool WifiConnection::isConnected() 
{
    wifi_ap_record_t info;
    return esp_wifi_sta_get_ap_info(&info) == ESP_OK;
}


void WifiConnection::disconnect() 
{
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
}