#include <Arduino.h>
//#include <WiFi.h>
//#include <ctime>

#include "display.hpp"

#define columns_data 23  // (reversed) When True columns off
#define reset 19  // (reversed) When False reset 74HC164D
#define clk 18
const std::array<const byte, 8> pins = {3, 1, 2, 4, 15, 13, 12, 14};  // Row pins
//const std::array<const byte, 8> pins = {14, 12, 13, 15, 4, 2, 1, 3};  // Row pins
#define menu_entry 32
#define menu_left 34
#define menu_right 35

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// updatable text to display
String content;

TaskHandle_t display_task_handle = nullptr;

// loop displaying to matrix display from content

TaskHandle_t update_time_task_handle = nullptr;

// get time from ntp server
void update_time(void *parameter);

void update_time(void *parameter) {
    while (true) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        delay(1000);
        vTaskSuspend(update_time_task_handle);
    }
}

void initWiFi() {
//    WiFiClass::mode(WIFI_STA);
//    WiFi.begin("Wokwi-GUEST");
    Serial.print("Connecting to WiFi ..");
//    while (WiFiClass::status() != WL_CONNECTED) {
//        Serial.print('.');
    delay(200);
//    }
//    Serial.println(WiFi.localIP());
}


Matrix_display display(columns_data, reset, clk, pins, menu_entry, menu_left, menu_right);

void display_loop(void *parameter) {
    while (true) {
        display.static_text("ABCDE");
    }
}

void setup() {

    //    Serial.begin(9600);
//    initWiFi();
//    xTaskCreate(update_time, "update_time", 2048, nullptr, 2, &update_time_task_handle);
    xTaskCreatePinnedToCore(display_loop, "display_loop", 4096, nullptr, 1, &display_task_handle, 0);
}

struct tm timeinfo{};

void loop() {
//    display.pixels_on();
    delay(500);
    display.pixels_off();
    delay(500);

//    if (!getLocalTime(&timeinfo)) {
//        Serial.println("Failed to obtain time");
//        delay(1);
//    }
//    Serial.println(&timeinfo);
//    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//    delay(1000);
}

