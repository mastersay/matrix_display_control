//
// Created by japru on 11/03/2023.
//
#include "display.hpp"
#include <Arduino.h>

#define COLUMNS 25

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
    digitalWrite(clk, 0);
}

void Matrix_display::pixels_on() const {
    digitalWrite(reset, 0);
    delayMicroseconds(1);
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

void Matrix_display::static_text(const std::string &text) {
    // Reading from left to right
    std::string text_r = std::string(text.rbegin(), text.rend());

    // Initialize first column
    digitalWrite(columns_data, 0);
    tick_column();
    digitalWrite(columns_data, 1);

    for (char letter: text_r) {
        for (uint8_t i = 0; i < COLUMNS / 5; ++i) {
            uint8_t letter_column = ~alphabet[letter][i];
            // Write data to the column
            for (uint8_t letter_row = 0; letter_row < 8; ++letter_row) {
                digitalWrite(pins[letter_row], letter_column & 1);
                letter_column >>= 1;
            }
            // Delay for human eye
            delayMicroseconds(300);
            reset_rows();
            tick_column();
        }
    }
    pixels_off();
}

void Matrix_display::display_loop(const std::string& text) {
    while (true) static_text(text);
}


