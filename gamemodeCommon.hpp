#ifndef GAMEMODE_COMMON
#define GAMEMODE_COMMON

/* > Include  *****************************************************************/
#include <stdint.h>
#include "common.hpp"
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/
#define MAX_HISTORY_RECORDS 3
#define BLINK_TIME 500
#define INTERRUPT_TIME 2000

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;
extern bool RELAY_ON;
extern bool RELAY_OFF;

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
  msTimeT timeLeft; // When invertTime == true this parameter calculate time elapsed since the start.
  msTimeT currentTime;
  msTimeT lastCurrentTime;
  msTimeT alarmSpeakerEnd;
  msTimeT buttonPushingTime;
  uint8_t isGameRunning: 1;
  uint8_t isUnlimitedTime: 1;
  uint8_t turnSpeakerAlarmOn: 1;
  uint8_t endButtonsKeepPushed: 1;
  uint8_t invertTime: 1;
  uint8_t padding: 3;
};

/* > Functions ****************************************************************/
void initializeTiming(gamemodeTiming* timing,
                      const unsigned long* const gametime,
                      const msTimeT* const alarmSpeakerTime);

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing);

void checkGameInterrupt(gamemodeTiming* timing);

void processGameSummary(gamemodeTiming* timing);

#endif