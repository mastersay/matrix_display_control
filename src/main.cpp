
//#define reset 11
//#define column_clock 9
//#define in_data_columns 8
//#define row_clock_memory 13 // clock sh_cp
//#define in_data_rows 8 // data
//#define row_clock 10 // latch
//#define rows_enable 12 // ground

#define reset 2
#define column_clock 3
#define in_data_columns 4
#define row_clock_memory 5 // clock sh_cp
#define in_data_rows 6 // data
#define row_clock 7 // latch
#define rows_enable 8 // ground
#define menu_entry 9
#define menu_left 10
#define menu_right 11

#include <Arduino.h>
//#include <DateTime.h>
#include <TimeLib.h>
//#include <Wire.h>

void setup() {
    pinMode(reset, OUTPUT);
    digitalWrite(reset, 1);
    pinMode(column_clock, OUTPUT);
    pinMode(in_data_columns, OUTPUT);
    pinMode(row_clock, OUTPUT);
    pinMode(row_clock_memory, OUTPUT);
    pinMode(in_data_rows, OUTPUT);
    pinMode(rows_enable, OUTPUT);
    digitalWrite(rows_enable, 1);
    digitalWrite(rows_enable, 0);

    pinMode(menu_entry, INPUT_PULLUP);
    pinMode(menu_left, INPUT_PULLUP);
    pinMode(menu_right, INPUT_PULLUP);
    Serial.begin(9600);
}

void tick_column();

void tick_row();

void tick_row_memory();

void display_on();

void display_off();

void display_reset();

void display_char_part(byte number);

void display_char(const byte sign[]);

void display_string(byte text[]);

void display_string2(String text);

void columns_on();

void reverse(byte arr[], byte size);

void run(char text[]);

void menu(int menu_pos);

void menu_update(const char dt_info[], int (*time_func)(time_t), unsigned long time_change, byte menu_u_pos);

