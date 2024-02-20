#ifndef CHARACTERS_H
#define CHARACTERS_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>

/* > Defines ******************************************************************/
// Characters
#define FILLED 0
#define RIGHT_STRIP 1
#define RIGHT_STRIP_FILLED 2
#define LEFT_STRIP 3
#define LEFT_STRIP_FILLED 4
#define BOTTOM 5

// LCD sizes
#define FIRST_LCD_CHAR 0
#define LAST_LCD_CHAR 15

#define FIRST_LEFT_PLACE 7
#define FIRST_RIGHT_PLACE 8

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd; // TODO - verify is that correct

/* > Structures ***************************************************************/

/* > Functions ****************************************************************/
void createProgressBarCharacters();

#endif