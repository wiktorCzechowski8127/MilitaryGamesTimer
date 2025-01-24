#include <stdint.h>
#ifndef DOMINATION_H
#define DOMINATION_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "gamemodeCommon.hpp"
#include "gpio.hpp"
#include "led.hpp"

/* > Defines ******************************************************************/

#define POINTING_NONE 0
#define POINTING_LEFT_TEAM 1
#define POINTING_RIGHT_TEAM 2
#define WINNING_POINTS_OUT_OF_RANGE "9+9"
#define LEFT_BUTTOM_INDICATOR 3
#define RIGHT_BUTTOM_INDICATOR 12
#define RED_LED 12
#define BLUE_LED 13

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;
extern bool RELAY_ON;
extern bool RELAY_OFF;

/* > Structures ***************************************************************/


/*******************************************************************************
 * struct: gameHistoryS
 * 
 * @brief Structure with history of games results .
 * 
*******************************************************************************/
struct dominationHistoryS
{
  msTimeT gameTime;
  msTimeT runningTime;
  unsigned int leftTeamWinningPoints;
  unsigned int rightTeamWinningPoints;
};

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
  unsigned int winningPointsLimit;
  msTimeT delayStart;
  bool enableSwitch;
  msTimeT alarmSpeaker;
  bool pressButtonToStartGame;
  dominationHistoryS history[MAX_HISTORY_RECORDS];
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
  byte padding:2;
};

/* > Functions ****************************************************************/
void printWinningPoints(const unsigned int* const leftTeamWinningPoints, 
                        const unsigned int* const rightTeamWinningPoints,
                        bool printOnLowerPart);

void printSummary(const msTimeT* const gameTime, 
                  const msTimeT* const runningTime,
                  const unsigned int* const leftTeamWinningPoints, 
                  const unsigned int* const rightTeamWinningPoints);

void clearButtonsStatus(dominationDataS* data);

bool validateWinningPoints(const unsigned int* const points, 
                           const unsigned int* const limit, 
                           msTimeT* alarmSpeakerEnd);

void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm); //DEBUG

void setPointingTeam(const gamemodeDominationS* const gm, 
                     dominationDataS* data, 
                     msTimeT currentTime, 
                     uint8_t pointingTeam);

void setNonePointingTeam(dominationDataS* data, ledC* led);

void ledBlink(const gamemodeDominationS* const gm,
              const dominationDataS* const data, 
              msTimeT currentTime,
              ledC* leftLed,
              ledC* rightLed);

bool calcWinningPointsAndCheckIsGameEnd(gamemodeTiming* timing, 
                                        dominationDataS* data,
                                        gamemodeDominationS* gm);

void saveResult(dominationHistoryS* const history, 
                const msTimeT* const gameTime,
                const msTimeT* const runningTime,
                const unsigned int* const leftTeamWinningPoints, 
                const unsigned int* const rightTeamWinningPoints);

void processDomination(gamemodeDominationS* const gm);

void printDominationHisotry(const dominationHistoryS* const history);

#endif