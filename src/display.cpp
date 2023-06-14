//
// Created by japru on 11/03/2023.
//
#include "display.hpp"
#include <Arduino.h>
#include <esp_task_wdt.h>

volatile int num = random(100);

Matrix_display::Matrix_display(uint8_t columns_data, uint8_t reset, uint8_t clk,
                               const std::array<const uint8_t, 8> pins,
                               uint8_t menu_entry, uint8_t menu_left, uint8_t menu_right) : pins(pins) {
    this->columns_data = columns_data;
    this->reset = reset;
    this->clk = clk;
    this->menu_entry = menu_entry;
    this->menu_left = menu_left;
    this->menu_right = menu_right;
    // Display pins
    for (uint8_t pin: (this->pins)) {
        pinMode(pin, 0x03);
    }
    // Management pins
    pinMode(columns_data, 3);
    pinMode(reset, 3);
    digitalWrite(reset, 1);
    pinMode(clk, 3);

    // Button pins
    pinMode(menu_entry, 5);
    pinMode(menu_left, 5);
    pinMode(menu_right, 5);
}

void Matrix_display::tick_column() const {
    digitalWrite(clk, 1);
    delayMicroseconds(1);
//    vTaskDelay(1/portTICK_PERIOD_MS);
    digitalWrite(clk, 0);
}

void Matrix_display::pixels_on() const {
    digitalWrite(reset, 0);
    delayMicroseconds(1);
//    vTaskDelay(1/portTICK_PERIOD_MS);
    digitalWrite(reset, 1);
    for (uint8_t pin: pins) {
        digitalWrite(pin, 1);
    }
}

void Matrix_display::pixels_off() const {
    digitalWrite(columns_data, 1);
    for (uint8_t i = 0; i < COLUMNS; ++i) {
        tick_column();
    }
    for (uint8_t pin: pins) {
        digitalWrite(pin, 0);
    }
}

void Matrix_display::reset_rows() const {
    for (uint8_t pin: pins) {
        digitalWrite(pin, 0);
    }
}

void Matrix_display::input_check() {
    while (true) {
        if (!digitalRead(menu_entry)) {
            vTaskSuspend(main_loop_handle);
            int network_count = WiFi.scanNetworks();
            static_loop(std::to_string(network_count), 1000);
            while (network_count == 0) {
                static_loop("nNETf", 1000); // No network found
                network_count = WiFi.scanNetworks();
                if (!digitalRead(menu_entry)) {
                    vTaskResume(main_loop_handle);
                    break;
                }
            }
            String wifi_ssids[network_count];
            for (int i = 0; i < network_count; i++) {
                wifi_ssids[i] = WiFi.SSID(i).substring(0, 5);
            }
            int display_index = 0;
            delay(100);
            while (true) {
                static_text(wifi_ssids[display_index].c_str());
                if (!digitalRead(menu_right)) {
                    if (display_index == network_count - 1) {
                        display_index = 0;
                    } else {
                        display_index++;
                    }
                    delay(200);
                } else if (!digitalRead(menu_left)) {
                    if (display_index == 0) {
                        display_index = network_count;
                    } else {
                        display_index--;
                    }
                    delay(200);
                } else if (!digitalRead(menu_entry)) {
                    delay(100);
                    if (WiFi.encryptionType(display_index) == 0) {
                        WiFi.begin(WiFi.SSID(display_index).c_str());
                        delay(1000);
                        if (WiFiClass::status() == WL_CONNECTED) {
                            static_loop("CONsc", 2000);
                            vTaskResume(main_loop_handle);
                            break;
                        } else {
                            static_loop("CONun", 2000);
                            vTaskResume(main_loop_handle);
                            break;
                        }
                    } else {
                        // TODO: password enter
                        byte password[63] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                        byte left_index = 0, changing_pos = 0;
                        bool moving = false;
                        while (true) {
                            byte temp[5] = {password[left_index], password[left_index + 1], password[left_index + 2],
                                            password[left_index + 3], password[left_index + 4]};
//                            static_text_byte(temp);
                            static_loop_byte(temp, 100);
                            if (!digitalRead(menu_right) && !digitalRead(menu_left)) {
                                static_loop("PASSe", 750);
                                break;
                            } else if (!digitalRead(menu_right)) {
                                if (password[changing_pos] == 125) {
                                    password[changing_pos] = 0;
                                } else if (password[changing_pos] == 0) {
                                    password[changing_pos] = 32;
                                } else {
                                    password[changing_pos]++;
                                }
                                delay(10);
                                moving = true;
                            } else if (!digitalRead(menu_left)) {
                                if (password[changing_pos] == 0) {
                                    password[changing_pos] = 125;
                                } else if (password[changing_pos] == 32) {
                                    password[changing_pos] = 0;
                                } else {
                                    password[changing_pos]--;
                                }
                                delay(10);
                                moving = true;
                            } else if (!digitalRead(menu_entry)) {
                                if (password[changing_pos] != 0) {
                                    left_index++;
                                    changing_pos++;
                                }
                            }
                            if (!moving) {
                                temp[0] = 124; // |
                                static_loop_byte(temp, 750);
                                moving = false;
                            }
                        }
                        String pass_phrase;
                        bool password_status = true;
                        for (int i = 0; i < 63; ++i) {
                            if (i <= 8 && password[i] == 0) {
                                password_status = false;
                                break;
                            } else if (password[i] != 0) {
                                pass_phrase += (char) password[i];
                            }
                        }
//                        pass_phrase = "01234567";
//                        Serial.println(pass_phrase);
//                        password_status = true;
                        if (password_status) {
//                            Serial.println(WiFi.SSID(display_index));
//                            Serial.println(pass_phrase);
                            WiFi.begin(WiFi.SSID(display_index).c_str(), pass_phrase.c_str());
                            delay(1000);
                            if (WiFiClass::status() == WL_CONNECTED) {
                                static_loop("CONsc", 2000);
                                vTaskResume(main_loop_handle);
                                break;
                            } else {
                                static_loop("CONun", 2000);
                                vTaskResume(main_loop_handle);
                                break;
                            }
                        } else {
                            static_loop("Error", 3000);
                            vTaskResume(main_loop_handle);
                            break;
                        }
                    }
                    static_loop("Done!", 2000);
                    vTaskResume(main_loop_handle);
                    break;
                }
            }
        }
    }
}

