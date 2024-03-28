#include "gamemodeCommon.hpp"

void initializeTiming(gamemodeTiming* timing, const unsigned long* const gametime, const msTimeT* const alarmSpeakerTime)
{
  memset(timing, 0, sizeof(timing));
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
}

bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing)
{
  bool isGameRunning = true;
  if(timing->currentTime > timing->endgame && timing->isUnlimitedTime == false)
  {
    isGameRunning = false;
    timing->timeLeft = 0;
    timing->turnSpeakerAlarmOn = true;
  }
  else
  {
    if(timing->isUnlimitedTime == true)
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

void verifyEndGame(gamemodeTiming* timing, uint8_t lcdpos1, uint8_t lcdpos2)
{
  if (timing->isGameRunning == true) 
  {
    lcd.setCursor(lcdpos1,lcdpos2);
    timing->isGameRunning = valideateEndGameOrPrintTimeLeft(timing);

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
    digitalWrite(RELAY, true);
  }

  delay(FREEZE_TIME);
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
          digitalWrite(RELAY, false);
          pushingTime = 0;
          timing->turnSpeakerAlarmOn = false;
          Serial.println("EXIT");
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
      digitalWrite(RELAY, false);
      timing->alarmSpeakerEnd = 0;
      timing->turnSpeakerAlarmOn = false;
    }
    //Serial.println("MILLIS: " + String(millis()) + " ALARM SPEAKER END " + String(timing.alarmSpeakerEnd));
  }
}