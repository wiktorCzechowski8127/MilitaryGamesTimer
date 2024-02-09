#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "domination.hpp"
#include "lcd.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

void setup() {
  // Lcd init
  lcd.init();
  lcd.backlight();
  createProgressBarCharacters();

  // Serial init
  Serial.begin(115200);
}

void loop()
{
  dominationMain();
}