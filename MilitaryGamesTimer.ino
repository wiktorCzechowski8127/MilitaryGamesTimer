#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "menu.hpp"
#include "buttons.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

//#define RELAY_ON true
//#define RELAY_OFF !RELAY_ON

bool RELAY_OFF = 0;
bool RELAY_ON = 0;

void setup() 
{
  //butons
  initbuttons();

  // Lcd init
  lcd.init();
  lcd.backlight();

  // Serial init
  Serial.begin(115200);
}


void loop()
{
  processMenu();
}