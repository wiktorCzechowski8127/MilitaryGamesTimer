#include "domination.hpp"

void printWinningPoints(const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints, bool printOnLowerPart) {
  // Printing left team points
  lcd.setCursor(FIRST_LCD_CHAR, printOnLowerPart);
  lcd.print(*leftTeamWinningPoints);

  // Calculating lcd position
  uint8_t rightLcdPos = LAST_LCD_CHAR;
  String stringToPrint = (String)*rightTeamWinningPoints;

  if ((*rightTeamWinningPoints >= 10) && (*rightTeamWinningPoints < 100)) {
    rightLcdPos -= 1;
  } else if ((*rightTeamWinningPoints >= 100) && (*rightTeamWinningPoints < 1000)) {
    rightLcdPos -= 2;
  } else if ((*rightTeamWinningPoints > 1000) && (printOnLowerPart == true)) {
    rightLcdPos -= 3;
  } else if ((*rightTeamWinningPoints > 1000) && (printOnLowerPart == false)) {
    rightLcdPos -= 2;
    stringToPrint = WINNING_POINTS_OUT_OF_RANGE;
  }
  // Printing right team points
  lcd.setCursor(rightLcdPos, printOnLowerPart);
  lcd.print((String)stringToPrint);
}

void printSummary(const msTimeT* const gameTime,
                  const msTimeT* const runningTime,
                  const unsigned int* const leftTeamWinningPoints,
                  const unsigned int* const rightTeamWinningPoints)
{
  // Printing passed Time
  msTimeT passedTime = (*gameTime != 0) ? *gameTime - *runningTime : *runningTime;
  lcd.clear();
  lcd.setCursor(FIRST_LCD_CHAR, 0);
  lcd.print("CZAS: ");
  printTime(&passedTime, false);

  printWinningPoints(leftTeamWinningPoints, rightTeamWinningPoints, true);

  // Printing winner signs
  if (*leftTeamWinningPoints >= *rightTeamWinningPoints)
  {
    lcd.setCursor(FIRST_LEFT_PLACE, 1);
    lcd.write(byte(127));
  }
  if (*rightTeamWinningPoints >= *leftTeamWinningPoints) {
    lcd.setCursor(FIRST_RIGHT_PLACE, 1);
    lcd.write(byte(126));
  }
}

void clearButtonsStatus(dominationDataS* data) {
  lcd.setCursor(3, 0);
  lcd.print(" ");
  lcd.setCursor(12, 0);
  lcd.print(" ");
  data->isLeftButtonPushed = false;
  data->isRightButtonPushed = false;
}

bool validateWinningPoints(const unsigned int* const points, const unsigned int* const limit, gamemodeTiming* timing)
{
  if (*points >= *limit) {
    digitalWrite(RELAY, RELAY_ON);
    timing->turnSpeakerAlarmOn = true;
    return false;
  }
  return true;
}

void dominationInitializeProgressBarData(progressBarDataS* data, const gamemodeDominationS* const gm){
  if (gm->takeOverTime != gm->fullTakeOverTime) 
  {
    data->ammountOfStages = BAR_TYPE_TWO_TEAMS;
    data->takeOverStageSquares = round(AMMOUNT_OF_SQUARES_PER_TEAM * ((float)gm->takeOverTime / gm->fullTakeOverTime));
    if (data->takeOverStageSquares == AMMOUNT_OF_SQUARES_PER_TEAM) {
      data->takeOverStageSquares--;
    } 
    else if (data->takeOverStageSquares == 0) 
    {
      data->takeOverStageSquares++;
    }
  } 
  else
  {
    data->ammountOfStages = BAR_TYPE_TWO_TEAMS;
    data->takeOverStageSquares = AMMOUNT_OF_SQUARES_PER_TEAM;
  }

  data->filledSite = 1;
  data->timeOfOneTakeOverSquareFilled = (float)gm->takeOverTime / data->takeOverStageSquares;
  data->timeOfOneFullTakeOverSquareFilled = (float)(gm->fullTakeOverTime - gm->takeOverTime) / (8 - data->takeOverStageSquares);

  //DEBUG
  /*
  Serial.println(" ");
  Serial.println("PROGRES BAR DATA:");
  Serial.println("ammountOfStages: " + (String)data->ammountOfStages);
  Serial.println("takeOverStageSquares: " + (String)data->takeOverStageSquares);
  Serial.println("filledSquared: " + (String)data->filledSquared);
  Serial.println("filledSite: " + (String)data->filledSite);
  Serial.println("timeOfOneTakeOverSquareFilled: " + (String)data->timeOfOneTakeOverSquareFilled); 
  Serial.println("timeOfOneFullTakeOverSquareFilled: " + (String)data->timeOfOneFullTakeOverSquareFilled);
  */
}

