#ifndef COMMON
#define COMMON

/* > Includes *****************************************************************/
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/
#define HOURS_IN_MS 3600000
#define MINUTES_IN_MS 60000
#define SECONDS_IN_MS (unsigned long)1000

#define BEEP_TIME 10
#define ALARM_SPEAKER_MAX_TIME 60 * MINUTES_IN_MS

#define FIRST_LCD_CHAR 0
#define LAST_LCD_CHAR 15
#define LCD_MIDDLE_LEFT 7
#define LCD_MIDDLE_RIGHT 8

/* > Typedefs *****************************************************************/
typedef long int msTimeWithSingT;
typedef unsigned long msTimeT;
typedef unsigned short humanTimeT;

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/* > Functions ****************************************************************/
void waitUntilButtonReleased();

void convertMsTo3var(const msTimeT* const timeInMs,
                     humanTimeT* hours,
                     humanTimeT* minutes,
                     humanTimeT* seconds);

void printTime(const msTimeT* const timeInMs, bool minutesOnly);

void beep();

#endif