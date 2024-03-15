#ifndef BOMB_H
#define BOMB_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "gamemodeCommon.hpp"
#include "lcd.hpp"
#include "buttons.hpp"

/* > Defines ******************************************************************/

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/*******************************************************************************
 * struct: gamemodeBombS
 * 
 * @brief Bomb gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeBombS
{   
    // STAGE_1_1_OPTIONS 3
    msTimeT gameTime;
    msTimeT armingTime;
    msTimeT defusingTime;
    bool enableSwitch;
    bool slowReversing;
    msTimeT alarmSpeaker;
};

struct bombDataS
{
  msTimeWithSingT pointsInMs;
  msTimeT lastPushedButtonTimeStamp;

};

/* > Functions ****************************************************************/


void processBomb(const gamemodeBombS* const gm);

void bombInitializeProgressBarData(progressBarDataS* data, const gamemodeBombS* const gm);

void printBombGamemodeSettingsOnSerial(const gamemodeBombS* const gm); //DEBUG
#endif
