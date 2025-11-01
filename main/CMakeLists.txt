idf_component_register(
    SRCS 
        "src/main.cpp"
        "src/WiFiConnection.cpp"
        "src/Communication.cpp"
        "src/SimConnection.cpp"
        "src/EthernetConnection.cpp"
        "src/BatteryPacket.cpp"
        "src/IPacket.cpp"
    INCLUDE_DIRS "include"
    REQUIRES 
        nvs_flash
        esp_event
        esp_wifi
        cbor
        littlefs
)

# Declare a compile-time variable for this component only
set(MY_COMPILE_TIME_VAR 123 CACHE STRING "Local compile-time variable")

# Pass it to the compiler
target_compile_definitions(${COMPONENT_LIB} PRIVATE MY_COMPILE_TIME_VAR=${MY_COMPILE_TIME_VAR})
