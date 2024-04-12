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
                  const unsigned int* const rightTeamWinningPoints,
                  ledC* leftLed,
                  ledC* rightLed)
{
  // Printing passed Time
  msTimeT passedTime = (*gameTime != 0) ? *gameTime - *runningTime : *runningTime;
  lcd.clear();
  lcd.setCursor(FIRST_LCD_CHAR, 0);
  lcd.print("CZAS: ");
  printTime(&passedTime, false);

  printWinningPoints(leftTeamWinningPoints, rightTeamWinningPoints, true);

  // Printing winner signs
  leftLed->turnOff();
  rightLed->turnOff();
  if (*leftTeamWinningPoints >= *rightTeamWinningPoints)
  {
    leftLed->turnOn();
    lcd.setCursor(LCD_MIDDLE_LEFT, 1);
    lcd.write(byte(127));
  }
  if (*rightTeamWinningPoints >= *leftTeamWinningPoints) 
  {
    rightLed->turnOn();
    lcd.setCursor(LCD_MIDDLE_RIGHT, 1);
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

void saveResult(dominationHistoryS* const history, 
                const msTimeT* const gameTime,
                const msTimeT* const runningTime,
                const unsigned int* const leftTeamWinningPoints, 
                const unsigned int* const rightTeamWinningPoints)
{
  for(uint8_t i = MAX_HISTORY_RECORDS - 1; i > 0; i--)
  {
    memcpy(&history[i], &history[i - 1], sizeof(history[0]));
  }
  history[0].gameTime = *gameTime;
  history[0].runningTime = *runningTime;
  history[0].leftTeamWinningPoints = *leftTeamWinningPoints;
  history[0].rightTeamWinningPoints = *rightTeamWinningPoints;
}

void processDomination(gamemodeDominationS* const gm)
{
  // Initialization
  lcd.clear();

  gamemodeTiming timing;
  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);

  dominationDataS data;
  memset(&data, 0, sizeof(data));

  twoDeviationProgressBarC progressBar(&lcd, gm->fullTakeOverTime, gm->takeOverTime, 1);
  progressBar.calculateProgressAndPrintIfDifferent(data.pointsInMs, FORCE_PRINTING);
  ledC leftLed(12, BLINK_TIME);
  ledC rightLed(13, BLINK_TIME);
  
  // LCD prints
  printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);

  //DEBUG
  printGamemodeSettingsOnSerial(gm);

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


    if (!data.swithStatus)
    {
      // 2.1 Right team
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
            rightLed.turnOn();
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
              leftLed.turnOff();
              data.pointingTeam = POINTING_NONE;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            }
          }
          progressBar.calculateProgressAndPrintIfDifferent(data.pointsInMs);
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
            leftLed.turnOn();
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
              rightLed.turnOff();
              data.pointingTeam = POINTING_NONE;
              beep();
              Serial.println("BEEP" + (String)timing.currentTime);
            }
          }
          // 3. LCD prints
          //printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
          progressBar.calculateProgressAndPrintIfDifferent(data.pointsInMs);
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
    if(abs(data.pointsInMs) != gm->fullTakeOverTime)
    {
      if(data.pointingTeam == 1)
      {
        leftLed.blink(timing.currentTime);
      }
      else if(data.pointingTeam == 2)
      {
        rightLed.blink(timing.currentTime);
      }
    }
    //Serial.println("pointsInMs: " + (String)data.pointsInMs + " pointingTeam: " + (String)data.pointingTeam);
    //Serial.println("LP: " + (String)data.leftTeamWinningPoints + " RP: " + (String)data.rightTeamWinningPoints);
    // 4. Endgame weryfication
    verifyEndGame(&timing, 4, 0);
  }  // End of main loop

  // 5. Printing summary
  printSummary(&gm->gameTime, &timing.timeLeft, &data.leftTeamWinningPoints, &data.rightTeamWinningPoints, &leftLed, &rightLed);
  saveResult(gm->history, &gm->gameTime, &timing.timeLeft, &data.leftTeamWinningPoints, &data.rightTeamWinningPoints);
  processGameSummary(&timing);
}

void printDominationHisotry(const dominationHistoryS* const history)
{
  bool isButtonPushed = true;
  uint8_t historyRecord = 0;
  ledC leftLed(12, 0);
  ledC rightLed(13, 0);
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
      printSummary(&history[historyRecord].gameTime, 
                    &history[historyRecord].runningTime,
                    &history[historyRecord].leftTeamWinningPoints,
                    &history[historyRecord].rightTeamWinningPoints,
                    &leftLed,
                    &rightLed);
      lcd.setCursor(LAST_LCD_CHAR, 0);
      lcd.print(historyRecord + 1);
      isButtonPushed = false;
      Serial.println("OK");
    }
  }
}