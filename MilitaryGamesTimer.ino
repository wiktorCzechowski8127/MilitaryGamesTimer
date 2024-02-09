#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "domination.hpp"
#include "characters.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

// To change
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

void setup() {
  lcd.init();

  lcd.createChar(FILLED, filled);
  lcd.createChar(RIGHT_STRIP, rightStrip);
  lcd.createChar(RIGHT_STRIP_FILLED, rightStripFilled);
  lcd.createChar(LEFT_STRIP, leftStrip);
  lcd.createChar(LEFT_STRIP_FILLED, leftStripFilled);
  lcd.createChar(BOTTOM, bottomStrip);
  lcd.backlight();

  Serial.begin(115200);


  
}


void loop()
{
  dominationMain();
}