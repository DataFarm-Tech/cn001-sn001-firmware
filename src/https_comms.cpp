#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Update.h>

#include "config.h"
#include "https_comms.h"
#include "queue_m.h"
#include "utils.h"

/******************* Hash Defines *****************/

/* HTTP / Controller HTTP Error Codes*/
#define HTTP_200_OK 200
#define HTTP_201_CREATED 201
#define HOST_FAILED_PING -1
#define RSSI_LOW_STRENGTH -2

/******************* Function Prototypes *****************/
int post_request(const String& type, uint8_t data);
void http_send(void* parameter);
void send_data(rs485_data data);

/******************* Globals *****************/
HTTPClient client;
String node_id;

const char* labels[] = {
    "moisture", 
    "temperature", 
    "conductivity", 
    "nitrogen", 
    "potassium", 
    "phosphorus", 
    "ph"
};

/******************* Function Definitions *****************/
/**
 * @brief This function is the entry point for the HTTP send thread. It sends the data to the controller API.
 * @param parameter - The parameter passed to the task
 * @return None
 */
void http_send(void* parameter)
{
    msg cur_msg;

    while (1){
        if (!internal_msg_q.empty()){ //TODO: Add mutex
            

        }
        int core = xPortGetCoreID();
        Serial.print("HTTP thread is running on Core ");
        Serial.println(core);

        sleep(3);

    }
    // while(1) 
    // {
    //     if (!internal_msg_q.empty()) //TODO: Add mutex to prevent race conditions with lora_listener
    //     {
    //         cur_msg = internal_msg_q.front();
    //         PRINT_STR("executing api post request");
    //         printf("\tsrc_node: %s    des_node: %s\n", cur_msg.src_node, cur_msg.des_node);
    //         node_id = cur_msg.src_node;
    //         send_data(cur_msg.data);
    //         internal_msg_q.pop();
    //     }
        
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }

    vTaskDelete(NULL);
}

/**
 * @brief This function sends the data to the controller API using the POST method.
 * @param data - The data to be sent to the controller API
 * @return None
 */
void send_data(rs485_data data)
{
    // Array of labels and corresponding data values
    int data_values[] = {data.rs485_humidity, data.rs485_temp, data.rs485_con, data.rs485_nit, data.rs485_pot, data.rs485_phos, data.rs485_ph};
    int success;

    // Loop through the labels and data values
    for (int i = 0; i < sizeof(data_values); i++) 
    {
        success = -1;

        while (success == -1 || success == -2)
        {
            success = post_request(labels[i], data_values[i]);
            PRINT_STR("POST request failed");
        }
        PRINT_STR("POST request sent");
        
        delay(1000);
    }
}


/**
 * @brief This function initializes the HTTP client with the given URL and adds the necessary headers.
 * @param url - The URL to which the client will connect
 * @return int - EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int init_http_client(const String& url)
{
    if (strncmp(API_KEY, "", sizeof(API_KEY)) == 0)
    {
        PRINT_STR("API key not initialized");
        DEBUG();
        return EXIT_FAILURE; //TODO: replace to hash define exit  failure
    }
    
    client.begin(url);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("access_token", API_KEY);

    return EXIT_SUCCESS;
}


/**
 * @brief This function sends a POST request to the controller API with the given type and data.
 * @param type - The type of data being sent
 * @param data - The data value to be sent
 * @return int - The HTTP status code of the request
 */
int post_request(const String& type, uint8_t data)
{
    char * url;
    int http_code;
    String json_payload;

    url = constr_endp(TX_POST_ENDPOINT);

    if (url == NULL)
    {
        PRINT_STR("URL is NULL");
        DEBUG();
        return HOST_FAILED_PING;
    }
    
    init_http_client(url);

    json_payload = "{\"node_id\":\"" + node_id + "\","
                          "\"level1\":\"" + data + "\","
                          "\"level2\":\"" + data + "\","
                          "\"level3\":\"" + data + "\","
                          "\"type\":\"" + type + "\"}";

    // Send the POST request with the JSON payload & close client connection.
    http_code = client.POST(json_payload);
    client.end();

    return http_code;
}
