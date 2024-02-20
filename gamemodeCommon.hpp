#ifndef GAMEMODE_COMMON
#define GAMEMODE_COMMON

/* > Include  *****************************************************************/
#include "common.hpp"
#include <LiquidCrystal_I2C.h>
/* > Defines ******************************************************************/


/*******************************************************************************
 * struct: gamemodeTiming
 * 
 * @brief Time pareameters usend to control game time.
 * 
*******************************************************************************/
struct gamemodeTiming
{   
  msTimeT endgame;
  unsigned long timeLeft;
  unsigned long currentTime;
  unsigned long lastCurrentTime; //DEBUG
  bool isGameRunning = true;
};

void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime);

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing);

#endif