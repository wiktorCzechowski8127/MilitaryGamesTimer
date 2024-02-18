#include "menu.hpp"
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void setTime(msTimeT* timeToModify)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("USTAW CZAS");
    while(true)
    {
        //upButton.loop();
        if(true)
        {   
            Serial.print("in");
            delay(2000);
            break;
        }
    }
}


/* > Function convertMsTo3var
*******************************************************************************/
/**
 * @brief Converting ms to hours, minutes and seconds.
 * 
 * @param[in] timeInMs     Time in Ms to covert.
 * @param[in] hours     Time in Ms to covert.
 * @param[in] minutes     Time in Ms to covert.
 * @param[in] seconds     Time in Ms to covert.
 * @return void
 * 
*******************************************************************************/
void convertMsTo3var(const msTimeT* const timeInMs,
                     humanTimeT* hours,
                     humanTimeT* minutes,
                     humanTimeT* seconds)
{
    *hours = (*timeInMs / HOURS_IN_MS);
    *minutes = (*timeInMs - (*hours * HOURS_IN_MS)) / MINUTES_IN_MS;
    *seconds = (*timeInMs - (*hours * HOURS_IN_MS) - (*minutes * MINUTES_IN_MS)) / SECONDS_IN_MS;
}

/* > Function printTime
*******************************************************************************/
/**
 * @brief Printing time as option.
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

/* > Function setBombGamemodeDomination
*******************************************************************************/
/**
 * @brief Setting default values to bomb gamemode options
 * 
 * @param[in] gamemodeBombS     Pointer to domination gamemode settings structure
 * @return void
 * 
*******************************************************************************/
void setDefaultGamemodeBomb(gamemodeBombS* gm)
{
    gm->gameTime = (0 * HOURS_IN_MS + 50 * MINUTES_IN_MS + 45 * SECONDS_IN_MS);
    gm->fullTakeOverTime = 20;
    gm->takeOverTime = 30;
}

/* > Function setDefaultGamemodeDomination
*******************************************************************************/
/**
 * @brief Setting default values to domination gamemode options
 * 
 * @param[in] gamemodeBombS     Pointer to domination gamemode settings structure
 * @return void
 * 
*******************************************************************************/
void setDefaultGamemodeDomination(gamemodeDominationS* gm)
{
    gm->gameTime = (0 * HOURS_IN_MS + 45 * MINUTES_IN_MS + 0 * SECONDS_IN_MS);
    gm->fullTakeOverTime = (0 * HOURS_IN_MS + 0 * MINUTES_IN_MS + 50 * SECONDS_IN_MS);
    gm->takeOverTime = (0 * HOURS_IN_MS + 0 * MINUTES_IN_MS + 20 * SECONDS_IN_MS);
}

/* > Function initializeMenu
*******************************************************************************/
/**
 * @brief Initializing and setting default values to menu and gamemode settings
 * 
 * @param[in] menuBaseS     Pointer to base menu structure.
 * @return void
 * 
*******************************************************************************/
void initializeMenu(menuBaseS* menuBase)
{   
    memset(&menuBase->navigation.menuPosition[0], 
           0,
           MENU_STAGES * sizeof(menuBase->navigation.menuPosition[0]));

    setDefaultGamemodeBomb(&menuBase->gamemodeData.gamemodeBomb);
    setDefaultGamemodeDomination(&menuBase->gamemodeData.gamemodeDomination);
    printMenu(menuBase);
}

/* > Function printBombOptions
*******************************************************************************/
/**
 * @brief Printing to LCD screen current bomb gamemode options.
 * 
 * @param[in] menuBaseS     Pointer to structure with menu and gamemodes data.
 * @return void
 * 
*******************************************************************************/
void printBombOptions(const menuBaseS* const menuBase)
{       
        unsigned short position = menuBase->navigation.menuPosition[1];
        lcd.print((String)(menuBase->navigation.menuPosition[1] + 1) + 
                    ". " + 
                    menuBase->menuStrings.stringBomb[position]);
        lcd.setCursor(0,1);
        switch (position)
        {
        case 0:
            printTime(&menuBase->gamemodeData.gamemodeBomb.gameTime, false);
            break;
        case 1:
            printTime(&menuBase->gamemodeData.gamemodeBomb.fullTakeOverTime, true);
            break;
        case 2:
            printTime(&menuBase->gamemodeData.gamemodeBomb.takeOverTime, true);
            break;
        default:
            break;
        }
}

