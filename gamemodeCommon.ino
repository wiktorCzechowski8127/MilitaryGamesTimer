#include "gamemodeCommon.hpp"

/* > Function initializeTiming
*******************************************************************************/
/**
 * @brief Filling timing structure.
 * 
 * @param[in] timeInMs      Time in Ms to print.
 * @param[in] minutesOlny   Printing time without hours.
 * @return void
 * 
*******************************************************************************/
void initializeTiming(gamemodeTiming* timing,
                      const unsigned long* const gametime,
                      const msTimeT* const alarmSpeakerTime)
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
  timing->lastCurrentTime = timing->currentTime;
  timing->alarmSpeakerEnd = *alarmSpeakerTime;

  //DEBUG
  /*
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
  */
}

/* > Function valideateEndGameOrPrintTimeLeft
*******************************************************************************/
/**
 * @brief Checking end game conditions and if none of them is true calculating
 *        and printing on lcd time left.
 * 
 * @param[in] timing      Pointer to gamemodeTiming structure.
 *
 * @return void
 * 
*******************************************************************************/
bool valideateEndGameOrPrintTimeLeft(gamemodeTiming* timing)
{
  bool isGameRunning = true;

  if(timing->currentTime > timing->endgame &&
     timing->invertTime == false)
  {
    isGameRunning = false;
    timing->timeLeft = 0;
    timing->turnSpeakerAlarmOn = true;
  }
  else
  {
    if(timing->invertTime == true)
    {
      // When invertTime == true this parameter calculate time elapsed since the start.
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

/* > Function checkGameInterrupt
*******************************************************************************/
/**
 * @brief Checking whether left and right menu buttons are pushed. If buttons are
 *        pushed longer than 2 seconds game end. 
 * 
 * @param[in] timing      Pointer to gamemodeTiming structure.
 *
 * @return void
 * 
*******************************************************************************/
void checkGameInterrupt(gamemodeTiming* timing)
{
  if (timing->isGameRunning == true) 
  {
    timing->endButtonsKeepPushed = false;

    // buttons pushing check.
    if (!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON))
    {
      timing->endButtonsKeepPushed = true;

      if (timing->buttonPushingTime == 0) 
      {
        timing->buttonPushingTime = timing->currentTime;
      }
    }

    // Pressing buttons continuously.
    if (timing->endButtonsKeepPushed == true) 
    {
      if (timing->currentTime > (timing->buttonPushingTime + INTERRUPT_TIME))
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

/* > Function processGameSummary
*******************************************************************************/
/**
 * @brief   Controlling the speaker alarm and processing buttons that allow 
 *          to leave the game summary and go to the menu.
 * 
 * @param[in] timing      Pointer to gamemodeTiming structure.
 *
 * @return void
 * 
*******************************************************************************/
void processGameSummary(gamemodeTiming* timing)
{
  if(timing->turnSpeakerAlarmOn)
  {
    timing->alarmSpeakerEnd = timing->alarmSpeakerEnd + millis();
    digitalWrite(RELAY, RELAY_ON);
  }

  uint8_t endButtonKeepPushed = false;
  msTimeT pushingTime = 0;
  waitUntilButtonReleased();

  while (true)
  {
    timing->currentTime = millis();
    endButtonKeepPushed = false;

    // TODO: try to move this to function.
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
      if (timing->currentTime > (pushingTime + INTERRUPT_TIME))
      {

        // turning off speaker alarm if it works.
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

    // checking if it's time to turn off the siren
    if ((timing->currentTime > timing->alarmSpeakerEnd) && (timing->turnSpeakerAlarmOn == true))
    {
      digitalWrite(RELAY, RELAY_OFF);
      timing->alarmSpeakerEnd = 0;
      timing->turnSpeakerAlarmOn = false;
    }
    //Serial.println("MILLIS: " + String(millis()) + " ALARM SPEAKER END " + String(timing.alarmSpeakerEnd));
  }
}