#ifndef DOMINATION_H
#define DOMINATION_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "gamemodeCommon.hpp"
#include "lcd.hpp"
#include "buttons.hpp"

/* > Defines ******************************************************************/

/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/*******************************************************************************
 * struct: gamemodeDominationS
 * 
 * @brief Domination gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeDominationS
{
    msTimeT gameTime;
    msTimeT fullTakeOverTime;
    msTimeT takeOverTime;
    bool enableSwitch;
};

/* > Functions ****************************************************************/

void dominationMain();
void setScren();


void printGamemodeSettingsOnSerial(const gamemodeDominationS* const gm);
void processDomination(const gamemodeDominationS* const gm);


#endif