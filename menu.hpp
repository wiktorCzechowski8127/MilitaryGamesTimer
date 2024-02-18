#ifndef MENU_H
#define MENU_H

#include <ezButton.h>

/* > Defines ******************************************************************/
#define AMMOUNT_OF_GAMEMODES 2
#define MENU_STAGES 3

#define AMMOUNT_OF_LCD_CHARS 20

#define STAGE_1_1_OPTIONS 3
#define STAGE_1_2_OPTIONS 3

#define BOMB_GAMEMODE 0
#define DOMINATION_GAMEMODE 1

#define HOURS_IN_MS 3600000
#define MINUTES_IN_MS 60000
#define SECONDS_IN_MS (unsigned long)1000
/* > Structures ***************************************************************/

typedef unsigned long msTimeT;
typedef unsigned short humanTimeT;
/*******************************************************************************
 * struct: gamemodeBombS
 * 
 * @brief Bomb gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeBombS
{   
    // STAGE_1_1_OPTIONS 3
    msTimeT gameTime; //max time 1193 hours
    msTimeT fullTakeOverTime;
    msTimeT takeOverTime;
};

/*******************************************************************************
 * struct: gamemodeDominationS
 * 
 * @brief Domination gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeDominationS //
{
    msTimeT gameTime; //max time 1193 hours
    msTimeT fullTakeOverTime;
    msTimeT takeOverTime;
};

/*******************************************************************************
 * struct: gamemodeDataS
 * 
 * @brief Colective structure with gamemodes.
 * 
*******************************************************************************/
struct gamemodeDataS
{   
    gamemodeBombS gamemodeBomb;
    gamemodeDominationS gamemodeDomination;
};

/*******************************************************************************
 * struct: menuStringsS
 * 
 * @brief Structure with strings used to printing menu.
 * 
*******************************************************************************/
struct menuStringsS
{   
    char stringStage0[AMMOUNT_OF_GAMEMODES][AMMOUNT_OF_LCD_CHARS] = {"BOMBA", "DOMINACJA"};

    char stringBomb[STAGE_1_1_OPTIONS][AMMOUNT_OF_LCD_CHARS] = {"CZAS GRY",
                                                                "ROZBRAJANIE",
                                                                "UZBRAJANIE"};

    char stringDomination[STAGE_1_2_OPTIONS][AMMOUNT_OF_LCD_CHARS] = {"CZAS GRY",
                                                                      "P. PRZEJECIE",
                                                                      "PRZEJECIE"};
};

/*******************************************************************************
 * struct: menuNavigationS
 * 
 * @brief Structure with menu position data.
 * 
*******************************************************************************/
struct menuNavigationS
{
    unsigned short menuStage = 0;
    unsigned short menuPosition[MENU_STAGES];
};

/*******************************************************************************
 * struct: menuBaseS
 * 
 * @brief Base structure with all menu  and gamemodes data.
 * 
*******************************************************************************/
struct menuBaseS
{   
    menuNavigationS navigation;
    gamemodeDataS gamemodeData;
    menuStringsS menuStrings;

};

/* > Functions ****************************************************************/
ezButton rightButton(8);
ezButton leftButton(9);
ezButton upButton(10);
ezButton downButton(11);

void convertMsTo3var(const msTimeT* const timeInMs,
                     humanTimeT* hours,
                     humanTimeT* minutes,
                     humanTimeT seconds);

void printTime(const unsigned long* const timeInMs, bool minutesOnly);

void printBombOptions(const menuBaseS* const menuBase);
void printDominationOptions(const menuBaseS* const menuBase);
void printMenuStage1(const menuBaseS* const menuBase);
void printMenuStage0(const menuBaseS* const menuBase);
void printMenu(const menuBaseS* const menuBaseS);

void validateStage0Position(unsigned short* currentMenuPosition);
void validateStage1_1Position(unsigned short* currentMenuPosition, gamemodeBombS* gm);
void validateStage1_2Position(unsigned short* currentMenuPosition, gamemodeDominationS* gm);
void validateMenuPositionWrapper(menuNavigationS* navigation, gamemodeDataS* gamemodeData);

void setDefaultGamemodeBomb(gamemodeBombS* gm);
void setDefaultGamemodeDomination(gamemodeDominationS* gm);
void initializeMenu(menuBaseS* menuBase);

void processMenu();

#endif