/* > Function printDominationOptions
*******************************************************************************/
/**
 * @brief Printing to LCD screen current domination gamemode options.
 * 
 * @param[in] menuBaseS     Pointer to structure with menu and gamemodes data.
 * @return void
 * 
*******************************************************************************/
void printDominationOptions(const menuBaseS* const menuBase)
{       
        unsigned short position = menuBase->navigation.menuPosition[1];
        lcd.print((String)(menuBase->navigation.menuPosition[1] + 1) + 
                    ". " + 
                    menuBase->menuStrings.stringDomination[position]);

        lcd.setCursor(0,1);
        switch (position)
        {
        case 0:
            printTime(&menuBase->gamemodeData.gamemodeDomination.gameTime, false);
            break;
        case 1:
            printTime(&menuBase->gamemodeData.gamemodeDomination.fullTakeOverTime, true);
            break;
        case 2:
            printTime(&menuBase->gamemodeData.gamemodeDomination.takeOverTime, true);
            break;
        default:
            break;
        }
}

/* > Function printMenuStage1
*******************************************************************************/
/**
 * @brief Printing to LCD screen current menu stage 1 option.
 * 
 * @param[in] menuNavigationS     Pointer to structure with menu navigation data.
 * @param[in] menuStringsS        Pointer to structure menu strings.
 * @return void
 * 
*******************************************************************************/
void printMenuStage1(const menuBaseS* const menuBase)
{   
    unsigned short positionStage0 = menuBase->navigation.menuPosition[0];
    unsigned short positionStage1 = menuBase->navigation.menuPosition[
        menuBase->navigation.menuStage];
        
    if(positionStage0 == BOMB_GAMEMODE)
    {
        printBombOptions(menuBase);
    }
    else if(positionStage0 == DOMINATION_GAMEMODE)
    {
        printDominationOptions(menuBase);
    }
}

/* > Function printMenuStage0
*******************************************************************************/
/**
 * @brief Printing to LCD screen current menu stage 0 option.
 * 
 * @param[in] menuNavigationS     Pointer to structure with menu navigation data.
 * @param[in] menuStringsS        Pointer to structure menu strings.
 * @return void
 * 
*******************************************************************************/
void printMenuStage0(const menuBaseS* const menuBase)
{   
    unsigned short position = menuBase->navigation.menuPosition[
        menuBase->navigation.menuStage];

    lcd.print((String)(position + 1) + 
            ". " + 
            menuBase->menuStrings.stringStage0[position]);
}

/* > Function printMenu
*******************************************************************************/
/**
 * @brief Printing to LCD screen current menu option
 * 
 * @param[in] menuNavigationS     Pointer to structure with menu navigation data.
 * @param[in] menuStringsS        Pointer to structure menu strings.
 * @return void
 * 
*******************************************************************************/
void printMenu(const menuBaseS* const menuBase)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    if(menuBase->navigation.menuStage == 0)
    {
        printMenuStage0(menuBase);
    }
    if(menuBase->navigation.menuStage == 1)
    {
        printMenuStage1(menuBase);
    }
}

/* > Function validateStage0Position
*******************************************************************************/
/**
 * @brief Checks menu positions and wrap around if its needed.
 * 
 * @param[in] currentMenuPosition  Pointer to structure with menu 
 *                                 navigation data.
 * @return void
 * 
*******************************************************************************/
void validateStage0Position(unsigned short* currentMenuPosition)
{
    if (*currentMenuPosition == AMMOUNT_OF_GAMEMODES)
    {
        *currentMenuPosition = 0;
    }
    else if (*currentMenuPosition == 65535) //TODO change this
    {
        *currentMenuPosition = AMMOUNT_OF_GAMEMODES - 1;
    }
}

