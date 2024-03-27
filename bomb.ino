#include "bomb.hpp"

void bombInitializeProgressBarData(progressBarDataS* data, const gamemodeBombS* const gm)
{
  memset(data, 0, sizeof(progressBarDataS));
  data->ammountOfStages = BAR_TYPE_NO_TEAMS;
  data->takeOverStageSquares = 10;
  data->filledSquared = 0;

  //DEBUG
  Serial.println("");
  Serial.println("TIMING");
  Serial.println("ammountOfStages: " + (String)data->ammountOfStages);
  Serial.println("takeOverStageSquares: " + (String)data->takeOverStageSquares);
  Serial.println("filledSquared: " + (String)data->filledSquared);
}

void calculateFilledSquaresBomb(const gamemodeBombS* const gm, progressBarDataS* progressBarData, const bombDataS* const bombData)
{
  if(bombData->currentBombStatus == BOMB_UNARMED)
  {
    progressBarData->timeOfOneTakeOverSquareFilled = round((float)gm->armingTime / NO_TEAM_PROGRESS_BAR_CHARACTERS);
  }
  else
  {
    progressBarData->timeOfOneTakeOverSquareFilled = round((float)gm->defusingTime / NO_TEAM_PROGRESS_BAR_CHARACTERS);
  }
  //TODO rename filledSquared
  progressBarData->filledSquared = bombData->pointsInMs / progressBarData->timeOfOneTakeOverSquareFilled;
  //DEBUG
  Serial.println("filledSquared: " + (String)progressBarData->filledSquared + " pointsInMs: " + (String)bombData->pointsInMs);
}

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
  //Serial.println("TimeDiff: " + (String)timediff + " unarmedTotalTime: " + (String)bombData->unarmedTotalTime);
}

void printSummary(bombDataS* data)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("R: ");
  printTime(&data->unarmedTotalTime, false);

  lcd.setCursor(0, 1);
  lcd.print("U: ");
  printTime(&data->armedTotalTime, false);

  if(data->currentBombStatus == BOMB_UNARMED)
  {
      lcd.setCursor(15, 0);
  }
  else
  {
      lcd.setCursor(15, 1);   
  }
  lcd.write(byte(127));
  Serial.println("UNARMED: " + (String)data->unarmedTotalTime);
  Serial.println("UNARMED: " + (String)data->armedTotalTime);
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