time_t date_time = 1;
bool previous_menu_state = true;
byte right_mover = 1;
byte alphabet[][5] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, //  Space
        {0xFF, 0xFF, 0xA0, 0xFF, 0xFF}, //  !
        {0xFB, 0xFC, 0xFB, 0xFC, 0xFF}, //  "
        {0xC9, 0x80, 0xD9, 0x80, 0xC9}, //  #
        {0xDB, 0xD5, 0x80, 0xD5, 0xED}, //  $
        {0xBC, 0xCC, 0xF7, 0x99, 0x9E}, //  %
        {0xCF, 0xB5, 0xB2, 0xCD, 0xB7}, //  &
        {0xFF, 0xFF, 0xF8, 0xFF, 0xFF}, //  '
        {0xE3, 0xDD, 0xBE, 0xFF, 0xFF}, //  (
        {0xFF, 0xFF, 0xBE, 0xDD, 0xE3}, //  )
        {0xD5, 0xE3, 0x80, 0xE3, 0xD5}, //  *
        {0xF7, 0xF7, 0xC1, 0xF7, 0xF7}, //  +
        {0xBF, 0xCF, 0xFF, 0xFF, 0xFF}, //  ,
        {0xF7, 0xF7, 0xF7, 0xF7, 0xF7}, //  -
        {0xFF, 0x9F, 0x9F, 0xFF, 0xFF}, //  .
        {0xBF, 0xCF, 0xF7, 0xF9, 0xFE}, //  /
        {0xC1, 0xAE, 0xB6, 0xBA, 0xC1}, //  0
        {0xFF, 0xBD, 0x80, 0xBF, 0xFF}, //  1
        {0xBD, 0x9E, 0xAE, 0xB6, 0xB9}, //  2
        {0xDE, 0xBE, 0xBA, 0xB4, 0xCE}, //  3
        {0xE7, 0xEB, 0xED, 0x80, 0xEF}, //  4
        {0xD8, 0xBA, 0xBA, 0xBA, 0xC6}, //  5
        {0xC1, 0xB6, 0xB6, 0xB6, 0xCF}, //  6
        {0xFE, 0x8E, 0xF6, 0xFA, 0xFC}, //  7
        {0xC9, 0xB6, 0xB6, 0xB6, 0xC9}, //  8
        {0xF9, 0xB6, 0xB6, 0xB6, 0xC1}, //  9
        {0xFF, 0x99, 0x99, 0xFF, 0xFF}, //  :
        {0xFF, 0xA9, 0xC9, 0xFF, 0xFF}, //  ;
        {0xF7, 0xEB, 0xDD, 0xBE, 0xFF}, //  <
        {0xEB, 0xEB, 0xEB, 0xEB, 0xEB}, //  =
        {0xFF, 0xBE, 0xDD, 0xEB, 0xF7}, //  >
        {0xFD, 0xFE, 0xAE, 0xF6, 0xF9}, //  ?
        {0xCD, 0xB6, 0x86, 0xBE, 0xC1}, //  @
        {0x81, 0xEE, 0xEE, 0xEE, 0x81}, //  A
        {0x80, 0xB6, 0xB6, 0xB6, 0xC9}, //  B
        {0xC1, 0xBE, 0xBE, 0xBE, 0xDD}, //  C
        {0x80, 0xBE, 0xBE, 0xDD, 0xE3}, //  D
        {0x80, 0xB6, 0xB6, 0xB6, 0xBE}, //  E
        {0x80, 0xF6, 0xF6, 0xF6, 0xFE}, //  F
        {0xC1, 0xBE, 0xBE, 0xB6, 0xC5}, //  G
        {0x80, 0xF7, 0xF7, 0xF7, 0x80}, //  H
        {0xFF, 0xBE, 0x80, 0xBE, 0xFF}, //  I
        {0xDF, 0xBF, 0xBE, 0xC0, 0xFE}, //  J
        {0x80, 0xF7, 0xEB, 0xDD, 0xBE}, //  K
        {0x80, 0xBF, 0xBF, 0xBF, 0xBF}, //  L
        {0x80, 0xFD, 0xF3, 0xFD, 0x80}, //  M
        {0x80, 0xFB, 0xF7, 0xEF, 0x80}, //  N
        {0xC1, 0xBE, 0xBE, 0xBE, 0xC1}, //  O
        {0x80, 0xF6, 0xF6, 0xF6, 0xF9}, //  P
        {0xC1, 0xBE, 0xAE, 0xDE, 0xA1}, //  Q
        {0x80, 0xF6, 0xE6, 0xD6, 0xB9}, //  R
        {0xB9, 0xB6, 0xB6, 0xB6, 0xCE}, //  S
        {0xFE, 0xFE, 0x80, 0xFE, 0xFE}, //  T
        {0xC0, 0xBF, 0xBF, 0xBF, 0xC0}, //  U
        {0xE0, 0xDF, 0xBF, 0xDF, 0xE0}, //  V
        {0xC0, 0xBF, 0xC7, 0xBF, 0xC0}, //  W
        {0x9C, 0xEB, 0xF7, 0xEB, 0x9C}, //  X
        {0xF8, 0xF7, 0x8F, 0xF7, 0xF8}, //  Y
        {0x9E, 0xAE, 0xB6, 0xBA, 0xBC}, //  Z
        {0xFF, 0x80, 0xBE, 0xFF, 0xFF}, //  [
        {0xFE, 0xF9, 0xF7, 0xCF, 0xBF}, //  backslash
        {0xFF, 0xFF, 0xBE, 0x80, 0xFF}, //  ]
        {0xFB, 0xFD, 0xFE, 0xFD, 0xFB}, //  ^
        {0xBF, 0xBF, 0xBF, 0xBF, 0xBF}, //  _
        {0xFF, 0xFE, 0xFD, 0xFF, 0xFF}, //  `
        {0xDF, 0xAB, 0xAB, 0xAB, 0x87}, //  a
        {0x80, 0xAF, 0xB7, 0xB7, 0xCF}, //  b
        {0xC7, 0xBB, 0xBB, 0xBB, 0xDF}, //  c
        {0xCF, 0xB7, 0xB7, 0xAF, 0x80}, //  d
        {0xC7, 0xAB, 0xAB, 0xAB, 0xF7}, //  e
        {0xF7, 0x81, 0xF6, 0xFE, 0xFF}, //  f
        {0xF3, 0xAD, 0xAD, 0xAD, 0xC1}, //  g
        {0x80, 0xF7, 0xFB, 0xFB, 0x87}, //  h
        {0xFF, 0xFF, 0x85, 0xFF, 0xFF}, //  i
        {0xDF, 0xBF, 0xBB, 0xC2, 0xFF}, //  j
        {0x80, 0xEF, 0xD7, 0xBB, 0xFF}, //  k
        {0xFF, 0xBE, 0x80, 0xBF, 0xFF}, //  l
        {0x83, 0xFB, 0xE7, 0xFB, 0x87}, //  m
        {0x83, 0xF7, 0xFB, 0xFB, 0x87}, //  n
        {0xC7, 0xBB, 0xBB, 0xBB, 0xC7}, //  o
        {0x83, 0xEB, 0xEB, 0xEB, 0xF7}, //  p
        {0xF7, 0xEB, 0xEB, 0xE7, 0x83}, //  q
        {0x83, 0xF7, 0xFB, 0xFB, 0xF7}, //  r
        {0xB7, 0xAB, 0xAB, 0xAB, 0xDF}, //  s
        {0xFB, 0xC0, 0xBB, 0xBF, 0xDF}, //  t
        {0xC3, 0xBF, 0xBF, 0xDF, 0x83}, //  u
        {0xE3, 0xDF, 0xBF, 0xDF, 0xE3}, //  v
        {0xC3, 0xBF, 0xCF, 0xBF, 0xC3}, //  w
        {0xBB, 0xD7, 0xEF, 0xD7, 0xBB}, //  x
        {0xF3, 0xAF, 0xAF, 0xAF, 0xC3}, //  y
        {0xBB, 0x9B, 0xAB, 0xB3, 0xBB}, //  z
        {0xF7, 0xC9, 0xBE, 0xFF, 0xFF}, //  {
        {0xFF, 0xFF, 0x80, 0xFF, 0xFF}, //  |
        {0xFF, 0xFF, 0xBE, 0xC9, 0xF7}, //  }
};


