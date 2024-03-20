#include "lcd.hpp"

void createProgressBarCharacters()
{
    // Both sites characters
    byte filled[8] = {B00000,
                      B00000,
                      B00000,
                      B11111,
                      B11111,
                      B11111,
                      B11111,
                      B11111,};

    byte bottomStrip[8] = {B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B00000,
                           B11111,};

    // Site characters
    byte rightStrip[8] = {B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B00001,
                          B11111,};

    byte rightStripFilled[8] = {B00001,
                                B00001,
                                B00001,
                                B11111,
                                B11111,
                                B11111,
                                B11111,
                                B11111,};

    byte leftStrip[8] = {B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B10000,
                         B11111,};

    byte leftStripFilled[8] = {B10000,
                               B10000,
                               B10000,
                               B11111,
                               B11111,
                               B11111,
                               B11111,
                               B11111,};

    byte twoStrips[8] = {B10001,
                         B10001,
                         B10001,
                         B10001,
                         B10001,
                         B10001,
                         B10001,
                         B11111,};

    byte twoStripsFilled[8] = {B10001,
                               B10001,
                               B10001,
                               B11111,
                               B11111,
                               B11111,
                               B11111,
                               B11111,};

    // Creating chars
    lcd.createChar(CHAR_FILLED, filled);
    lcd.createChar(CHAR_RIGHT_STRIP, rightStrip);
    lcd.createChar(CHAR_RIGHT_STRIP_FILLED, rightStripFilled);
    lcd.createChar(CHAR_LEFT_STRIP, leftStrip);
    lcd.createChar(CHAR_LEFT_STRIP_FILLED, leftStripFilled);
    lcd.createChar(CHAR_BOTTOM, bottomStrip);
    lcd.createChar(CHAR_TWO_STRIPS, twoStrips);
    lcd.createChar(CHAR_TWO_STRIPS_FILLED, twoStripsFilled);
}

void printtwoZonesProgresBar(const progressBarDataS* const data)
{
  char charToPrint = CHAR_BOTTOM;

  // LEFT SITE
  for(int i = TWO_ZONES_FIRST_CHAR; i <= TWO_ZONES_LAST_CHAR; i++)
  {   
    charToPrint = CHAR_BOTTOM;
    if(i == TWO_ZONES_FIRST_CHAR)
    { 
      // FIRST CHAR
      if((data->filledSite == LEFT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        if(data->takeOverStageSquares != 1)
        {
          charToPrint = CHAR_RIGHT_STRIP_FILLED;
        }
        else
        {
          charToPrint = CHAR_TWO_STRIPS_FILLED;
        }
      }
      else
      {
        if(data->takeOverStageSquares != 1)
        {
          charToPrint = CHAR_RIGHT_STRIP;
        }
        else
        {
          charToPrint = CHAR_TWO_STRIPS;
        }      
      }
    }
    else if(i == (data->takeOverStageSquares - 1))
    {
      if((data->filledSite == LEFT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_LEFT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_LEFT_STRIP;
      }
    }
    else if(i == TWO_ZONES_LAST_CHAR)
    {
      if((data->filledSite == LEFT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_LEFT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_LEFT_STRIP;
      }
    }
    else
    {
      if((data->filledSite == LEFT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_FILLED;
      }
    }

    lcd.setCursor((7 - i), 1);
    lcd.write(byte(charToPrint));
  }
  
  //RIGHT SITE
  for(int i = TWO_ZONES_FIRST_CHAR; i <= TWO_ZONES_LAST_CHAR; i++)
  {   
    charToPrint = CHAR_BOTTOM;
    if(i == TWO_ZONES_FIRST_CHAR)
    {
      if((data->filledSite == RIGHT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        if(data->takeOverStageSquares != 1)
        {
          charToPrint = CHAR_LEFT_STRIP_FILLED;
        }
        else
        {
          charToPrint = CHAR_TWO_STRIPS_FILLED;
        }
      }
      else
      {
        if(data->takeOverStageSquares != 1)
        {
          charToPrint = CHAR_LEFT_STRIP;
        }
        else
        {
          charToPrint = CHAR_TWO_STRIPS;
        }
      }
    }
    else if(i == (data->takeOverStageSquares - 1))
    {
      if((data->filledSite == RIGHT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_RIGHT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_RIGHT_STRIP;
      }
    }
    else if(i == TWO_ZONES_LAST_CHAR)
    {
      if((data->filledSite == RIGHT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_RIGHT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_RIGHT_STRIP;
      }
    }
    else
    {
      if((data->filledSite == RIGHT_TEAM) && (data->filledSquared >= (i + 1)))
      {
        charToPrint = CHAR_FILLED;
      }
    }

    lcd.setCursor((8 + i), 1);
    lcd.write(byte(charToPrint));
  }    
}

void printNoTeamsProgressBar(const progressBarDataS* const data)
{

  for(int i = 0; i < NO_TEAM_PROGRESS_BAR_CHARACTERS; i++)
  {
    char charToPrint = CHAR_BOTTOM;
    if(i == FIRST_LCD_CHAR)
    {
      if(data->filledSquared >= 1)
      {
        charToPrint = CHAR_LEFT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_LEFT_STRIP;
      }
    }
    else if(i == NO_TEAM_PROGRESS_BAR_CHARACTERS - 1)
    {
      if(data->filledSquared == NO_TEAM_PROGRESS_BAR_CHARACTERS)
      {
        charToPrint = CHAR_RIGHT_STRIP_FILLED;
      }
      else
      {
        charToPrint = CHAR_RIGHT_STRIP;
      }
    }
    else
    {
      if(data->filledSquared >= (i + 1))
      {
        charToPrint = CHAR_FILLED;
      }
    }

    lcd.setCursor((6 + i), 1);
    lcd.write(byte(charToPrint));
  }
}

void printProgressBar(const progressBarDataS* const data)
{
  if(data->ammountOfStages == BAR_TYPE_TWO_TEAMS)
  {
    printtwoZonesProgresBar(data);
  }
  else if(data->ammountOfStages == BAR_TYPE_NO_TEAMS)
  {
    printNoTeamsProgressBar(data);
  }

}