void calculateFilledSquares(const gamemodeDominationS* const gm, progressBarDataS* progressBarData, const long int* const pointsInMs) 
{
  if (*pointsInMs != 0)
  {
    long int absPointsInMs = abs(*pointsInMs);
    if (*pointsInMs > 0)
    {
      progressBarData->filledSite = 1;
    }
    else 
    {
      progressBarData->filledSite = 0;
    }

    if (absPointsInMs <= gm->takeOverTime) 
    {
      progressBarData->filledSquared = absPointsInMs / progressBarData->timeOfOneTakeOverSquareFilled;
    }
    else 
    {
      progressBarData->filledSquared = ((absPointsInMs - gm->takeOverTime) / progressBarData->timeOfOneFullTakeOverSquareFilled);
      progressBarData->filledSquared += progressBarData->takeOverStageSquares;
    }
    //Serial.println("POINST: " + (String)*pointsInMs + " " + absPointsInMs);
    //Serial.println("FILLED SQUARES: " +(String)progressBarData->filledSquared);
  }
  else 
  {
    progressBarData->filledSquared = 0;
  }
}

bool calcWinningPointsAndCheckIsGameEnd(gamemodeTiming* timing, dominationDataS* data, gamemodeDominationS* gm)
{
  bool gameKeepRunnung = true;
  if (data->pointingTeam != POINTING_NONE)
  {
    if (timing->currentTime > data->addPointTimeStamp)
    {
      if (data->pointingTeam == POINTING_LEFT_TEAM)
      {
        data->leftTeamWinningPoints += 1;
        gameKeepRunnung = validateWinningPoints(&data->leftTeamWinningPoints, &gm->winningPointsLimit, timing);
      }
      else  // == POINTING_RIGHT_TEAM
      {
        data->rightTeamWinningPoints += 1;
        gameKeepRunnung = validateWinningPoints(&data->rightTeamWinningPoints, &gm->winningPointsLimit, timing);
      }
      data->addPointTimeStamp += gm->pointTime;
      printWinningPoints(&data->leftTeamWinningPoints, &data->rightTeamWinningPoints, false);
    }
  }
  return gameKeepRunnung;
}

void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm) {
  Serial.println(" ");
  Serial.println("Dominacja ustawienia:");
  Serial.println("gameTime: " + (String)gm->gameTime);
  Serial.println("takeOverTime: " + (String)gm->takeOverTime);
  Serial.println("fullTakeOverTime: " + (String)gm->fullTakeOverTime);
  Serial.println("winningPointsLimit: " + (String)gm->winningPointsLimit);
  Serial.println("enableSwitch: " + (String)gm->enableSwitch);
  Serial.println("alarmSpeaker: " + (String)gm->alarmSpeaker);
}

