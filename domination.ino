#include "domination.hpp"

int lastRightPointingMoment = 0;
bool isRightButtonWasPushed = false;
int filledRightSquares = 0;
int rightStatus = false;
unsigned int rightTeamProgressBarStage = 0;

unsigned long currentTime = 0;
/////////////////////////////

unsigned long time = 0;
unsigned long lastScreenRefresigh = 0;

int takeOver = 20;
unsigned int takeOverFull = 60;
int squares = 8;

float takeOverToFullTakeOver = (float)takeOver / takeOverFull;

int takeOverSquares = round(squares * takeOverToFullTakeOver);
int takeOverFullSquares = squares - takeOverSquares;
// TAKE OVER
float takeOverfilledPerSec = (float)takeOver / takeOverSquares;
int takeOverfFilledPerMs = takeOverfilledPerSec * 1000;
// FULL TAKE OVER
float takeOverFullfilledPerSec = (float)(takeOverFull - takeOver) / takeOverFullSquares;
int takeOverfFullFilledPerMs = takeOverFullfilledPerSec * 1000;

int righttakeOverSquares = FIRST_RIGHT_PLACE + takeOverSquares - 1;
int lefttakeOverSquares = FIRST_LEFT_PLACE - takeOverSquares + 1;

unsigned long rightTeamPoints = 0;
bool button = true;

void dominationMain()
{
  
    setScren();
    time = millis();
    lastScreenRefresigh = millis();
    Serial.println("takeOverfilledPerSec: " + (String)takeOverfilledPerSec + 
                    " takeOverfFullFilledPerMs: " + (String)takeOverfFullFilledPerMs);
    Serial.println("takeOverSquares: " + (String)takeOverSquares +
                    " takeOverFullSquares: " + (String)takeOverFullSquares);

  // main loop
  bool isGameKeepRunning = true;
  while (isGameKeepRunning)
  {
    currentTime = millis();
    if (button == true)
    {
      if (isRightButtonWasPushed)
      {
        int timediff = currentTime - lastRightPointingMoment;
        rightTeamPoints += timediff;

        if (rightTeamPoints > (takeOverFull * 1000))
        {
          rightTeamPoints = takeOverFull * 1000;
        }

        if (rightTeamPoints > (takeOver * 1000))
        {
          rightStatus = true;
          rightTeamProgressBarStage = 1;
        }
        else
        {
          rightStatus = false;
          rightTeamProgressBarStage = 0;
        }
      }

      isRightButtonWasPushed = true;
      lastRightPointingMoment = currentTime;
    }
    else
    {
      bool isRightButtonWasPushed = false;
    }
    // END OF BUTTON PUSHED





    if ((lastScreenRefresigh + 125) < currentTime)
    {
      if (rightTeamProgressBarStage == 0)
      {
        filledRightSquares = rightTeamPoints / takeOverfFilledPerMs;
      }
      else if (rightTeamProgressBarStage == 1)
      {
        filledRightSquares = ((rightTeamPoints - (takeOver * 1000)) / takeOverfFullFilledPerMs);
        filledRightSquares += takeOverSquares;
      }

      Serial.println("rightTeamPoints: " + String(rightTeamPoints) + " filledRightSquares: " + String(filledRightSquares));
      lastScreenRefresigh = currentTime;

      if (rightStatus)
      {
        lcd.setCursor(14, 0);
        lcd.print("T");
      }
      int charToPrint = CHAR_BOTTOM;
      int modifiedSquare = 0;

      for (int i = 0; i < filledRightSquares; i++)
      {
        modifiedSquare = FIRST_RIGHT_PLACE + i;

        if ((modifiedSquare == FIRST_LCD_CHAR) ||
            (modifiedSquare == lefttakeOverSquares) ||
            (modifiedSquare == FIRST_RIGHT_PLACE))
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = CHAR_LEFT_STRIP_FILLED;
        }
        else if ((modifiedSquare == LAST_LCD_CHAR) ||
                 (modifiedSquare == righttakeOverSquares) ||
                 (modifiedSquare == FIRST_LEFT_PLACE))
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = CHAR_RIGHT_STRIP_FILLED;
        }
        else
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = CHAR_FILLED;
        }
        lcd.write(byte(charToPrint));
      }
    }

    // TODO loop break
  }
}

void setScren()
{
  if ((takeOver == 0) && (takeOverFull == 0))
  {
    takeOverToFullTakeOver = 0;
  }
  lcd.clear();
  int charToPrint = CHAR_BOTTOM;
  for (int i = 0; i <= 15; i++)
  {
    if ((i == FIRST_LCD_CHAR) || (i == lefttakeOverSquares) || (i == FIRST_RIGHT_PLACE))
    {
      lcd.setCursor(i, 1);
      charToPrint = CHAR_LEFT_STRIP;
    }
    else if ((i == LAST_LCD_CHAR) || (i == righttakeOverSquares) || (i == FIRST_LEFT_PLACE))
    {
      lcd.setCursor(i, 1);
      charToPrint = CHAR_RIGHT_STRIP;
    }
    else
    {
      lcd.setCursor(i, 1);
      charToPrint = CHAR_BOTTOM;
    }
    lcd.write(byte(charToPrint));
  }
  Serial.println("Screen is set");
}

