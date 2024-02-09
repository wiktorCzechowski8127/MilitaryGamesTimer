#include <LiquidCrystal_I2C.h>
#include "lcd.hpp"

void createProgressBarCharacters()
{
    extern LiquidCrystal_I2C lcd; // TODO - verify is that correct

    // Both sites characters
    byte filled[8] = {B00000,
                      B00000,
                      B00000,
                      B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111,};

    byte bottomStrip[8] = {B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B11111,};

    // Site characters
    byte rightStrip[8] = {B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B11111,};

    byte rightStripFilled[8] = {B00001,
                                B00001,
                                B00001,
                                B11111,
                                B11111,
                                B11111,
                                B11111,
                                B11111,};

    byte leftStrip[8] = {B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B11111,};

    byte leftStripFilled[8] = {B10000,
                               B10000,
                               B10000,
                               B11111,
                               B11111,
                               B11111,
                               B11111,
                               B11111,};

    // Creating chars
    lcd.createChar(FILLED, filled);
    lcd.createChar(RIGHT_STRIP, rightStrip);
    lcd.createChar(RIGHT_STRIP_FILLED, rightStripFilled);
    lcd.createChar(LEFT_STRIP, leftStrip);
    lcd.createChar(LEFT_STRIP_FILLED, leftStripFilled);
    lcd.createChar(BOTTOM, bottomStrip);
}