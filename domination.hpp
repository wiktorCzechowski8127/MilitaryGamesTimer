#include <stdint.h>
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
#define WINNING_POINTS_OUT_OF_RANGE "9+9"

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

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
  bool enableSwitch;
  msTimeT alarmSpeaker;
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
  byte padding:4;
};

/* > Functions ****************************************************************/
void printWinningPoints(const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints, bool printOnLowerPart);
void printSummary(const msTimeT* const gameTime, const msTimeT* const runningTime ,const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints);

void clearButtonsStatus(dominationDataS* data);
bool validateWinningPoints(const unsigned int* const points, const unsigned int* const limit, msTimeT* alarmSpeakerEnd);
void dominationInitializeProgressBarData(progressBarDataS* data, const gamemodeDominationS* const gm);
void calculateFilledSquares(const gamemodeDominationS* const gm, progressBarDataS* progressBarData, const long int* const pointsInMs, gamemodeTiming* timing);
void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm); //DEBUG

bool calcWinningPointsAndCheckIsGameEnd(gamemodeTiming* timing, dominationDataS* data, gamemodeDominationS* gm);
void saveResult(dominationHistoryS* const history, 
                const msTimeT* const gameTime,
                const msTimeT* const runningTime,
                const unsigned int* const leftTeamWinningPoints, 
                const unsigned int* const rightTeamWinningPoints);

void processDomination(gamemodeDominationS* const gm);

void printDominationHisotry(const dominationHistoryS* const history);

#endif