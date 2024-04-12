#include "bomb.hpp"

void calculateTotalTimes(gamemodeTiming* timing, bombDataS* bombData)
{
  msTimeT timediff = timing->currentTime - timing->lastCurrentTime;
  if(bombData->currentBombStatus == BOMB_ARMED)
  {
    bombData->armedTotalTime += timediff;
  }
  else
  {
    bombData->unarmedTotalTime += timediff;
  }
  //Serial.println("TimeDiff: " + (String)timediff + " unarmedTotalTime: " + (String)bombData->unarmedTotalTime); //DEBUG
}

void printSummary(const msTimeT* const unarmedTotalTime,
                  const msTimeT* const armedTotalTime,
                  uint8_t bombStatus,
                  ledC* armedLed,
                  ledC* defusedLed)
{
  armedLed->turnOff();
  defusedLed->turnOff();

  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("R: ");
  printTime(unarmedTotalTime, false);

  lcd.setCursor(1, 1);
  lcd.print("U: ");
  printTime(armedTotalTime, false);
  
  if(bombStatus == BOMB_ARMED)
  {
    lcd.setCursor(0, 1);
    armedLed->turnOn();
  }
  else
  {
    lcd.setCursor(0, 0);
    defusedLed->turnOn(); 
  }
  lcd.write(byte(126));
  //DEBUG
  //Serial.println("UNARMED: " + (String)*unarmedTotalTime);
  //Serial.println("ARMED: " + (String)*armedTotalTime);
}

void printBombStatus(bombDataS* const data)
{
  lcd.setCursor(FIRST_LCD_CHAR, 1);
  if(data->noBombStatus)
  {
    lcd.print("    ");    
  }
  else
  {
    if(data->currentBombStatus == BOMB_UNARMED)
    {
      lcd.print("UZB.");
    }
    else
    {
      lcd.print("ROZ.");
    }
  }
}

void processNoButtonPush(bombDataS* data, ledC* armedLed, ledC* defusedLed, msTimeT currentTime)
{
  if(data->isButtonsPushed == true)
  {
    data->clearPiontsAndLcd = true;
  }
  else
  {
    if(data->currentBombStatus == BOMB_ARMED)
    {
      defusedLed->turnOff();
      armedLed->blink(currentTime);
    }
    else
    {
      armedLed->turnOff();
      defusedLed->blink(currentTime);
    }
  }

  data->isButtonsPushed = false;
}

void processClearingPoints(const gamemodeBombS* const gm, 
                           bombDataS* const data,
                           gamemodeTiming* timing,
                           classicProgressBarC* armingPG,
                           classicProgressBarC* defusingPG)
{
  if(gm->slowReversing)
  {
    if(data->pointsInMs > 0)
    {
      data->pointsInMs -= (timing->currentTime - timing->lastCurrentTime);
      data->noBombStatus = false;
    }
    if (data->pointsInMs <= 0)
    {
      lcd.setCursor(FIRST_LCD_CHAR, 1);
      lcd.print("    ");
      data->pointsInMs = 0;
      data->noBombStatus = true;
      data->clearPiontsAndLcd = false;
    }
  }
  else
  {
    data->noBombStatus = true;
    data->pointsInMs = 0;
    data->clearPiontsAndLcd = false;     
  }
  
  if(data->currentBombStatus == BOMB_UNARMED)
  {
    armingPG->calculateProgressAndPrintIfDifferent(data->pointsInMs);
  }
  else
  {
    defusingPG->calculateProgressAndPrintIfDifferent(data->pointsInMs);
  }
}

void saveResult(bombHistoryS* const history, 
                const msTimeT* const unarmedTotalTime,
                const msTimeT* const armedTotalTime,
                const uint8_t bombStatus)
{
  for(uint8_t i = MAX_HISTORY_RECORDS - 1; i > 0; i--)
  {
    memcpy(&history[i], &history[i - 1], sizeof(history[0]));
  }
  history[0].unarmedTotalTime = *unarmedTotalTime;
  history[0].armedTotalTime = *armedTotalTime;
  history[0].bombStatus = bombStatus;

  //DEBUG
  //Serial.println("");
  //Serial.println("unarmedTotalTime: " + (String)*unarmedTotalTime);
  //Serial.println("history[0].unarmedTotalTime: " + (String)history[0].unarmedTotalTime);
}