void Matrix_display::static_text(const std::string &text) {
    // Reading from left to right
    std::string text_r = std::string(text.rbegin(), text.rend());
    text_r.insert(text_r.begin(), 5 - text_r.size(), ' ');
    // Initialize first column
    digitalWrite(columns_data, 0);
    tick_column();
    digitalWrite(columns_data, 1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (char letter: text_r) {
        for (uint8_t i = 0; i < COLUMNS / 5; ++i) {
            uint8_t letter_column = ~alphabet[letter][i];
//            uint8_t letter_column = ~alphabet[30][i];
            // Write data to the column
            for (uint8_t letter_row = 0; letter_row < 8; ++letter_row) {
                digitalWrite(pins[letter_row], letter_column & 1);
                letter_column >>= 1;
            }
            // Delay for human eye
            delayMicroseconds(300);
//            vTaskDelay(1/portTICK_PERIOD_MS);
//            vTaskDelayUntil(&xLastWakeTime, 1);
            reset_rows();
            tick_column();
        }
    }
//    pixels_off();
}

void Matrix_display::static_text_byte(const byte text[]) {
    // Reading from left to right
    // Initialize first column
    digitalWrite(columns_data, 0);
    tick_column();
    digitalWrite(columns_data, 1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (byte i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < COLUMNS / 5; ++j) {
            uint8_t letter_column = ~alphabet[text[4 - i]][j];
//            uint8_t letter_column = ~alphabet[30][i];
            // Write data to the column
            for (uint8_t letter_row = 0; letter_row < 8; ++letter_row) {
                digitalWrite(pins[letter_row], letter_column & 1);
                letter_column >>= 1;
            }
            // Delay for human eye
            delayMicroseconds(300);
//            vTaskDelay(1/portTICK_PERIOD_MS);
//            vTaskDelayUntil(&xLastWakeTime, 1);
            reset_rows();
            tick_column();
        }
    }
}

void Matrix_display::static_loop(const std::string &text, time_t duration) {
//    auto *params = (Static_loop_params *) parameter;
//    const TickType_t duration = pdMS_TO_TICKS(params->duration);
    TickType_t start_time = xTaskGetTickCount();
//    vTaskSuspend(this->main_loop_handle);
    const auto ms_to_ticks = pdMS_TO_TICKS(duration);
    while (xTaskGetTickCount() - start_time < ms_to_ticks) {
        static_text(text);
        vTaskDelay(5);
    }
//    vTaskResume(this->main_loop_handle);
//    vTaskDelete(nullptr);
}

void Matrix_display::static_loop_byte(const byte text[], time_t duration) {
//    auto *params = (Static_loop_params *) parameter;
//    const TickType_t duration = pdMS_TO_TICKS(params->duration);
    TickType_t start_time = xTaskGetTickCount();
//    vTaskSuspend(this->main_loop_handle);
    const auto ms_to_ticks = pdMS_TO_TICKS(duration);
    while (xTaskGetTickCount() - start_time < ms_to_ticks) {
        static_text_byte(text);
        vTaskDelay(10);
    }
//    vTaskResume(this->main_loop_handle);
//    vTaskDelete(nullptr);
}

void Matrix_display::manual_wifi() {
    static_loop("netnd", 3);
    int network_count = WiFi.scanNetworks();


}

// Main displaying loop
[[noreturn]] void Matrix_display::display_loop(void *parameter) {
//    std::string *text;
//    text = (std::string *) parameter;
//    String data = "xxxxx";
//    char buffer[6] = "xxxx";
    esp_task_wdt_init(9999, false);
    while (true) {
        esp_task_wdt_reset();
        // TODO: Check if network credentials in memory
        if (WiFiClass::status() != WL_CONNECTED) {
            static_loop("NETnc", 3000);
        }
        if (alarm_enabled) {
            if (time_info.tm_hour == alarm_info.hour && time_info.tm_min == alarm_info.minute) {
                Serial.println("Alarm sounded");
                const byte full[] = {30, 30, 30, 30, 30};
                for (byte i = 0; i < 5; i++) {
                analogWrite(25, 1000);
                    static_loop_byte(full, 500);
                analogWrite(25, 500);
                    static_loop("     ", 500);
                analogWrite(25, 0);
                }
            }
        }
        getLocalTime(&time_info);
        static_loop(std::to_string(time_info.tm_hour) + ":" + std::to_string(time_info.tm_min),
                    4000); // Display hour and minute
        static_loop(std::to_string(time_info.tm_mon + 1) + "." + std::to_string(time_info.tm_mday),
                    4000); // Display month and day
        static_loop(std::to_string(time_info.tm_year + 1900), 4000); // Display year
        Serial.println(time_info.tm_year);
        Serial.println(time_info.tm_mon);
        Serial.println(time_info.tm_mday);
        Serial.println(time_info.tm_hour);
        Serial.println(time_info.tm_min);
//        params.text = "MSG01";
//        static_loop(params.text, params.duration);
//        params.text = "MSG02";
//        static_loop(params.text, params.duration);
//        static_text(*text);
    }
}
