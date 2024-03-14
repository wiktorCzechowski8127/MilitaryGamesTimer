#ifndef COMMON
#define COMMON

/* > Includes *****************************************************************/
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/
#define HOURS_IN_MS 3600000
#define MINUTES_IN_MS 60000
#define SECONDS_IN_MS (unsigned long)1000

#define BEEP_TIME 10
#define ALARM_SPEAKER_MAX_TIME 2 * MINUTES_IN_MS

/* > Typedefs *****************************************************************/
typedef long int msTimeWithSingT;
typedef unsigned long msTimeT;
typedef unsigned short humanTimeT;

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/* > Functions ****************************************************************/
void convertMsTo3var(const msTimeT* const timeInMs,
                     humanTimeT* hours,
                     humanTimeT* minutes,
                     humanTimeT* seconds);

void printTime(const msTimeT* const timeInMs, bool minutesOnly);

void beep();

#endif