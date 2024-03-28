#include <stdint.h>
#ifndef GAMEMODE_COMMON
#define GAMEMODE_COMMON

/* > Include  *****************************************************************/
#include "common.hpp"
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/
#define MAX_HISTORY_RECORDS 3
/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/*******************************************************************************
 * struct: gamemodeTiming
 * 
 * @brief Time pareameters usend to control game time.
 * 
*******************************************************************************/
struct gamemodeTiming
{   
  msTimeT endgame;
  msTimeT timeLeft;
  msTimeT currentTime;
  msTimeT lastCurrentTime; //DEBUG
  msTimeT alarmSpeakerEnd;
  msTimeT buttonPushingTime;
  uint8_t isGameRunning: 1;
  uint8_t isUnlimitedTime: 1;
  uint8_t turnSpeakerAlarmOn: 1;
  uint8_t endButtonsKeepPushed: 1;
  uint8_t padding: 4;
};

/* > Functions ****************************************************************/
void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime, const msTimeT* const alarmSpeakerTime);

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing);

void verifyEndGame(gamemodeTiming* timing, uint8_t lcdpos1, uint8_t lcdpos2);
void processGameSummary(gamemodeTiming* timing);

#endif