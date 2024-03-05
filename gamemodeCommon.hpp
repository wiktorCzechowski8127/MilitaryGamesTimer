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
  bool isGameRunning = true;
};

/* > Functions ****************************************************************/
void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime);

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing);

#endif