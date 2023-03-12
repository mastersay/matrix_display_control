//
// Created by japru on 11/03/2023.
//

#ifndef ESP32_VERSION_DISPLAY_HPP
#define ESP32_VERSION_DISPLAY_HPP

#include <cstdint>
#include <array>

const uint8_t alphabet[][5] = {
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
        {0x00, 0x01, 0x02, 0x03, 0x04},
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

class Matrix_display {


    void tick_column() const;

    void reset_rows() const;

    uint8_t columns_data, reset, clk, menu_entry, menu_left, menu_right;
    const std::array<const uint8_t, 8> pins;
public:
    Matrix_display(uint8_t columns_data, uint8_t reset, uint8_t clk, std::array<const uint8_t, 8> pins,
                   uint8_t menu_entry, uint8_t menu_left, uint8_t menu_right);


    void pixels_on() const;

    void pixels_off() const;

    void static_text(const std::string &text);

    void display_loop(const std::string& text);
};


#endif //ESP32_VERSION_DISPLAY_HPP