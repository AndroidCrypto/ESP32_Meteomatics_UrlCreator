# ESP32 Meteomatics dynamic URL Creator
Using the Meteomatics.com Weather API requires to build a dynamic API URL, and this project is creating the API by getting the current date and time for your local timezone and UTC/GMT timezone.

It is a supplementary project and article for other articles regarding Meteomatics.com to keep them shorter. The final code of this parser is used in my projects "Create an Internet Weather Station with 10 days Forecast on an ESP32 with ST7789 TFT Display (Meteomatics.com)" and "Create an Internet Weather Station with 10 days Forecast on an ESP32-C6 Supermini with an Epaper Display (Meteomatics.com)" (links coming soon).

This is the accompanying repository for my article "How to parse a Meteomatics.com Weather Forecast CSV file with an ESP32" available here: https://medium.com/@androidcrypto/create-your-meteomatics-com-dynamic-api-url-for-your-esp32-weather-station-project-8a19a316edee

## Runtime Environment
````plaintext
Arduino 2.3.6 (Windows)
esp32 boards version 3.2.0
For compiling and uploading, I'm using the "ESP32 Dev Module" board, but all other microcontrollers of the ESP32 familiy should work as well.
This is the Serial Monitor printout of the sample data:
````

## Serial Monitor Logfile
````plaintext
ESP32 Meteomatics.com Build dynamic API URL Step 2 V02

Connecting to: FRITZ!Box 7490
WiFi connected at: 192.168.178.72
Connected to your Wi-Fi
Setting up time
Got the time from NTP
Setting Timezone to CET-1CEST,M3.5.0,M10.5.0/3
Timezone initialized
Thursday, August 21 2025 12:49:54 zone CEST +0200 
Change the Timezone to UTC/GMT
Setting Timezone to GMT0
Thursday, August 21 2025 10:49:54 zone GMT +0000 
current_time:
2025-08-21T10:49:00.000Z
future_time:
2025-08-31T00:00:00.000Z
API URL:
https://api.meteomatics.com/2025-08-21T10:49:00.000Z--2025-08-31T00:00:00.000Z:PT1H/t_2m:C,precip_1h:mm/40.71272810,-74.00601520/csv?model=mix
````
