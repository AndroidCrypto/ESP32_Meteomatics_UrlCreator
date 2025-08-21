/*
  This sketch builds the dynmic API call for the Weather API of Meteomatics.com to
  receive temperature and precipitation as 10 days 1 hourly forecast.

  This step 2 of the tutorial.

  Sample template URL
  https://api.meteomatics.com/2025-08-17T16:20:00.000+02:00--2025-08-27T00:00:00.000+02:00:PT1H/t_2m:C,precip_1h:mm/40.7127281,-74.0060152/csv?model=mix


  Version Management
  21.08.2025 V02 Tutorial Version
  17.08.2025 V01 Initial programming.
*/

// --------------------------------------------------------------
// Programm Information
const char* PROGRAM_VERSION = "ESP32 Meteomatics.com Build dynamic API URL Step 2 V02";

// --------------------------------------------------------------
// User Credentials

// Wi-Fi
//const char* ssid     = "your_SSID";
//const char* password = "your_PASSWORD";
// Change to your WiFi credentials

// New York, NY, United States of America 40.77783423080643, -73.96993911929277
const double LOCATION_LATITUDE_DOUBLE = 40.7127281;
const double LOCATION_LONGITUDE_DOUBLE = -74.0060152;

// --------------------------------------------------------------
// Global variable ontaining the API URL
char API_URL[180];  // "just a dummy as generated dynamically";

// --------------------------------------------------------------
// Wi-Fi client
#include <WiFi.h>  // Built-in

uint8_t StartWiFi() {
  Serial.println("\r\nConnecting to: " + String(WIFI_SSID));
  IPAddress dns(8, 8, 8, 8);  // Use Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);  // switch off AP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(500);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  } else Serial.println("WiFi connection *** FAILED ***");
  return WiFi.status();
}

// --------------------------------------------------------------
// Timezone
#include "time.h"

// Timezone Information
const char* TIMEZONE    = "CET-1CEST,M3.5.0,M10.5.0/3"; // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv 
                                                        // See below for examples
const char* TIMEZONE_GMT = "GMT0";                      // we need the GMT time for building the Meteomatics call
const char* NTP_SERVER   = "pool.ntp.org";              // Or, choose a time server close to you, but in most cases it's best to use pool.ntp.org to find an NTP server
                                                        // then the NTP system decides e.g. 0.pool.ntp.org, 1.pool.ntp.org as the NTP syem tries to find  the closest available servers
                                                        // EU "0.europe.pool.ntp.org"
                                                        // US "0.north-america.pool.ntp.org"
                                                        // See: https://www.ntppool.org/en/  
const char* NTP_SERVER_2 = "time.nist.gov";    

// variables
String timeString, dateString;  // takes the last update time & date data

// Day of the week
const char* weekday_D[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
// Month
const char* month_M[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 15000)) {  // Wait for 15-sec for time to synchronise
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}

void setTimezone(String timezone) {
  Serial.printf("Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  // Now adjust the TZ. Clock settings are adjusted to show the new local time
  tzset();
}

bool InitTime(String timezone) {
  struct tm timeinfo;
  char time_output[30], day_output[30], update_time[30];
  Serial.println("Setting up time");
  configTime(0, 0, NTP_SERVER, NTP_SERVER_2); // First connect to NTP server, with 0 TZ offset
  if (!getLocalTime(&timeinfo, 15000)) { // Wait for 5-sec for time to synchronise)){
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println("Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
  return true;
}

void createApiUrl() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo, 15000)) {  // Wait for 5-sec for time to synchronise
    Serial.println("Failed to obtain time");
    return;
  }
  
  char current_time[40];
  char future_time[40];
  char timeOutput[30];
  strftime(current_time, sizeof(current_time), "%FT%R:00.000Z", &timeinfo);
  Serial.println("current_time:");
  Serial.println(current_time);
  // adding of days: https://cplusplus.com/reference/ctime/strftime/
  time_t now = time(nullptr);
  struct tm *timeinfoFuture = localtime(&now);
  int daysToAdd = 10;
  timeinfoFuture->tm_mday += daysToAdd;
  // Adjust the date if necessary (e.g., if adding days overflows the month)
  mktime(timeinfoFuture); 
  strftime(future_time, sizeof(future_time), "%FT00:00:00.000Z", timeinfoFuture);
  Serial.println("future_time:");
  Serial.println(future_time);
  sprintf(API_URL, "https://api.meteomatics.com/%s--%s:PT1H/t_2m:C,precip_1h:mm/%1.8f,%1.8f/csv?model=mix", current_time, future_time, LOCATION_LATITUDE_DOUBLE, LOCATION_LONGITUDE_DOUBLE);
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(PROGRAM_VERSION);

  if (StartWiFi() == WL_CONNECTED) {
    Serial.println("Connected to your Wi-Fi");
    if (InitTime(TIMEZONE) == true) {
      Serial.println("Timezone initialized");
      printLocalTime();
      Serial.println("Change the Timezone to UTC/GMT");
      setTimezone(TIMEZONE_GMT);
      printLocalTime();

      // new in step 2:
      createApiUrl();
      Serial.println("API URL:\n");
      Serial.println(API_URL);
    } else {
      Serial.println("Could not initialise the local Timezone");
    }
  } else {
    Serial.println("No connection to your Wi-Fi");
  }
}

void loop() {
}
