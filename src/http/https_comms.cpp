#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Update.h>

#include "config.h"
#include "https_comms.h"
#include "msg_queue.h"
#include "utils.h"
#include "th/th_handler.h"

/******************* Hash Defines *****************/

/* HTTP / Controller HTTP Error Codes*/
#define HTTP_200_OK 200
#define HTTP_201_CREATED 201
#define HOST_FAILED_PING -1
#define RSSI_LOW_STRENGTH -2

/******************* Function Prototypes *****************/
int post_request(String jsonPayload);
void http_send(void* parameter);
void send_data(msg message);
void checkInternet();
char* constr_endp(const char* endpoint);

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
    /**
     * This portion of code doesnt make any sense
     * But it still works??
     * Calling the delete_th function removes it, but doesnt 
     * show the removal in the threads cmd.
     * Will find a work around.
     */
    // http_th = NULL;
    // vTaskDelete(NULL);

    msg cur_msg;
    //check internet connection
    // add_msg_to_queue();

    // display the core on which the thread is running
    int core = xPortGetCoreID();
    // Serial.print("HTTP thread is running on Core ");
    Serial.println(core);

    while(1) 
    {
        // Lock the mutex before accessing the queue
        if (xSemaphoreTake(msg_queue_mh, portMAX_DELAY) == pdTRUE) 
        {
            if (!internal_msg_q.empty())
        {
            cur_msg = internal_msg_q.front();
            internal_msg_q.pop();
            // Unlock the mutex after accessing the queue
            xSemaphoreGive(msg_queue_mh); // Release the mutex

            // PRINT_STR("executing api post request");
            // printf("\tsrc_node: %s    des_node: %s\n", cur_msg.src_node, cur_msg.des_node);

            node_id = cur_msg.src_node;
            send_data(cur_msg);
        }
        else 
        {
            // If queue was empty, unlock the mutex and wait
            xSemaphoreGive(msg_queue_mh); // Release the mutex
            Serial.println("Queue is empty, waiting...");
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
        }

        
    }

    vTaskDelete(NULL);
}

/**
 * @brief This function sends the data to the controller API using the POST method.
 * @param message - The data to be sent to the controller API
 * @return None
 */
void send_data(msg message) {
    // Create a JSON document
    JsonDocument doc; // Adjust size as needed
  
    // Add node_id (src_node)
    doc["node_id"] = message.src_node;
  
    // Create the data array
    JsonArray data = doc["data"].to<JsonArray>();
  
    // Helper function to add sensor data objects
    auto addSensorData = [&data](const char* type, float value) {
        JsonObject obj = data.add<JsonObject>();
        obj["type"] = type;
        obj["level1"] = value;
        obj["level2"] = 0.0f; // Using float literal
        obj["level3"] = 0.0f;
    };
  
    // Add all sensor data
    addSensorData("moisture", message.data.rs485_humidity / 1.0f);
    addSensorData("ph", message.data.rs485_ph / 1.0f);
    addSensorData("temperature", message.data.rs485_temp / 1.0f);
    addSensorData("conductivity", message.data.rs485_con / 1.0f);
    addSensorData("nitrogen", message.data.rs485_nit / 1.0f);
    addSensorData("phosphorus", message.data.rs485_phos / 1.0f);
    addSensorData("potassium", message.data.rs485_pot / 1.0f);
  
    // Serialize JSON to string
    String jsonStr;
    serializeJson(doc, jsonStr);

    // Debug output
    // Serial.println("Sending JSON:");
    // Serial.println(jsonStr);
  
    // Send the POST request with the complete JSON
    int success = post_request(jsonStr); // You'll need to modify your post_request function
    
    while (success == -1 || success == -2) {
      PRINT_STR("POST request failed");
      // Check if the host is reachable
      checkInternet();
      success = post_request(jsonStr);
    }
    PRINT_STR("POST request sent successfully");
  }


/**
 * @brief This function initializes the HTTP client with the given URL and adds the necessary headers.
 * @param url - The URL to which the client will connect
 * @return int - EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int init_http_client(const String& url)
{   
    //TODO: check if url is NULL and handle it properly
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
 * @param jsonPayload - The json payload  to be sent
 * @return int - The HTTP status code of the request
 */
int post_request(String jsonPayload)
{
    char * url;
    int http_code;
    String json_payload;

    url = constr_endp(TX_POST_ENDPOINT); 

    //TODO: check if url is NULL and handle it properly
    if (url == NULL)
    {
        PRINT_STR("URL is NULL");
        DEBUG();
        return HOST_FAILED_PING;
    }
    
    init_http_client(url);

    // printf("URL: %s\n", url);
    // Check if the client is connected to the host
    
    // Send the POST request with the JSON payload & close client connection.
    // http_code = client.POST(json_payload);
    http_code = client.POST(jsonPayload);

    //mock http_code for testing purposes
    // http_code = HTTP_201_CREATED; //TODO: remove this line when testing is done
    // Serial.print("POSTING FROM NODE:");
    // Serial.print(" " +node_id);
    // PRINT_STR(json_payload);
    client.end();

    return http_code;
}



void checkInternet() {
    const char* googleDNS = "google.com"; // Google DNS for internet check

    // 1. Loop until WiFi connects
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.reconnect();
        delay(3000); // Prevents watchdog trigger
    }

    // 2. Loop until Google pings (confirms internet)
    while (true) {
        Serial.println("Pinging Google...");
        if (Ping.ping(googleDNS, 5)) {
            break;
        }
        Serial.println("Google ping failed. Retrying in 3s...");
        delay(3000);
    }

    // 3. Loop until your server pings
    while (true) {
        Serial.println("Pinging df server server...");
        if (Ping.ping(API_HOST, 5)) {
            Serial.println("Server ping successful.");
            break;
        }
        Serial.println("Server ping failed. Retrying in 3s...");
        delay(3000);
    }
}

/**
 * @brief Constructs the endpoint URL.
 * @param endpoint The endpoint to be appended to the URL.
 * @return char* - The constructed endpoint.
 */
char* constr_endp(const char* endpoint)
{
    static char endp[200];  // Static buffer to persist after function returns

    if (strncmp(endpoint, "", sizeof(endpoint)) == 0)
    {
        PRINT_STR("Endpoint is empty");
        return NULL;
    }
    
    snprintf(endp, sizeof(endp), "http://%s:%s%s", API_HOST, API_PORT, endpoint);
    return endp;
}
