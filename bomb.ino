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

void printBombStatus(bombDataS* const data, uint8_t printBombStatus = true)
{
  lcd.setCursor(FIRST_LCD_CHAR, 1);
  if(printBombStatus == true)
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
  else
  {
    lcd.print("    ");
    data->pointsInMs = 0;
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

  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);

  //MoveToGameData
  msTimeT explosionTimeToSave = gm->explosionTime;
  msTimeT endGameTimeToSave = timing.endgame;

  progressBarDataS progressBarData;
  bombInitializeProgressBarData(&progressBarData, gm);

  bombDataS data;
  memset(&data, 0, sizeof(data));

  lcd.setCursor(0, 0);
  lcd.print("POSILKI");

  //DEBUG
  printBombGamemodeSettingsOnSerial(gm);
  
  // 3. LCD prints
  calculateFilledSquaresBomb(gm, &progressBarData, &data);
  //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
  printProgressBar(&progressBarData);

  while (timing.isGameRunning) 
  {
    // 1. Calculating time
    timing.currentTime = millis();
    //Serial.println("TIMEDIFF: " + (String)(timing.currentTime - timing.lastCurrentTime));  //DEBUG
    //Serial.println("CURRENT TIME:" + (String)timing.currentTime);
    timing.lastCurrentTime = timing.currentTime;  //DEBUG

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
        // Display button push character
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
              clearPiontsAndBombStatus= true;
              beep();
              isAllowedToPushButtons = false;
              timing.endgame = timing.currentTime + explosionTimeToSave;
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
                timing.endgame = endGameTimeToSave;
                //if(resetczasuwybuchu)
                explosionTimeToSave = timing.timeLeft;
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
      printBombStatus(&data, CLEAR_BOMB_STATUS);
      calculateFilledSquaresBomb(gm, &progressBarData, &data);
      printProgressBar(&progressBarData);
      clearPiontsAndBombStatus = false;
    }

    // 4. Endgame weryfication
    // TODO make functions of this part
    verifyEndGame(&timing, 8, 0);
  }
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