void loop() {
    while (Serial.available()) {
        long new_time = Serial.parseInt();
        if (new_time > 1) {
            date_time = new_time;
        }
    }
    Serial.println(date_time);
    //String text2 = "AAABB";
    // Texts with length < 5 may lead to undefined behaviour
    char text[] = "ABCDEF";
//    byte text[] = {'A', 'B', 'C', 'D', 'E'};
    //display_string(text);
    delay(10);
    display_reset();
//    display_string(text);
    run(text);
//    display_string2(text2);
}

void tick_column() {
    digitalWrite(column_clock, 1);
    delayMicroseconds(1);
    digitalWrite(column_clock, 0);
}

void tick_row() {
    digitalWrite(row_clock, 1);
    delayMicroseconds(1);
    digitalWrite(row_clock, 0);
}

void tick_row_memory() {
    digitalWrite(row_clock_memory, 1);
    delayMicroseconds(1);
    digitalWrite(row_clock_memory, 0);
}

void columns_on() {
    digitalWrite(in_data_columns, 1);
    for (byte i = 0; i < 25; i++) {
        tick_column();
    }
}

void display_on() {
    digitalWrite(reset, 1);
    digitalWrite(rows_enable, 0);
    digitalWrite(in_data_columns, 1);
    for (byte i = 0; i < 25; i++) {
        tick_column();
    }
}

void display_off() {
    digitalWrite(reset, 0);
    digitalWrite(rows_enable, 1);
}

void display_reset() {
    digitalWrite(reset, 0);
    delayMicroseconds(1);
    digitalWrite(reset, 1);
}

void display_char_part(byte number) {
    //number = ~number;
    for (byte i = 0; i < 8; ++i) {
        //digitalWrite(in_data_columns, number & 128);
        digitalWrite(in_data_rows, number & 128);
        number = number << 1;
        tick_row_memory();
    }
    tick_row();
}

void display_char(const byte sign[]) {
//    columns_on();
    // byte size = sizeof(sign) / sizeof(sign[0]);
    //size = size;
    for (short i = 4; i >= 0; i--) {
        display_char_part(sign[i]);
        digitalWrite(in_data_columns, right_mover);
        tick_column();
        right_mover = right_mover >> 1;
//        right_mover = 0;
        digitalWrite(rows_enable, 0);
        delayMicroseconds(500);
        digitalWrite(rows_enable, 1);
    }
}

void display_string(const char text[]) {
    right_mover = 1;
    for (short i = 4; i >= 0; i--) {

        byte c = text[i];
        display_char(alphabet[c]);
        delay(1);
    }
}

