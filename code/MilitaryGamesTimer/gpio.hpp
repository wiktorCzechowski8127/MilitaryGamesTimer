#ifndef GPIO_H
#define GPIO_H


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
extern bool RELAY_OFF;
extern bool RELAY_ON;
/* > Functions ****************************************************************/
void initbuttons()
{
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, false);

  pinMode(LEFT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LEFT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_TEAM_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT_PULLUP);

  digitalWrite(A0, LOW);
  if(digitalRead(A1) == HIGH)
  {
    RELAY_ON = 1;
    RELAY_OFF = 0;
  }
  else
  {
    RELAY_ON = 0;
    RELAY_OFF = 1;    
  }
  
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, RELAY_OFF);

}

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