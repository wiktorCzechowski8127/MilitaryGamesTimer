#ifndef CHARACTERS_H
#define CHARACTERS_H

#define FILLED 0
#define RIGHT_STRIP 1
#define RIGHT_STRIP_FILLED 2
#define LEFT_STRIP 3
#define LEFT_STRIP_FILLED 4
#define BOTTOM 5

#define FIRST_LEFT_PLACE 7
#define FIRST_RIGHT_PLACE 8

#define FIRST_LCD_CHAR 0
#define LAST_LCD_CHAR 15

// Both sites
byte filled[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte bottomStrip[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

// Ledt site
byte rightStrip[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111,
};

// Ledt site
byte rightStripFilled[8] = {
  B00001,
  B00001,
  B00001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

// right site
byte leftStrip[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11111,
};

byte leftStripFilled[8] = {
  B10000,
  B10000,
  B10000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

#endif