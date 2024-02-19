#ifndef BUTTONS_H
#define BUTTONS_H

#define COOLDOWN 20

#define RIGHT_BUTTON 8
#define LEFT_BUTTON 9
#define UP_BUTTON 10
#define DOWN_BUTTON 11

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