void processBomb(const gamemodeBombS* const gm)
{
  // Initialization
  lcd.clear();
  //TODO move to data

  gamemodeTiming timing;
  bool isFirstIteration = 1;

  bombDataS data;
  memset(&data, 0, sizeof(data));
  data.isAllowedToPushButtons = true;
  
  //printBombGamemodeSettingsOnSerial(gm); //DEBUG

  lcd.setCursor(0, 0);
  lcd.print("POSILKI");

  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);
  data.timeToExplosion = gm->explosionTime;
  data.endGameDefaultTime = timing.endgame;

  classicProgressBarC armingPB(&lcd, gm->armingTime, 10, 6, 1);
  classicProgressBarC defusingPB(&lcd, gm->defusingTime, 10, 6, 1);
  armingPB.calculateProgressAndPrintIfDifferent(data.pointsInMs, FORCE_PRINTING);
  ledC armedLed(12, BLINK_TIME);
  ledC defusedLed(13,BLINK_TIME);

  while (timing.isGameRunning) 
  {
    // 1. Calculating time
    timing.lastCurrentTime = timing.currentTime;
    timing.currentTime = millis();
    calculateTotalTimes(&timing, &data);


    //Serial.print("TIMEDIFF: ");
    //unsigned int tmp = (timing.currentTime - timing.lastCurrentTime);
    //Serial.println(tmp);  //DEBUG
    //Serial.println("CURRENT TIME:" + (String)timing.currentTime);

    // 2. Reading game buttons

    // 2.0 Checking switch status
    if (gm->enableSwitch)
    {
      data.swithStatus = digitalRead(SWITCH);
    }
    // 2.1 Right team
    if (!data.swithStatus)
    {
      if (!digitalRead(RIGHT_TEAM_BUTTON) && 
          !digitalRead(LEFT_TEAM_BUTTON) && 
          data.isAllowedToPushButtons)
      {
        data.clearPiontsAndLcd = false;
        data.noBombStatus = false;
        printBombStatus(&data);

        // continous or first button push
        if (data.isButtonsPushed == true) 
        {
          // Calculating Points
          data.pointsInMs += (timing.currentTime - data.lastPushedButtonTimeStamp);

          if(data.currentBombStatus == BOMB_UNARMED)
          {
            armedLed.turnOn();
            defusedLed.turnOff();

            // changing bomb status to BOMB_ARMED.
            if(data.pointsInMs > gm->armingTime)
            {
              data.currentBombStatus = BOMB_ARMED;
              lcd.setCursor(0, 0);
              lcd.print("WYBUCH ");
              data.pointsInMs = 0;
              data.clearPiontsAndLcd = true;
              beep();
              data.isAllowedToPushButtons = false;

              // process bomb time reset.
              if(gm->explosionTimeReset)
              {
                timing.endgame = timing.currentTime + gm->explosionTime;
              }
              else
              {
                timing.endgame = timing.currentTime + data.timeToExplosion;
              }
            }

            armingPB.calculateProgressAndPrintIfDifferent(data.pointsInMs);
          }
          else // if(data.currentBombStatus == BOMB_ARMED)
          {
            armedLed.turnOff();
            defusedLed.turnOn();

            if(data.pointsInMs > gm->defusingTime)
            {
              data.currentBombStatus = BOMB_UNARMED;
              lcd.setCursor(0, 0);
              lcd.print("POSILKI");
              data.pointsInMs = 0;
              data.clearPiontsAndLcd = true;
              beep();
              data.isAllowedToPushButtons = false;

              // endGame validation
              if(gm->isDefuseEndGame)
              {
                timing.isGameRunning = false;
              }
              else
              {
                timing.endgame = data.endGameDefaultTime;
                data.timeToExplosion = timing.timeLeft;
              }
            }

            defusingPB.calculateProgressAndPrintIfDifferent(data.pointsInMs);
          }
        } 
        else  //first button push
        {
          data.isButtonsPushed = true;
        }

        //save button push timestamp
        data.lastPushedButtonTimeStamp = timing.currentTime;
      }
      // Bomb status changed and buttond need to be released.
      else if (digitalRead(RIGHT_TEAM_BUTTON) && 
               digitalRead(LEFT_TEAM_BUTTON) && 
               data.isAllowedToPushButtons == false)
      {
        data.isAllowedToPushButtons = true;
      }
      else  // no button pushed
      {
        processNoButtonPush(&data, &armedLed, &defusedLed, timing.currentTime);
      }
    }
    else  // Blocked by switch
    {
      processNoButtonPush(&data, &armedLed, &defusedLed, timing.currentTime);
      data.isAllowedToPushButtons = true;
    }

    if(data.clearPiontsAndLcd)
    {
      processClearingPoints(gm, &data, &timing, &armingPB, &defusingPB);
    }

    // Endgame weryfication
    verifyEndGame(&timing, 8, 0);

  } // end of main loop

  saveResult(gm->history,
             &data.unarmedTotalTime,
             &data.armedTotalTime,
             data.currentBombStatus);

  printSummary(&data.unarmedTotalTime,
               &data.armedTotalTime,
               data.currentBombStatus,
               &armedLed,
               &defusedLed);

  processGameSummary(&timing);
}

void printBombGamemodeSettingsOnSerial(const gamemodeBombS* const gm)
{
  Serial.println(" ");
  Serial.println("Bomba ustawienia:");
  Serial.println("gameTime: " + (String)gm->gameTime);
  Serial.println("explosionTime: " + (String)gm->explosionTime);
  Serial.println("armingTime: " + (String)gm->armingTime);
  Serial.println("defusingTime: " + (String)gm->defusingTime);
  Serial.println("enableSwitch: " + (String)gm->enableSwitch);
  Serial.println("isDefuseEndGame: " + (String)gm->isDefuseEndGame);
  Serial.println("slowReversing: " + (String)gm->slowReversing);
}

void printBombHisotry(const bombHistoryS* const history)
{
  bool isButtonPushed = true;
  uint8_t historyRecord = 0;
  ledC armedLed(12, 500);
  ledC defusedLed(13, 500);
  while(true)
  {
    // Reading button status
    if(buttonPushed(UP_BUTTON))
    {
      break;
    }
    if(buttonPushed(RIGHT_BUTTON))
    {
      isButtonPushed = true;

      if (historyRecord == MAX_HISTORY_RECORDS - 1)
      {
        historyRecord = 0;
      }
      else
      {
        historyRecord++;
      }
    }
    if(buttonPushed(LEFT_BUTTON))
    {
      isButtonPushed = true;

      if (historyRecord == 0)
      {
        historyRecord = MAX_HISTORY_RECORDS - 1;
      }
      else
      {
        historyRecord--;
      }
    }

    if(isButtonPushed)
    {
      printSummary(&history[historyRecord].unarmedTotalTime, 
                   &history[historyRecord].armedTotalTime,
                   history[historyRecord].bombStatus,
                   &armedLed,
                   &defusedLed);

      lcd.setCursor(LAST_LCD_CHAR, 0);
      lcd.print(historyRecord + 1);
      isButtonPushed = false;
    }
  }
}