void reverse(byte arr[], byte size) {
    byte temp;
    byte i, j;
    for (i = 0, j = size - 1; i < size / 2; i++, j--) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void run(char text[]) {
    right_mover = 1;
//    byte buffer[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
//                     0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
//                     0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}; // with spaces
    // Buffer size: 5 letters * 5 columns = 25
    byte buffer[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//    byte buffer[][5] = {{0x80, 0x80, 0x80, 0x80, 0x80},
//                        {0x80, 0x80, 0x80, 0x80, 0x80},
//                        {0x80, 0x80, 0x80, 0x80, 0x80},
//                        {0x80, 0x80, 0x80, 0x80, 0x80},
//                        {0x80, 0x80, 0x80, 0x80, 0x80}};
    byte text_length = strlen(text);
    auto text_size_rounds = static_cast<float>(text_length / 5.0);
    byte sizer = 0;
    //for (byte i = 0; i < (byte) strlen(text); ++i) {
    for (byte i = 0; i < 5; ++i) {
        //reverse(alphabet[text[i]], 5);
//        for (byte j = 0; j < 5; j++) {
//            buffer[sizer][j] = alphabet[text[i]][j];
//        }
//        sizer++;
//        for (int j = 4; j >= 0; --j) {
        for (int j = 0; j < 5; ++j) {
            buffer[sizer] = alphabet[(byte) text[i]][j];
            sizer++;
        }
//        buffer[sizer] = 0xFF; // add spaces between
//        sizer++; // add spaces between
//        for (byte j: alphabet[text[i]]) {
//            buffer[sizer] = j;
//            sizer++;
//        }
    }
    right_mover = 1;
//    for (byte i = 0; i < 5; ++i) {
//
//
//        for (byte j = 4; j >= 0; --j) {
//
//            for (byte k = 0; k < 25; ++k) {
//                display_char_part(buffer[i][i]);
//                digitalWrite(in_data_columns, right_mover);
//                tick_column();
//                right_mover = right_mover >> 1;
//                digitalWrite(rows_enable, 0);
//                delay(1);
//                digitalWrite(rows_enable, 1);
//            }
//            right_mover = 1;
//            display_reset();
//        }
//    }
    short k;
    byte increaser = 0;
    while (increaser < text_size_rounds) {
        for (byte i = 0; i < 25; i++) {
            byte time = 25;
            if (i < 24) {
                time -= i;
            }

            // Go to menu
            if (digitalRead(menu_entry)) {
                previous_menu_state = true;
            }
            if (digitalRead(menu_entry) != previous_menu_state) {
                previous_menu_state = false;
                menu(0);
            }

            for (byte j = 0; j < time; j++) { // repeat one column with history
                for (k = i; k >= 0; --k) {
//            for (k = 0; k <=i; ++k) {
                    digitalWrite(in_data_columns, right_mover);
                    tick_column();
                    display_char_part(buffer[k]);
                    right_mover = right_mover >> 1;
                    digitalWrite(rows_enable, 0);
                    delayMicroseconds(500);
                    digitalWrite(rows_enable, 1);
                }
                right_mover = 1;
                display_reset();
            }
        }
        sizer = 0;
        increaser++;
        for (byte i = 0; i < 5; ++i) {
//            for (byte j = 4; j >= 0; --j) {
            for (byte j = 0; j < 5; ++j) {
                byte text_index = i + 5 * increaser;
                if (text_index > text_length) {
                    buffer[sizer] = alphabet[0][j];
                } else {
                    buffer[sizer] = alphabet[(byte) text[i + 5 * increaser]][j];
                }
                sizer++;
            }
        }
    }
}

void menu_update(const char dt_info[], int (*time_func)(time_t), unsigned long time_change, byte menu_u_pos) {
    char date_time_s[6];
    snprintf(date_time_s, 6, dt_info, time_func(date_time));
    while (true) {
        display_string(date_time_s);
        if (!digitalRead(menu_right)) {
            date_time += time_change; // Increase date & time
            snprintf(date_time_s, 6, dt_info, time_func(date_time));
            for (byte inner_delay = 0; inner_delay < 9; inner_delay++) {
                display_string(date_time_s);
            }
        } else if (!digitalRead(menu_left)) {
            date_time -= time_change; // Decrease date & time
            snprintf(date_time_s, 6, dt_info, time_func(date_time));
            for (byte inner_delay = 0; inner_delay < 9; inner_delay++) {
                display_string(date_time_s);
            }
        }
        if (digitalRead(menu_entry)) {
            previous_menu_state = true;
        }
        if (digitalRead(menu_entry) != previous_menu_state) {
            previous_menu_state = false;
            menu(menu_u_pos + 1);
            return;
        }
    }
}

void menu(int menu_pos) {
    char date_time_s[6];
    if (menu_pos == 0) {
        menu_update("Y%i", &year, 31536000, menu_pos);
    } else if (menu_pos == 1) {
        menu_update("M  %i", &month, 2628288, menu_pos);
    } else if (menu_pos == 2) {
        menu_update("D  %i", &day, 86400, menu_pos);
    } else if (menu_pos == 3) {
        menu_update("h  %i", &hour, 3600, menu_pos);
    } else if (menu_pos == 4) {
        menu_update("m  %i", &minute, 60, menu_pos);
    } else if (menu_pos == 5) {
        menu_update("s  %i", &second, 1, menu_pos);
    }
}

void display_string2(String text) {
    right_mover = 1;
    for (byte i = 0; i < 5; i++) {

        byte c = text[i];
        display_char(alphabet[c - 65]);
        delay(1);
    }
}
