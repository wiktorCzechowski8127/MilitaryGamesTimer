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
      int charToPrint = BOTTOM;
      int modifiedSquare = 0;

      for (int i = 0; i < filledRightSquares; i++)
      {
        modifiedSquare = FIRST_RIGHT_PLACE + i;

        if ((modifiedSquare == FIRST_LCD_CHAR) ||
            (modifiedSquare == lefttakeOverSquares) ||
            (modifiedSquare == FIRST_RIGHT_PLACE))
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = LEFT_STRIP_FILLED;
        }
        else if ((modifiedSquare == LAST_LCD_CHAR) ||
                 (modifiedSquare == righttakeOverSquares) ||
                 (modifiedSquare == FIRST_LEFT_PLACE))
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = RIGHT_STRIP_FILLED;
        }
        else
        {
          lcd.setCursor(modifiedSquare, 1);
          charToPrint = FILLED;
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
  int charToPrint = BOTTOM;
  for (int i = 0; i <= 15; i++)
  {
    if ((i == FIRST_LCD_CHAR) || (i == lefttakeOverSquares) || (i == FIRST_RIGHT_PLACE))
    {
      lcd.setCursor(i, 1);
      charToPrint = LEFT_STRIP;
    }
    else if ((i == LAST_LCD_CHAR) || (i == righttakeOverSquares) || (i == FIRST_LEFT_PLACE))
    {
      lcd.setCursor(i, 1);
      charToPrint = RIGHT_STRIP;
    }
    else
    {
      lcd.setCursor(i, 1);
      charToPrint = BOTTOM;
    }
    lcd.write(byte(charToPrint));
  }
  Serial.println("Screen is set");
}



void processDomination(const gamemodeDominationS* const gm)
{
  lcd.clear();
  bool isGameRunning = true;
  gamemodeTiming timing;
  initializeTiming(&timing, &gm->gameTime);
  
  long int pointsInMs = 0;
  unsigned long lastPushedButtonTimeStamp = 0;
  bool isRightButtonPushed = 0;
  bool isLeftButtonPushed = 0;
  int gameStatus = 0;
  while(isGameRunning)
  {
    timing.currentTime = millis();
    //Serial.println("TIMEDIFF: " + (String)(timing.currentTime - timing.lastCurrentTime)); //DEBUG
    timing.lastCurrentTime = timing.currentTime; //DEBUG

    //buttons is set as INPUT_PULLUP mode. If button is pushed digitalRead return 0.
    if(!digitalRead(RIGHT_TEAM_BUTTON) && digitalRead(LEFT_TEAM_BUTTON))
    {
      Serial.println("RIGHT_TEAM");
      if(isRightButtonPushed == true)
      {
        pointsInMs += (timing.currentTime - lastPushedButtonTimeStamp);
        if (pointsInMs > (long int)gm->fullTakeOverTime)
        {
          pointsInMs = gm->fullTakeOverTime;
        }
      }
      else
      {
        isRightButtonPushed = true;
        isLeftButtonPushed = false;
      }
      lastPushedButtonTimeStamp = timing.currentTime;
    }
    else if(!digitalRead(LEFT_TEAM_BUTTON) && digitalRead(RIGHT_TEAM_BUTTON))
    {
      Serial.println("LEFT_TEAM");
      if(isLeftButtonPushed == true)
      {
        pointsInMs -= (timing.currentTime - lastPushedButtonTimeStamp);
        Serial.println("LPOINST: " + (String)pointsInMs);
        if (pointsInMs < (long int)(gm->fullTakeOverTime * -1))
        {
          pointsInMs = (gm->fullTakeOverTime * -1);
        }
      }
      else
      {
        isRightButtonPushed = false;
        isLeftButtonPushed = true;
      }
      lastPushedButtonTimeStamp = timing.currentTime;
    }
    else
    {
      isRightButtonPushed = false;
      isLeftButtonPushed = false;
    }


    Serial.println("POINST: " + (String)pointsInMs);
    isGameRunning = valideateEndGameOrPrintTimeLeft(&timing);
  }
}

void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm)
{
    Serial.println("Dominacja ustawienia:");
    Serial.println("gameTime: " + (String)gm->gameTime);
    Serial.println("takeOverTime: " + (String)gm->takeOverTime);
    Serial.println("fullTakeOverTime: " + (String)gm->fullTakeOverTime);
    Serial.println("enableSwitch: " + (String)gm->enableSwitch);   
}