#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "menu.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

#define RELAY_ON true
#define RELAY_OFF !RELAY_ON


void setup() 
{
  //butons
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, RELAY_OFF);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, false);

  pinMode(LEFT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LEFT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
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