void printWinningPoints(const unsigned int* const leftTeamWinningPoints, const unsigned int* const rightTeamWinningPoints, bool printOnLowerPart)
{
  // Printing left team points
  lcd.setCursor(FIRST_LCD_CHAR, printOnLowerPart);
  lcd.print(*leftTeamWinningPoints);
  // Calculating lcd position
  uint8_t rightLcdPos = LAST_LCD_CHAR;
  if(*rightTeamWinningPoints > 9)
  {
    rightLcdPos -= 1;
  }
  else if(*rightTeamWinningPoints > 99)
  {
    rightLcdPos -= 2;
  }
  else if(*rightTeamWinningPoints > 999)
  {
    rightLcdPos -= 3;
  }
  // Printing right team points
  lcd.setCursor(rightLcdPos, printOnLowerPart);
  lcd.print(*rightTeamWinningPoints);
}

void printSummary(const msTimeT* const gameTime, 
                  const msTimeT* const runningTime,
                  const unsigned int* const leftTeamWinningPoints,
                  const unsigned int* const rightTeamWinningPoints)
{
  // Printing passed Time
  msTimeT passedTime = *gameTime - *runningTime;
  lcd.clear();
  lcd.setCursor(FIRST_LCD_CHAR, 0);
  lcd.print("CZAS: ");
  printTime(&passedTime, false);

  printWinningPoints(leftTeamWinningPoints, rightTeamWinningPoints, true);
  
  // Printing winner signs
  if(*leftTeamWinningPoints >= *rightTeamWinningPoints)
  {
    lcd.setCursor(FIRST_LEFT_PLACE, 1);
    lcd.write(byte(127));
  }
  if(*rightTeamWinningPoints >= *leftTeamWinningPoints)
  {
    lcd.setCursor(FIRST_RIGHT_PLACE, 1);
    lcd.write(byte(126));
  }

}

void initializeProgressBarData(progressBarDataS* data, const gamemodeDominationS* const gm)
{
  data->ammountOfStages = TWO_TEAMS_FOUR_ZONES;
  data->secondStageSquares =  round(8 * ((float)gm->takeOverTime / gm->fullTakeOverTime));
  data->filledSite = 1;
  data->timeOfOneTakeOverSquareFilled = (float)gm->takeOverTime / data->secondStageSquares;
  data->timeOfOneFullTakeOverSquareFilled = (float)(gm->fullTakeOverTime - gm->takeOverTime) / (8 - data->secondStageSquares);

  Serial.println(" ");
  Serial.println("PROGRES BAR DATA:");
  Serial.println("ammountOfStages: " + (String)data->ammountOfStages);
  Serial.println("secondStageSquares: " + (String)data->secondStageSquares);
  Serial.println("filledSquared: " + (String)data->filledSquared);
  Serial.println("filledSite: " + (String)data->filledSite);
  Serial.println("timeOfOneTakeOverSquareFilled: " + (String)data->timeOfOneTakeOverSquareFilled); 
}

void calculateFilledSquares(const gamemodeDominationS* const gm, progressBarDataS* progressBarData, const long int* const pointsInMs)
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
    progressBarData->filledSquared = ((absPointsInMs -  gm->takeOverTime) / progressBarData->timeOfOneFullTakeOverSquareFilled);
    progressBarData->filledSquared += progressBarData->secondStageSquares;
  }
    //Serial.println("POINST: " + (String)*pointsInMs + " " + absPointsInMs);
    //Serial.println("FILLED SQUARES: " +(String)progressBarData->filledSquared);

}

