#ifndef CHARACTERS_H
#define CHARACTERS_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"

/* > Defines ******************************************************************/
// Characters
#define CHAR_FILLED 0
#define CHAR_RIGHT_STRIP 1
#define CHAR_RIGHT_STRIP_FILLED 2
#define CHAR_LEFT_STRIP 3
#define CHAR_LEFT_STRIP_FILLED 4
#define CHAR_BOTTOM 5
#define CHAR_TWO_STRIPS 6
#define CHAR_TWO_STRIPS_FILLED 7

// LCD sizes
#define TWO_ZONES_FIRST_CHAR 0
#define TWO_ZONES_LAST_CHAR 7

#define FIRST_LCD_CHAR 0
#define LAST_LCD_CHAR 15

#define FIRST_LEFT_PLACE 7
#define FIRST_RIGHT_PLACE 8

#define AMMOUNT_OF_SQUARES_PER_TEAM 8

#define BAR_TYPE_NO_TEAMS 0
#define BAR_TYPE_TWO_TEAMS 1

#define LEFT_TEAM 0
#define RIGHT_TEAM 1

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd; // TODO - verify is that correct

/* > Structures ***************************************************************/
struct progressBarDataS
{
  unsigned int ammountOfStages: 2;
  unsigned int secondStageSquares: 4;
  unsigned int filledSquared: 4;
  unsigned int filledSite: 1;
  unsigned int padding: 5;
  msTimeT timeOfOneTakeOverSquareFilled;
  msTimeT timeOfOneFullTakeOverSquareFilled;
};

/* > Functions ****************************************************************/
void createProgressBarCharacters();
void printtwoZonesProgresBar(const progressBarDataS* const data);
void printProgressBar(const progressBarDataS* const data);
#endif