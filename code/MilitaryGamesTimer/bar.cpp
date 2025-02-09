#include "bar.hpp"
#include <arduino.h>

/*******************************************************************************
 * Function: createProgressBarCharacters
 * 
 * @brief Creating cnd writing to ldc memory custom characters used to print
 *        progress bar.
 * 
*******************************************************************************/
void progressBarBaseC::createProgressBarCharacters()
{
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
  _lcd->createChar(CHAR_BOTTOM, bottomStrip);
  _lcd->createChar(CHAR_FILLED, filled);
  _lcd->createChar(CHAR_RIGHT_STRIP, rightStrip);
  _lcd->createChar(CHAR_RIGHT_STRIP_FILLED, rightStripFilled);
  _lcd->createChar(CHAR_LEFT_STRIP, leftStrip);
  _lcd->createChar(CHAR_LEFT_STRIP_FILLED, leftStripFilled);
  _lcd->createChar(CHAR_TWO_STRIPS, twoStrips);
  _lcd->createChar(CHAR_TWO_STRIPS_FILLED, twoStripsFilled);
}

/*******************************************************************************
 * Function: printProgressBarOnLcd
 * 
 * @brief Function prints progres bar on lcd.
 * 
*******************************************************************************/
void progressBarBaseC::printProgressBarOnLcd()
{
  _lcd->setCursor(_xPos, _yPos);
  uint8_t modifier = 0;
  for (int i = 0; i < _size; i++)
  {
    modifier = (_layoutMask >> i) & 0x01;
    _lcd->write(_progressBarLayout[i] + modifier);
  }
}

/*******************************************************************************
 * Function: changeBarPosition
 * 
 * @brief Function changing ProgressBar position on lcd screen.
 *
 * @param [in]  xPos  
 * 
*******************************************************************************/
void progressBarBaseC::changeBarPosition(uint8_t xPos, uint8_t yPos)
{
  _yPos = xPos;
  _yPos = yPos;
}

/* > classicProgressBarC ***************************************************************/

/* > public ***************************************************************/
/*******************************************************************************
 * Function: calculateProgressAndPrintIfDifferent
 * 
 * @brief Function calculate progress and prints progress bar is its needed
 *        or when user force printing.
 *
 * @param [in] value          Value that shoud be translated to progress.
 * @param [in] forcePrint     Set as false by defualt. If it's true function
 *                            print progress bar even if progress is the same
 *                            as in previous calculation.
 * @return void
 * 
*******************************************************************************/
void classicProgressBarC::calculateProgressAndPrintIfDifferent(unsigned long value, uint8_t forcePrint = false)
{
  uint8_t newProgress = value / _scaleFactor;
  if ((newProgress != _progress) || forcePrint)
  {
    _progress = newProgress;
    _layoutMask = 0;
    for (uint8_t i = 0; i < _progress; i++)
    {
      _layoutMask = _layoutMask | 1 << i;
    }
    progressBarBaseC::printProgressBarOnLcd();
  }
}

/* > Private ***************************************************************/
/*******************************************************************************
 * Function: generateProgressBarLayout
 * 
 * @brief Generationg progress bar layout.
 *
 * @return void
 * 
*******************************************************************************/
void classicProgressBarC::generateProgressBarLayout()
{
  memset(_progressBarLayout, 0, sizeof(_progressBarLayout));
  _progressBarLayout[0] = CHAR_LEFT_STRIP;
  _progressBarLayout[_size-1] = CHAR_RIGHT_STRIP;
}

/*******************************************************************************
 * Function: printClassicProgressBarData
 * 
 * @brief function print classic progress bar setting on serial.
 *
 * @return void
 * 
*******************************************************************************/
void classicProgressBarC::printClassicProgressBarData()
{
  Serial.println();
  Serial.println("CLASSIC BAR CLASS");
  Serial.print("maxValue: ");
  Serial.println(_maxValue);
  Serial.print("progress");
  Serial.println(_progress);
  Serial.print("scaleFactor");
  Serial.println(_scaleFactor);
}

