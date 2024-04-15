#include <arduino.h>
#include "led.hpp"
/*******************************************************************************
 * Function: printBaseClass
 * 
 * @brief DEBUG FUNCTION - Printing on serial progressBarBaseC variables.
 * 
*******************************************************************************/
void ledC::turnOn()
{
  _nextBlink = 0xFFFFFFFF;
  _ledStatus = 1;
  digitalWrite(_pin, _ledStatus);
}

/*******************************************************************************
 * Function: printBaseClass
 * 
 * @brief DEBUG FUNCTION - Printing on serial progressBarBaseC variables.
 * 
*******************************************************************************/
void ledC::turnOff()
{
  _nextBlink = 0xFFFFFFFF;
  _ledStatus = 0;
  digitalWrite(_pin, _ledStatus);
}

/*******************************************************************************
 * Function: printBaseClass
 * 
 * @brief DEBUG FUNCTION - Printing on serial progressBarBaseC variables.
 * 
*******************************************************************************/
void ledC::blink(uint32_t currentTime)
{
  if(_nextBlink == 0xFFFFFFFF)
  {
    _nextBlink = currentTime + _blinkTime;
    _ledStatus = true;
    digitalWrite(_pin, _ledStatus);
  }
  else if(currentTime > _nextBlink)
  {
    _nextBlink = currentTime + _blinkTime;
    _ledStatus = !_ledStatus;
    digitalWrite(_pin, _ledStatus);
  }
}