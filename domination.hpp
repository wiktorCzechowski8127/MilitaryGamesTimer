#ifndef DOMINATION_H
#define DOMINATION_H

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
 * struct: gamemodeDominationS
 * 
 * @brief Domination gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeDominationS
{
  msTimeT gameTime;
  msTimeWithSingT fullTakeOverTime;
  msTimeWithSingT takeOverTime;
  bool enableSwitch;
};

struct pointsAndButtonsDataS
{
  long int pointsInMs;
  unsigned long lastPushedButtonTimeStamp;
  byte isRightButtonPushed:1;
  byte isLeftButtonPushed:1;
  byte isLeftTeamWinning:1;
  byte padding:5;
};

/* > Functions ****************************************************************/

void dominationMain();
void setScren();

void initializeProgressBarData(progressBarDataS* data);
void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm);
void processDomination(const gamemodeDominationS* const gm);


#endif