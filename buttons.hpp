#ifndef BUTTONS_H
#define BUTTONS_H

/* > Include  *****************************************************************/
#include "bar.hpp"
/* > Defines ******************************************************************/
#define COOLDOWN 20

#define RELAY 3
#define BUZZER 4
#define SWITCH 5
#define LEFT_TEAM_BUTTON 6
#define RIGHT_TEAM_BUTTON 7


#define RIGHT_BUTTON 8
#define LEFT_BUTTON 9
#define UP_BUTTON 10
#define DOWN_BUTTON 11

#define RED_LED 12
#define BLUE_LED 13

/* > Externs ******************************************************************/

/* > Functions ****************************************************************/
bool buttonPushed(unsigned short pin)
{   
    if (digitalRead(pin) == 0)
    {
        delay(COOLDOWN);
        if (digitalRead(pin) == 0)
        {
            while(true)
            {
                if(digitalRead(pin) == 1)
                {
                    return(true);
                }
            }
        }
    }
    return false;
}


#endif