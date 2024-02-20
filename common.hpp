#ifndef COMMON
#define COMMON

#include <LiquidCrystal_I2C.h>

#define HOURS_IN_MS 3600000
#define MINUTES_IN_MS 60000
#define SECONDS_IN_MS (unsigned long)1000

/* > Typedefs *****************************************************************/
typedef unsigned long msTimeT;
typedef unsigned short humanTimeT;

void convertMsTo3var(const msTimeT* const timeInMs,
                     humanTimeT* hours,
                     humanTimeT* minutes,
                     humanTimeT* seconds);

void printTime(const msTimeT* const timeInMs, bool minutesOnly);
#endif