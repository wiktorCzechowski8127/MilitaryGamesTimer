#ifndef DOMINATION_H
#define DOMINATION_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "gamemodeCommon.hpp"
#include "lcd.hpp"
#include "buttons.hpp"

/* > Defines ******************************************************************/

#define POINTING_NONE 0
#define POINTING_LEFT_TEAM 1
#define POINTING_RIGHT_TEAM 2

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
  msTimeT pointTime;
  bool enableSwitch;
};

struct dominationDataS
{
  msTimeWithSingT pointsInMs;
  msTimeT lastPushedButtonTimeStamp;
  msTimeT addPointTimeStamp;
  unsigned int leftTeamWinningPoints;
  unsigned int rightTeamWinningPoints;  
  byte isRightButtonPushed:1;
  byte isLeftButtonPushed:1;
  byte isLeftTeamWinning:1;
  byte pointingTeam:2; // 0 - None, 1 - left, 2 - right
  byte swithStatus:1;
  byte padding:4;
};

/* > Functions ****************************************************************/

void dominationMain();
void setScren();

void printWinningPoints(const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints, bool printOnLowerPart);
void printSummary(const msTimeT* const gameTime, const msTimeT* const runningTime ,const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints);

void clearButtonsStatus(dominationDataS* data);

void initializeProgressBarData(progressBarDataS* data);
void calculateFilledSquares(const gamemodeDominationS* const gm, progressBarDataS* progressBarData, const long int* const pointsInMs);
void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm);
void processDomination(const gamemodeDominationS* const gm);


#endif