#include "common.hpp"


void waitUntilButtonReleased()
{
  while(true)
  {
    if (digitalRead(LEFT_BUTTON) && digitalRead(RIGHT_BUTTON))
    {
      break;
    }
  }
}

/* > Function convertMsTo3var
*******************************************************************************/
/**
 * @brief Converting ms to hours, minutes and seconds.
 * 
 * @param[in] hours     Hours in human format.
 * @param[in] minutes   Minutes in human format.
 * @param[in] seconds   Seconds in human format.
 * @param[in] timeInMs  Time in Ms to covert.
 * @return void
 * 
*******************************************************************************/
void convert3varToMs(humanTimeT hours,
                     humanTimeT minutes,
                     humanTimeT seconds,
                     msTimeT* timeInMs)
{
    *timeInMs = (hours * HOURS_IN_MS + minutes * MINUTES_IN_MS + seconds * SECONDS_IN_MS);
}

/* > Function printTime
*******************************************************************************/
/**
 * @brief Printing time of option on lcd.
 * 
 * @param[in] timeInMs      Time in Ms to print.
 * @param[in] minutesOlny   Printing time without hours.
 * @return void
 * 
*******************************************************************************/
void printTime(const msTimeT* const timeInMs, bool minutesOnly)
{
    humanTimeT hours;
    humanTimeT minutes;
    humanTimeT seconds;
    convertMsTo3var(timeInMs, &hours, &minutes, &seconds);
    char valueToPrint[15];
    if (minutesOnly)
    {
        snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d", minutes, seconds);
    }
    else
    {
        snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d:%02d", hours, minutes, seconds);       
    }
    lcd.print(valueToPrint);
}

/* > Function beep
*******************************************************************************/
/**
 * @brief Short buzzer sound.
 * 
 * @return void
 * 
*******************************************************************************/
void beep()
{
  digitalWrite(BUZZER, true);
  delay(BEEP_TIME);
  digitalWrite(BUZZER, false);
}