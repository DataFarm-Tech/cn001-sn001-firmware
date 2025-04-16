# RFM95W Pinout

The change in chip type will effect the available pins to use, meaning there will
be different pin outs for different chip types.

### ESP32
| From Device | From Pin          | To Device   | To Pin         |
| ----------- | ----------------- | ----------- | -------------- |
| ESP32       | GPIO 5            | RFM95W      | NSS/CS         |
| ESP32       | GPIO 23           | RFM95W      | MOSI           |
| ESP32       | GPIO 19           | RFM95W      | MISO           |
| ESP32       | GPIO 18           | RFM95W      | SCK            |
| ESP32       | GPIO 14           | RFM95W      | RST            |
| ESP32       | GPIO 2            | RFM95W      | DIO0/IRQ       |
| ESP32       | 3.3V              | RFM95W      | VCC            |
| ESP32       | GND               | RFM95W      | GND            |
#

### ESP32-S3 
TODO
#