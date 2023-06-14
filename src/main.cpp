#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "esp_log.h"
#include <Arduino.h>
//#include <ctime>
#include <esp_task_wdt.h>

#include "display.hpp"
#include "server_wifi.hpp"
#include "wifi_server.hpp"


#define columns_data 23  // (reversed) When True columns off
#define reset 19  // (reversed) When False reset 74HC164D
#define clk 18
const std::array<const byte, 8> pins = {3, 1, 2, 4, 15, 13, 12, 14};  // Row pins
//const std::array<const byte, 8> pins = {14, 12, 13, 15, 4, 2, 1, 3};  // Row pins
#define menu_entry 32
#define menu_left 34
#define menu_right 35

const char *ntpServer = "pool.ntp.org";
const char *ntpServer2 = "0.sk.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
// Default content
std::string display_content = "XxXxX";

// updatable text to display

// loop displaying to matrix display from content

TaskHandle_t update_time_handle = nullptr;

// get time from ntp server
void update_time(void *parameter);

tm time_info;
bool alarm_enabled = false;
Alarm alarm_info;

void update_time(void *parameter) {
    TickType_t last_wake_time;
    const TickType_t frequency = pdMS_TO_TICKS(10000);
    while (true) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, ntpServer2);
        Serial.println("NTP time configured");
//        getLocalTime(&time_info);
        vTaskDelayUntil(&last_wake_time, frequency);
    }
    vTaskDelete(nullptr);
}

void initWiFi() {
//    WiFiClass::mode(WIFI_STA);
//    WiFi.begin("Wokwi-GUEST");
//    Serial.print("Connecting to WiFi ..");
//    while (WiFiClass::status() != WL_CONNECTED) {
//        Serial.print('.');
//    delay(200);
//    }
//    Serial.println(WiFi.localIP());
}

//WiFi.begin(ssid, password)
//while(WiFi.status() != WL_CONNECTED)
Matrix_display display(columns_data, reset, clk, pins, menu_entry, menu_left, menu_right);

TaskHandle_t t_change = nullptr;

void text_change(void *parameter) {
    while (true) {
//        Serial.println(xPortGetCoreID());

//        display_content = "HI!!!";
//        vTaskDelay(2000 / portTICK_PERIOD_MS);
//        display_content = "Hello";
//        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(nullptr);
}


xTaskHandle network_handle = nullptr;

void network(void *parameter) {
    wifi_init_soft_ap();
    TickType_t start_time = xTaskGetTickCount();
    time_t duration = 3000;
    esp_task_wdt_init(9999, false);
    while (true) {
        esp_task_wdt_reset();
//        if (xTaskGetTickCount() - start_time >= pdMS_TO_TICKS(duration)) {
//            Serial.println("Loop running");
//            start_time = xTaskGetTickCount();
//            Serial.println(start_time);
//        }
        client_loop();
    }
    vTaskDelete(network_handle);
}

void setup() {
    esp_log_level_set("*", ESP_LOG_INFO);
//    ledcDetachPin(25); //turn of the buzzer ???
//    initWiFi();
//    xTaskCreate(update_time, "update_time", 2048, nullptr, 2, &update_time_task_handle);
    Serial.begin(115200);
    // Main display loop initialization
//    xTaskCreatePinnedToCore(network, "network", 4096, nullptr, 4, &network_handle, 1);
//    xTaskCreatePinnedToCore([](void *parameter) {display.display_loop(parameter);},
//                            "display_loop", 16384, &display_content, 3,
//                            &display_handle, 0);

    xTaskCreate(network, "network", 4096, nullptr, 5, &network_handle);
//    xTaskCreatePinnedToCore(network, "network", 4096, nullptr, 7, &network_handle, 1);

    xTaskCreate(update_time, "update_time", 2048, nullptr, 7, &update_time_handle);
    vTaskSuspend(update_time_handle);
//    xTaskCreate(update_time, "update_time", 2048, nullptr, 4, nullptr);
    xTaskCreate([](void *parameter) { display.display_loop(parameter); }, "display_loop", 16384,
                &display_content, 6,
                &display.main_loop_handle);

//    xTaskCreate([](void *parameter) { display.input_check(); }, "input_check", 4096, nullptr, 7, nullptr);

//    display.display_loop(nullptr);
//    xTaskCreatePinnedToCore([](void *parameter) { display.display_loop(parameter); }, "display_loop", 16384, &display_content, 6,
//                &display.main_loop_handle, 0);

//    xTaskCreatePinnedToCore(text_change, "text_change", 2048, &display_content, 8, &t_change, 1);
//    xTaskCreate(text_change, "text_change", 2048, &display_content, 8, &t_change);
//    Serial.println("Tasks init");
}


void loop() {
//    display.display_loop(nullptr);
//    display.static_text(display_content);
//    if (!getLocalTime(&time_info)) {
//        Serial.println("Failed to obtain time");
//        delay(1);
//    }
//    Serial.println(&timeinfo);
//    Serial.println(&time_info, "%A, %B %d %Y %H:%M:%S");
}