void processBomb(const gamemodeBombS* const gm) {
  // Initialization
  lcd.clear();
  //TODO move to data
  bool clearPiontsAndBombStatus = false;
  bool isAllowedToPushButtons = true;
  msTimeT pushingTime = 0;
  gamemodeTiming timing;
  bool isFirstIteration = 1;

  progressBarDataS progressBarData;
  bombInitializeProgressBarData(&progressBarData, gm);

  bombDataS data;
  memset(&data, 0, sizeof(data));

  //DEBUG
  printBombGamemodeSettingsOnSerial(gm);

  lcd.setCursor(0, 0);
  lcd.print("POSILKI");

  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);
  data.timeToExplosion = gm->explosionTime;
  data.endGameDefaultTime = timing.endgame;

  // 3. LCD prints
  calculateFilledSquaresBomb(gm, &progressBarData, &data);
  //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
  printProgressBar(&progressBarData);

  while (timing.isGameRunning) 
  {
    // 1. Calculating time
    timing.lastCurrentTime = timing.currentTime;  //DEBUG
    timing.currentTime = millis();
    //Serial.println("currentTime: " + (String)timing.currentTime);
    

    calculateTotalTimes(&timing, &data);


    //Serial.println("TIMEDIFF: " + (String)(timing.currentTime - timing.lastCurrentTime));  //DEBUG
    //Serial.println("CURRENT TIME:" + (String)timing.currentTime);

    // 2. Reading game buttons
    //buttons is set as INPUT_PULLUP mode. If button is pushed digitalRead return 0.

    // 2.0 Checking switch status
    if (gm->enableSwitch)
    {
      data.swithStatus = digitalRead(SWITCH);
    }
    // 2.1 Right team
    if (!data.swithStatus)
    {
      if (!digitalRead(RIGHT_TEAM_BUTTON) && !digitalRead(LEFT_TEAM_BUTTON) && isAllowedToPushButtons)
      {
        clearPiontsAndBombStatus = false;
        // Display button push character
        data.noBombStatus = false;
        printBombStatus(&data);
        // continous or first button push
        if (data.isButtonsPushed == true) 
        {
          // Calculating Points
          data.pointsInMs += (timing.currentTime - data.lastPushedButtonTimeStamp);
          if(data.currentBombStatus == BOMB_UNARMED)
          {
            if(data.pointsInMs > gm->armingTime)
            {
              data.currentBombStatus = BOMB_ARMED;
              lcd.setCursor(0, 0);
              lcd.print("WYBUCH ");
              data.pointsInMs = 0;
              clearPiontsAndBombStatus= true;
              beep();
              isAllowedToPushButtons = false;
              if(gm->explosionTimeReset)
              {
                timing.endgame = timing.currentTime + gm->explosionTime;
              }
              else
              {
                timing.endgame = timing.currentTime + data.timeToExplosion;
              }
            }
            else
            {
              calculateFilledSquaresBomb(gm, &progressBarData, &data);
              //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
              printProgressBar(&progressBarData);
            }
          }
          else
          {
            if(data.pointsInMs > gm->defusingTime)
            {
              data.currentBombStatus = BOMB_UNARMED;
              lcd.setCursor(0, 0);
              lcd.print("POSILKI");
              data.pointsInMs = 0;
              clearPiontsAndBombStatus= true;
              beep();
              isAllowedToPushButtons = false;
              if(gm->isDefuseEndGame)
              {
                timing.isGameRunning = false;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("ROZBROJONO");
              }
              else
              {
                timing.endgame = data.endGameDefaultTime;
                //if(resetczasuwybuchu)
                data.timeToExplosion = timing.timeLeft;
              }
            }
            else
            {
              calculateFilledSquaresBomb(gm, &progressBarData, &data);
              //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
              printProgressBar(&progressBarData);
            }
          }
        } 
        else  //first button push
        {
          data.isButtonsPushed = true;
        }

        data.lastPushedButtonTimeStamp = timing.currentTime;
      }
      else if (digitalRead(RIGHT_TEAM_BUTTON) && digitalRead(LEFT_TEAM_BUTTON) && isAllowedToPushButtons == false)
      {
        isAllowedToPushButtons = true;
      }
      else
      {
        if(data.isButtonsPushed == true)
        {
          clearPiontsAndBombStatus = true;
        }
        data.isButtonsPushed = false;
        //printBombStatus(&data, CLEAR_BOMB_STATUS);
      }
    }
    else  // Blocked by switch
    {
      if(data.isButtonsPushed == true)
      {
        clearPiontsAndBombStatus = true;
      }
      data.isButtonsPushed = false;
      isAllowedToPushButtons = true;
      //printBombStatus(&data, CLEAR_BOMB_STATUS);
      //clearButtonsStatus(&data);
    }

    if(clearPiontsAndBombStatus)
    {
      if(gm->slowReversing)
      {
        if(data.pointsInMs > 0)
        {
          data.pointsInMs -= (timing.currentTime - timing.lastCurrentTime);
          data.noBombStatus = false;
        }
        if (data.pointsInMs <= 0)
        {
          lcd.setCursor(FIRST_LCD_CHAR, 1);
          lcd.print("    ");
          data.pointsInMs = 0;
          data.noBombStatus = true;
          clearPiontsAndBombStatus = false;
        }
      }
      else
      {
        data.noBombStatus = true;
        data.pointsInMs = 0;
        clearPiontsAndBombStatus = false;     
      }
      printBombStatus(&data);
      calculateFilledSquaresBomb(gm, &progressBarData, &data);
      printProgressBar(&progressBarData);
    }

    

    // 4. Endgame weryfication
    // TODO make functions of this part
    verifyEndGame(&timing, 8, 0);
  }
  printSummary(&data);
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