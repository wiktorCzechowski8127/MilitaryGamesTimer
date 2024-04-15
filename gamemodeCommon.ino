#include "gamemodeCommon.hpp"

void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime, const msTimeT* const alarmSpeakerTime)
{
  memset(timing, 0, sizeof(gamemodeTiming));
  timing->isGameRunning = true;
  timing->currentTime = millis();
  if(*gametime == 0)
  {
    timing->isUnlimitedTime = true;
    timing->endgame = timing->currentTime;
  }
  else
  {
    timing->isUnlimitedTime = false;
    timing->endgame = timing->currentTime + *gametime;
  }
  timing->timeLeft = 0;
  timing->lastCurrentTime = timing->currentTime; //DEBUG
  timing->alarmSpeakerEnd = *alarmSpeakerTime;
  //DEBUG
  
  Serial.println("");
  Serial.println("GAMEMODE");    
  Serial.println("endgame: " + (String)timing->endgame);
  Serial.println("timeLeft: " + (String)timing->timeLeft);
  Serial.println("currentTime: " + (String)timing->currentTime);
  Serial.println("lastCurrentTime: " + (String)timing->lastCurrentTime);
  Serial.println("alarmSpeakerEnd: " + (String)timing->alarmSpeakerEnd);
  Serial.println("isUnlimitedTime: " + (String)timing->isUnlimitedTime);
  Serial.println("turnSpeakerAlarmOn: " + (String)timing->turnSpeakerAlarmOn);
  Serial.println("invertTime: " + (String)timing->invertTime);
  
}

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing)
{
  bool isGameRunning = true;
  if(timing->currentTime > timing->endgame &&
     timing->invertTime == false)
  {
    Serial.println("ENTER PIXA");
    isGameRunning = false;
    timing->timeLeft = 0;
    timing->turnSpeakerAlarmOn = true;
  }
  else
  {
    if(timing->invertTime == true)
    {
      timing->timeLeft = timing->currentTime - timing->endgame;
    }
    else
    {
      timing->timeLeft = timing->endgame - timing->currentTime;
    }
    printTime(&timing->timeLeft, false);
    //Serial.println(timing->timeLeft); //DEBUG
  }
  return isGameRunning;
}

void verifyEndGame(gamemodeTiming* timing)
{
  if (timing->isGameRunning == true) 
  {
    // Turning game off using buttons.
    timing->endButtonsKeepPushed = false;
    if (!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON))
    {
      timing->endButtonsKeepPushed = true;
      if (timing->buttonPushingTime == 0) 
      {
        timing->buttonPushingTime = timing->currentTime;
      }
    }
    if (timing->endButtonsKeepPushed == true) 
    {
      if (timing->currentTime > (timing->buttonPushingTime + 2000)) 
      {
        timing->isGameRunning = false;
        timing->turnSpeakerAlarmOn = false;
      }
    } 
    else
    {
      timing->buttonPushingTime = 0;
    }
  }
}

void processGameSummary(gamemodeTiming* timing)
{
  if(timing->turnSpeakerAlarmOn)
  {
    timing->alarmSpeakerEnd = timing->alarmSpeakerEnd + millis();
    digitalWrite(RELAY, RELAY_ON);
  }

  waitUntilButtonReleased();
  uint8_t endButtonKeepPushed = false;
  msTimeT pushingTime = 0;

  while (true)
  {
    timing->currentTime = millis();
    endButtonKeepPushed = false;

    if (!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON))
    {
      endButtonKeepPushed = true;
      if (pushingTime == 0)
      {
        pushingTime = timing->currentTime;
      }
    }
    if (endButtonKeepPushed == true)
    {
      if (timing->currentTime > (pushingTime + 2000))
      {
        if (timing->turnSpeakerAlarmOn == true) 
        {
          digitalWrite(RELAY, RELAY_OFF);
          pushingTime = 0;
          timing->turnSpeakerAlarmOn = false;
          waitUntilButtonReleased();
        } 
        else
        {
          break;
        }
      }
    } 
    else
    {
      pushingTime = 0;
    }
    if ((timing->currentTime > timing->alarmSpeakerEnd) && (timing->turnSpeakerAlarmOn == true))
    {
      digitalWrite(RELAY, RELAY_OFF);
      timing->alarmSpeakerEnd = 0;
      timing->turnSpeakerAlarmOn = false;
    }
    //Serial.println("MILLIS: " + String(millis()) + " ALARM SPEAKER END " + String(timing.alarmSpeakerEnd));
  }
}