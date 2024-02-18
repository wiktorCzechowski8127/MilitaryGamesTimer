//#include <ezButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "domination.hpp"
#include "lcd.hpp"
#include "menu.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

/*
ezButton rightButton(8);
ezButton leftButton(9);
ezButton upButton(10);
ezButton downButton(11);

menuBaseS menuBase;
*/

void setup() {
  //butons


  // Lcd init
  lcd.init();
  lcd.backlight();
  createProgressBarCharacters();

  // Serial init
  Serial.begin(115200);

  //initializeMenu(&menuBase);
  
}

//bool isButtonPushed = false;

void loop()
{ 

  processMenu();

  //dominationMain();
}