/* > twoDeviationProgressBarC ***************************************************************/

/* > private ***************************************************************/

/*******************************************************************************
 * Function: generateProgressBarLayout
 * 
 * @brief Generationg progress bar layout.
 *
 * @return void
 * 
*******************************************************************************/
void twoDeviationProgressBarC::generateProgressBarLayout()
{
  memset(_progressBarLayout, 0, sizeof(_progressBarLayout));

  _specialValueScaleFactor = 0;
  _specialValuePlace = 0;
  _progressBarLayout[0] = CHAR_LEFT_STRIP;
  _progressBarLayout[7] = CHAR_RIGHT_STRIP;
  _progressBarLayout[8] = CHAR_LEFT_STRIP;
  _progressBarLayout[15] = CHAR_RIGHT_STRIP;

  if(_maxValue != _specialValue)
  {
    _specialValuePlace = round(LCD_HALF * ((float)_specialValue / _maxValue));


    if (_specialValuePlace == 0 || _specialValuePlace == 1) 
    {
      _progressBarLayout[7] = CHAR_TWO_STRIPS;
      _progressBarLayout[8] = CHAR_TWO_STRIPS;
      _specialValuePlace = 1;
    }
    else
    {
      if (_specialValuePlace == LCD_HALF) 
      {
        _specialValuePlace--;
      } 
      _progressBarLayout[7 + _specialValuePlace] = CHAR_RIGHT_STRIP;
      _progressBarLayout[8 - _specialValuePlace] = CHAR_LEFT_STRIP;
    }
  }
  else //(_maxValue == _specialValue)
  {
    _specialValuePlace = 8;
  }
    _specialValueScaleFactor = _specialValue / _specialValuePlace;
    _maxValueScaleFactor = (_maxValue - _specialValue) / ((_size / 2) - _specialValuePlace);

}

/*******************************************************************************
 * Function: calculateProgressAndPrintIfDifferent
 * 
 * @brief Function calculate progress and prints progress bar is its needed
 *        or when user force printing.
 *
 * @param [in] value          Value that shoud be translated to progress.
 * @param [in] forcePrint     Set as false by defualt. If it's true function
 *                            print progress bar even if progress is the same
 *                            as in previous calculation.
 * @return void
 * 
*******************************************************************************/
void twoDeviationProgressBarC::calculateProgressAndPrintIfDifferent(long value, uint8_t forcePrint = false)
{

  bool filledSite = (value > 0) ? 1 : 0;
  if (_lastFilledSite != filledSite)
  {
    _lastFilledSite = filledSite;
    forcePrint = true;
  }
  value = abs(value);

  uint8_t newProgress = 0;

  if (value < _specialValue)
  {
    newProgress = value / _specialValueScaleFactor;
  }
  else
  {
    newProgress = ((value - _specialValue) / _maxValueScaleFactor) + _specialValuePlace;
  }

  if ((newProgress != _progress) || forcePrint)
  {
    _progress = newProgress;
    _layoutMask = 0;
    for (uint8_t i = 0; i < _progress; i++)
    {
      _layoutMask = _layoutMask | 1 << i;
    }

    if(filledSite == 0)
    {
      _layoutMask = _layoutMask << (LCD_HALF - _progress);
    }
    else
    {
      _layoutMask = _layoutMask << (LCD_HALF);
    }
    progressBarBaseC::printProgressBarOnLcd();
    /*
    uint8_t modifier = 0;
    for (int i = 0; i < _size; i++)
    { 
      if(i == 8)
      {
        Serial.print("|");
      }
      modifier = (_layoutMask >> i) & 0x01;
      Serial.print(_progressBarLayout[i] + modifier);
    }
    Serial.println("");
    */
  }

/*
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
  */
}