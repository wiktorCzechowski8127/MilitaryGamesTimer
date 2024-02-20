#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "lcd.hpp"
#include "menu.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

void setup() {
  //butons

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  // Lcd init
  lcd.init();
  lcd.backlight();
  createProgressBarCharacters();

  // Serial init
  Serial.begin(115200);
}


void loop()
{
  processMenu();
}