void processDomination(const gamemodeDominationS* const gm)
{
  // Initialization
  lcd.clear();
  gamemodeTiming timing;
  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);

  progressBarDataS progressBarData;
  dominationInitializeProgressBarData(&progressBarData, gm);

  dominationDataS data;
  memset(&data, 0, sizeof(data));

  //DEBUG
  printGamemodeSettingsOnSerial(gm);

  // LCD prints
  printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
  calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
  //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
  printProgressBar(&progressBarData);

  // Main game loop
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
      if (!digitalRead(RIGHT_TEAM_BUTTON) && digitalRead(LEFT_TEAM_BUTTON))
      {
        // Display button push character
        lcd.setCursor(12, 0);
        lcd.write(byte(126));

        // continous or first button push
        if (data.isRightButtonPushed == true)
        {
          // Calculating Points
          data.pointsInMs += (timing.currentTime - data.lastPushedButtonTimeStamp);
          if (data.pointsInMs > gm->fullTakeOverTime)
          {
            data.pointsInMs = gm->fullTakeOverTime;
            beep();
          }

          // Setting pointing site
          if (data.pointingTeam != POINTING_RIGHT_TEAM)
          {
            if (data.pointsInMs >= gm->takeOverTime)
            {
              data.pointingTeam = POINTING_RIGHT_TEAM;
              data.addPointTimeStamp = timing.currentTime + gm->pointTime;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            } 
            else if ((data.pointsInMs > -(gm->takeOverTime)) && (data.pointingTeam != POINTING_NONE))  // Button pushed when pointingTeam == POINTING_LEFT_TEAM
            {
              data.pointingTeam = POINTING_NONE;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            }
          }
          calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
          //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
          printProgressBar(&progressBarData);
        }
        else  //first button push
        {
          data.isRightButtonPushed = true;
          data.isLeftButtonPushed = false;
        }
        data.lastPushedButtonTimeStamp = timing.currentTime;
      }
      // 2.2 Left Team
      else if (!digitalRead(LEFT_TEAM_BUTTON) && digitalRead(RIGHT_TEAM_BUTTON)) 
      {
        // Display button push character
        lcd.setCursor(3, 0);
        lcd.write(byte(127));

        // continous or first button push
        if (data.isLeftButtonPushed == true)
        {
          // Calculating Points
          data.pointsInMs -= (timing.currentTime - data.lastPushedButtonTimeStamp);
          if (data.pointsInMs < (gm->fullTakeOverTime * -1))
          {
            data.pointsInMs = (gm->fullTakeOverTime * -1);
            beep();
          }

          // Setting pointing site
          if (data.pointingTeam != POINTING_LEFT_TEAM) 
          {
            if (data.pointsInMs <= -(gm->takeOverTime)) 
            {
              data.pointingTeam = POINTING_LEFT_TEAM;
              data.addPointTimeStamp = timing.currentTime + gm->pointTime;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            } 
            else if ((data.pointsInMs < gm->takeOverTime) && (data.pointingTeam != POINTING_NONE))  // Button pushed when pointingTeam == POINTING_LEFT_TEAM
            {
              data.pointingTeam = POINTING_NONE;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            }
          }
          // 3. LCD prints
          //printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
          calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
          //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
          printProgressBar(&progressBarData);
        } 
        else  //first button push
        {
          data.isRightButtonPushed = false;
          data.isLeftButtonPushed = true;
        }

        data.lastPushedButtonTimeStamp = timing.currentTime;
      } 
      else  // No button pushed
      {
        clearButtonsStatus(&data);
      }
    } 
    else  // Blocked by switch
    {
      clearButtonsStatus(&data);
    }

    timing.isGameRunning = calcWinningPointsAndCheckIsGameEnd(&timing, &data, gm);
    //Serial.println("pointsInMs: " + (String)data.pointsInMs + " pointingTeam: " + (String)data.pointingTeam);
    //Serial.println("LP: " + (String)data.leftTeamWinningPoints + " RP: " + (String)data.rightTeamWinningPoints);
    // 4. Endgame weryfication
    verifyEndGame(&timing, 4, 0);
  }  // End of main loop

  // 5. Printing summary
  printSummary(&gm->gameTime, &timing.timeLeft, &data.leftTeamWinningPoints, &data.rightTeamWinningPoints);
  processGameSummary(&timing);
 
}