/* > Function validateStage1_1Position
*******************************************************************************/
/**
 * @brief Checks bomb gamemode menu positions and 
 *        wrap around if its needed.
 * 
 * @param[in] currentMenuPosition  Pointer to structure with menu 
 *                                 navigation data.
 * @return void
 * 
*******************************************************************************/
void validateStage1_1Position(menuNavigationS* navigation, gamemodeBombS* gm)
{
    Serial.print("STAGE: ");
    Serial.println(navigation->menuStage);
    if(navigation->menuStage == 2)
    {
        switch (navigation->menuPosition[1])
        {
        case 0:
            Serial.println("settingTime");
            setTime(&gm->fullTakeOverTime);
            break;
        
        default:
            break;
        }
    }
    else
    {
        if (navigation->menuPosition[1] == STAGE_1_1_OPTIONS)
        {
            navigation->menuPosition[1] = 0;
        }
        else if (navigation->menuPosition[1] == 65535) //TODO change this
        {
            navigation->menuPosition[1] = STAGE_1_1_OPTIONS - 1;
        }
    }
}

/* > Function validateStage1_2Position
*******************************************************************************/
/**
 * @brief Checks domination gamemode menu positions and
 *        wrap around if its needed.
 * 
 * @param[in] currentMenuPosition  Pointer to structure with menu 
 *                                 navigation data.
 * @return void
 * 
*******************************************************************************/
void validateStage1_2Position(unsigned short* currentMenuPosition, gamemodeDominationS* gm)
{


    if (*currentMenuPosition == STAGE_1_2_OPTIONS)
    {
        *currentMenuPosition = 0;
    }
    else if (*currentMenuPosition == 65535) //TODO change this
    {
        *currentMenuPosition = STAGE_1_2_OPTIONS - 1;
    }
}

/* > Function validateMenuPositionWrapper
*******************************************************************************/
/**
 * @brief Wrapper for menu position cheecker and wrap around position 
 *        if its needed.
 *        
 * @param[in] menuNavigationS  Pointer to structure with menu navigation data.
 * @return void
 * 
*******************************************************************************/
void validateMenuPositionWrapper(menuNavigationS* navigation,  gamemodeDataS* gamemodeData)
{   
    unsigned short stage = navigation->menuStage;
    if (stage == 0)
    { 
        validateStage0Position(&navigation->menuPosition[0]);
    }
    else if (stage == 1 || stage == 2)
    {
        if (navigation->menuPosition[0] == 0)
        {
            validateStage1_1Position(navigation, &gamemodeData->gamemodeBomb);
            navigation->menuStage = 1;
        }
        else if(navigation->menuPosition[0] == 1)
        {
            validateStage1_2Position(&navigation->menuPosition[1], &gamemodeData->gamemodeDomination);
        }
    }
    if (stage == MENU_STAGES)
    {
        navigation->menuStage = MENU_STAGES - 1;
    }
    else if (stage == 65535)
    {
        navigation->menuStage = 0;        
    }
}

/* > Function processMenu
*******************************************************************************/
/**
 * @brief Create menu variables and structures and run main menu loop.
 *        
 * @return void
 * 
*******************************************************************************/
void processMenu()
{

    menuBaseS menuBase;

    rightButton.setDebounceTime(50);
    leftButton.setDebounceTime(50);
    upButton.setDebounceTime(50);
    downButton.setDebounceTime(50);

    initializeMenu(&menuBase);
    bool isButtonPushed = false;

    while(true)
    {   
        isButtonPushed = false;
        rightButton.loop();
        leftButton.loop();
        upButton.loop();
        downButton.loop();

        if(rightButton.isPressed())
        {
            Serial.println("right");
            isButtonPushed = true;
            menuBase.navigation.menuPosition[menuBase.navigation.menuStage]++;
        }
        if(leftButton.isPressed())
        {
                        Serial.println("left");
            isButtonPushed = true;
            menuBase.navigation.menuPosition[menuBase.navigation.menuStage]--;
        }
        if(downButton.isPressed())
        {
            Serial.println("down");
            isButtonPushed = true;
            menuBase.navigation.menuStage++;
            //menuBase.navigation.menuPosition[menuBase.navigation.menuStage] = 0;
        }
        if(upButton.isPressed())
        {
                        Serial.println("up");
            isButtonPushed = true;
            menuBase.navigation.menuStage--;
        }

        if(isButtonPushed)
        {
            validateMenuPositionWrapper(&menuBase.navigation, &menuBase.gamemodeData);
            printMenu(&menuBase);
        }
    }
}