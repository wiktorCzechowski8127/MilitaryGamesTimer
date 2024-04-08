#ifndef LED_H
#define LED_H

/* > Include  *****************************************************************/
#include <stdint.h>

/* > Defines ******************************************************************/

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/

/* > Structures ***************************************************************/

/* > Classes ******************************************************************/
class ledC
{
  public:

    /**
    * Constructor
    *
    * @param pin        Led pin.
    * @param blinkTime  Delay between led blinks.
    */
    ledC(uint8_t pin, uint16_t blinkTime = 0)
    {
      _pin = pin;
      _blinkTime = blinkTime;
      turnOff();
    }

    ~ledC() 
    {
      turnOff();
    }

    void turnOn();
    void turnOff();
    void blink(uint32_t currentTime);
  //public:

  private:
    uint8_t _pin;
    uint8_t _ledStatus;
    uint16_t _blinkTime;
    uint32_t _nextBlink;   
  //private:


};

#endif
