#include <stdint.h>
#ifndef MENU_H
#define MENU_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "domination.hpp"
#include "bomb.hpp"
#include "gpio.hpp"
#include <stdint.h>

/* > Defines ******************************************************************/
#define AMMOUNT_OF_GAMEMODES 2
#define MENU_STAGES 3

#define AMMOUNT_OF_LCD_CHARS 14

#define STAGE_1_1_OPTIONS 11
#define STAGE_1_2_OPTIONS 9

#define BOMB_GAMEMODE 0
#define DOMINATION_GAMEMODE 1


#define TIME_CHARACTERS 8
#define TIME_CHARACTERS_MINUTES_ONLY 6

#define HOURS_TENS 0
#define HOURS_UNITS 1
#define MINUTES_TENS 3
#define MINUTES_UNITS 4
#define SECONDS_TENS 6
#define SECONDS_UNTIS 7

#define TEENS 10
#define UNITS 1

#define BOOL_OPTION_CURSOR_POS 4
#define UINT_OPTION_CURSOR_POS 2


/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/

/* > Structures ***************************************************************/
extern LiquidCrystal_I2C lcd;

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
    /*
    char stringBomb[STAGE_1_1_OPTIONS][AMMOUNT_OF_LCD_CHARS] = {"CZAS GRY",
                                                                "CZAS WYBUCHU",
                                                                "UZBRAJANIE",
                                                                "ROZBRAJANIE",
                                                                "PRZELACZNIK",
                                                                "ROZ. KONCZY",
                                                                "COFANIE PROG.",
                                                                "SYRENA",
                                                                "START"};

    char stringDomination[STAGE_1_1_OPTIONS][AMMOUNT_OF_LCD_CHARS] = {"CZAS GRY",
                                                                      "P. PRZEJECIE",
                                                                      "PRZEJECIE",
                                                                      "PUNKT CO",
                                                                      "LIMIT PUNKOW",
                                                                      "PRZELACZNIK",
                                                                      "SYRENA",
                                                                      "START"};
                                                                      */
};

/*******************************************************************************
 * struct: menuNavigationS
 * 
 * @brief Structure with menu position data.
 * 
*******************************************************************************/
struct menuNavigationS
{
    unsigned short menuStage = 0; //TODO delete this
    unsigned short menuPosition[MENU_STAGES];
    bool freezeMenu;
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
void startGame(menuBaseS* menuBase);
void validateTime(humanTimeT* time);
void setTime(msTimeT* timeToModify, bool minutesOnly, msTimeT timeLimit = UINT32_MAX);
void setBoolean(bool* option);
void setValue(int* valueToModify, int valueLimit);

void convert3varToMs(humanTimeT hours,
                     humanTimeT minutes,
                     humanTimeT seconds,
                     msTimeT* timeInMs);



void printBoolOption(const bool* const option);
void printValueOption(const int* const value, bool spaceFill);

void printBombOptions(const menuBaseS* const menuBase);
void printDominationOptions(const menuBaseS* const menuBase);
void printMenuStage1(const menuBaseS* const menuBase);
void printMenuStage0(const menuBaseS* const menuBase);
void printMenu(const menuBaseS* const menuBaseS);

void validateStage0Position(unsigned short* currentMenuPosition);
void validateStage1_1Position(menuBaseS* menuBase);
void validateStage1_2Position(menuBaseS* menuBase);
void validateMenuPositionWrapper(menuNavigationS* navigation, gamemodeDataS* gamemodeData);

void setDefaultGamemodeBomb(gamemodeBombS* gm);
void setDefaultGamemodeDomination(gamemodeDominationS* gm);
void initializeMenu(menuBaseS* menuBase);

void processMenu();

#endif