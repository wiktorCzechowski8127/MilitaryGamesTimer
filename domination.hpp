#ifndef DOMINATION_H
#define DOMINATION_H

///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "lcd.hpp"
#include "menu.hpp"
#include "gamemodeCommon.hpp"
///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////


void dominationMain();
void setScren();


void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm);
void processDomination(const gamemodeDominationS* const gm);


#endif