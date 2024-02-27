#include "gamemodeCommon.hpp"

void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime)
{
  memset(timing, 0, sizeof(timing));
  timing->endgame = millis() + *gametime;
  timing->timeLeft = 0;
  timing->currentTime = 0;
  timing->lastCurrentTime = 0; //DEBUG
  //DEBUG 
  Serial.println("");
  Serial.println("GAMEMODE");    
  Serial.println("endgame: " + (String)timing->endgame);
  Serial.println("timeLeft: " + (String)timing->timeLeft);
  Serial.println("currentTime: " + (String)timing->currentTime);
  Serial.println("lastCurrentTime: " + (String)timing->lastCurrentTime);
}

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing)
{
  bool isGameRunning = true;
  if(timing->currentTime > timing->endgame)
  {
    isGameRunning = false;
  }
  else
  {
    timing->timeLeft = timing->endgame - timing->currentTime;
    lcd.setCursor(4,0);
    printTime(&timing->timeLeft, false);
    //Serial.println(timing->timeLeft); //DEBUG
  }
  return isGameRunning;
}