void processDomination(const gamemodeDominationS* const gm)
{
  lcd.clear();
  bool isGameRunning = true;
  gamemodeTiming timing;
  initializeTiming(&timing, &gm->gameTime);

  progressBarDataS progressBarData;
  initializeProgressBarData(&progressBarData, gm);

  dominationDataS data;
  memset(&data, 0, sizeof(data));

  while(isGameRunning)
  {
    timing.currentTime = millis();
    //Serial.println("TIMEDIFF: " + (String)(timing.currentTime - timing.lastCurrentTime)); //DEBUG
    timing.lastCurrentTime = timing.currentTime; //DEBUG

    //Reading game buttons
    //buttons is set as INPUT_PULLUP mode. If button is pushed digitalRead return 0.
    // Right team
    if(!digitalRead(RIGHT_TEAM_BUTTON) && digitalRead(LEFT_TEAM_BUTTON))
    {
      lcd.setCursor(12, 0);
      lcd.write(byte(126));
      // Button pushed
      if(data.isRightButtonPushed == true)
      {
        // Calculating Points
        data.pointsInMs += (timing.currentTime - data.lastPushedButtonTimeStamp);
        if (data.pointsInMs > gm->fullTakeOverTime)
        {
          data.pointsInMs = gm->fullTakeOverTime;
        }
        // Setting pointing site
        if (data.pointingTeam != POINTING_RIGHT_TEAM)
        {
          if (data.pointsInMs > gm->takeOverTime)
          {
            data.pointingTeam = POINTING_RIGHT_TEAM;
            data.addPointTimeStamp = timing.currentTime + gm->pointTime;

                // DEBUG
            Serial.println("");
            Serial.println("BUTTON");
            Serial.print("pointingTeam: " + (String)data.pointingTeam);
            Serial.print(" currentTime: " + (String)timing.currentTime); 
            Serial.println(" addPointTimeStamp: " + (String)data.addPointTimeStamp);   
          }
          else
          {
            data.pointingTeam = POINTING_NONE;
          }
        }
      }
      else
      {
        data.isRightButtonPushed = true;
        data.isLeftButtonPushed = false;
      }
      data.lastPushedButtonTimeStamp = timing.currentTime;
    }
    // LEFT TEAM
    else if(!digitalRead(LEFT_TEAM_BUTTON) && digitalRead(RIGHT_TEAM_BUTTON))
    {
      lcd.setCursor(3, 0);
      lcd.write(byte(127));
      if(data.isLeftButtonPushed == true)
      {
        data.pointsInMs -= (timing.currentTime - data.lastPushedButtonTimeStamp);
        if (data.pointingTeam != POINTING_LEFT_TEAM)
        {
          if (data.pointsInMs < -(gm->takeOverTime))
          {
            data.pointingTeam = POINTING_LEFT_TEAM;
            data.addPointTimeStamp = timing.currentTime + gm->pointTime;

                  // DEBUG
              Serial.println("");
              Serial.println("BUTTON");
              Serial.print("pointingTeam: " + (String)data.pointingTeam);
              Serial.print(" currentTime: " + (String)timing.currentTime); 
              Serial.println(" addPointTimeStamp: " + (String)data.addPointTimeStamp);   
          }
          else
          {
            data.pointingTeam = POINTING_NONE;
          }
        }
        if (data.pointsInMs < (gm->fullTakeOverTime * -1))
        {
          data.pointsInMs = (gm->fullTakeOverTime * -1);
        }
      }
      else
      {
        data.isRightButtonPushed = false;
        data.isLeftButtonPushed = true;
      }
      data.lastPushedButtonTimeStamp = timing.currentTime;
    }
    else
    {
      lcd.setCursor(3, 0);
      lcd.print(" ");
      lcd.setCursor(12, 0);
      lcd.print(" ");
      data.isRightButtonPushed = false;
      data.isLeftButtonPushed = false;
    }


    // calculating winning points
    if(data.pointingTeam != POINTING_NONE)
    {
      if(timing.currentTime > data.addPointTimeStamp)
      {
        if(data.pointingTeam == POINTING_LEFT_TEAM)
        {
          data.leftTeamWinningPoints += 1;
        }
        else
        {
          data.rightTeamWinningPoints += 1;
        }
        data.addPointTimeStamp += gm->pointTime;

            // DEBUG
        Serial.println("");
        Serial.println("AUTO");
        Serial.print("pointingTeam: " + (String)data.pointingTeam);
        Serial.print(" currentTime: " + (String)timing.currentTime); 
        Serial.println(" addPointTimeStamp: " + (String)data.addPointTimeStamp);   
      }
    } 

    printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
    calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
    //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
    printProgressBar(&progressBarData);

    isGameRunning = valideateEndGameOrPrintTimeLeft(&timing);
    // Reading steering buttons
    if(!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON))
    {
      isGameRunning = false;
    }
  }

  // Printing summary
  printSummary(&gm->gameTime, &timing.timeLeft, &data.leftTeamWinningPoints, &data.rightTeamWinningPoints);
  delay(2000);
  while(true)
  {
    if(!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON))
    {
      break;
    }
  }
  delay(500);
}

void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm)
{
    Serial.println("Dominacja ustawienia:");
    Serial.println("gameTime: " + (String)gm->gameTime);
    Serial.println("takeOverTime: " + (String)gm->takeOverTime);
    Serial.println("fullTakeOverTime: " + (String)gm->fullTakeOverTime);
    Serial.println("enableSwitch: " + (String)gm->enableSwitch);   
}