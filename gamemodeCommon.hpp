#include <stdint.h>
#ifndef GAMEMODE_COMMON
#define GAMEMODE_COMMON

/* > Include  *****************************************************************/
#include "common.hpp"
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/

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
  uint8_t isGameRunning: 1;
  uint8_t padding: 7;
};

/* > Functions ****************************************************************/
void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime, const msTimeT* const alarmSpeakerTime);

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing);

#endif