#include "menu.hpp"
#include <LiquidCrystal_I2C.h>
#include "buttons.hpp"
#include "common.hpp"


extern LiquidCrystal_I2C lcd;


void startGame(menuBaseS* menuBase)
{
    if(menuBase->navigation.menuPosition[0] == BOMB_GAMEMODE)
    {
        bool startButtonKeepPushed = false;
        unsigned long pushingTime = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("STARTUJE TRYB");
        lcd.setCursor(0,1);
        lcd.print("BOMBY");
        while (true)
        {
            startButtonKeepPushed = false;
            if(buttonPushed(UP_BUTTON))
            {
                Serial.println("up");
                break;
            }
            if(digitalRead(DOWN_BUTTON) == 0)
            {
                startButtonKeepPushed = true;
                if(pushingTime == 0)
                {
                    pushingTime = millis();
                }
            }

            if(startButtonKeepPushed == true)
            {
                if(millis() > (pushingTime + 3000))
                {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("WYSTARTOWANO");
                    while(1);
                }
                else
                {   
                    lcd.setCursor(13,1);
                    if(millis() > (pushingTime + 0)) lcd.print(".");
                    if(millis() > (pushingTime + 1000)) lcd.print(".");
                    if(millis() > (pushingTime + 2000)) lcd.print(".");
                }
            }
            else
            {
                lcd.setCursor(13,1);
                lcd.print("   ");
                pushingTime = 0;
            }
        }
    }
}

/* > Function validateTime
*******************************************************************************/
/**
 * @brief Proceding tresholds on time.
 * 
 * @param[in] time      Time param to procede.
 * @return void
 * 
*******************************************************************************/
void validateTime(humanTimeT* time)
{
    if(*time > 65515)
    {
        *time = 0;
    }
    else if(*time > 60)
    {
        *time = 60;
    }
}

/* > Function setTime
*******************************************************************************/
/**
 * @brief Setting time of gamemode option.
 * 
 * @param[in] timeInMs     Pointer to time in Ms to set.
 * @param[in] minutesOnly  Information about using only minutes and seconds.
 * @return void
 * 
*******************************************************************************/
void setTime(msTimeT* timeToModify, bool minutesOnly)
{
    // Initializing variables
    char valueToPrint[15] = "";
    humanTimeT hours;
    humanTimeT minutes;
    humanTimeT seconds;
    convertMsTo3var(timeToModify, &hours, &minutes, &seconds);

    unsigned short cursorPosition = 0;
    unsigned short monthOnlyPositionTreshold = 0;

    bool isButtonPushed = false;
    bool isUpOrDownButtonPused = false;
    short operand = 1;

    // Printing status on lcd
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("USTAW CZAS");
    if (minutesOnly)
    {
        cursorPosition = 3;
        monthOnlyPositionTreshold = 3;
        snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d", minutes, seconds);
    }
    else
    {
        cursorPosition = 0;
        monthOnlyPositionTreshold = 0;
        snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d:%02d", hours, minutes, seconds);       
    }
    lcd.setCursor(0,1);
    lcd.print(valueToPrint);
    lcd.setCursor(cursorPosition - monthOnlyPositionTreshold,1);
    lcd.blink();

    // Main function loop
    while(true)
    {
        isUpOrDownButtonPused = false;
        isButtonPushed = false;

        // Reading button status
        if(buttonPushed(UP_BUTTON))
        {
            Serial.println("up");
            isButtonPushed = true;
            isUpOrDownButtonPused = true;
            operand = 1;
        }
        if(buttonPushed(DOWN_BUTTON))
        {
            isButtonPushed = true;
            isUpOrDownButtonPused = true;
            operand = -1;
        }
        if(buttonPushed(RIGHT_BUTTON))
        {
            isButtonPushed = true;
            cursorPosition++;
            if(cursorPosition == 2 || cursorPosition == 5)
            {
                cursorPosition++;
            }
        }
        if(buttonPushed(LEFT_BUTTON))
        {
            isButtonPushed = true;
            cursorPosition--;
            if(cursorPosition == 2 || cursorPosition == 5)
            {
                cursorPosition--;               
            }
        }

        // Proceding up/down buttons
        if(isUpOrDownButtonPused)
        {
            switch (cursorPosition)
            {
            case HOURS_TENS:
                hours += (TEENS * operand);
                validateTime(&hours);
                break;
            case HOURS_UNITS:
                hours += (UNITS * operand);
                validateTime(&hours);
                break;
            case MINUTES_TENS:
                minutes += (TEENS * operand);
                validateTime(&minutes);
                break;
            case MINUTES_UNITS:
                minutes += (UNITS * operand);
                validateTime(&minutes);
                break;
            case SECONDS_TENS:
                seconds += (TEENS * operand);
                validateTime(&seconds);
                break;
            case SECONDS_UNTIS:
                seconds += (UNITS * operand);
                validateTime(&seconds);
                break;
            default:
                break;
            }
        }

        // Printing status on lcd
        if(isButtonPushed)
        {   
            if (cursorPosition >= TIME_CHARACTERS || cursorPosition < monthOnlyPositionTreshold)
            {
                convert3varToMs(hours, minutes, seconds, timeToModify);
                lcd.noBlink();
                break;
            }
            if (minutesOnly)
            {
                snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d", minutes, seconds);
            }
            else
            {
                snprintf(valueToPrint, sizeof valueToPrint, "%02d:%02d:%02d", hours, minutes, seconds);       
            }
            lcd.setCursor(0,1);
            lcd.print(valueToPrint);
            lcd.setCursor(cursorPosition - monthOnlyPositionTreshold,1);
            lcd.blink();
        }
    }
}

/* > Function setBoolean
*******************************************************************************/
/**
 * @brief Setting boolean option of gamemode .
 * 
 * @param[in] timeInMs     Pointer to time in Ms to set.
 * @param[in] minutesOnly  Information about using only minutes and seconds.
 * @return void
 * 
*******************************************************************************/
void setBoolean(bool* option)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("USTAW STATUS");
    printBoolOption(option);
    lcd.setCursor(BOOL_OPTION_CURSOR_POS,1);

    while(true)
    {
        // Reading button status
        if(buttonPushed(UP_BUTTON))
        {
            break;
        }
        if(buttonPushed(RIGHT_BUTTON))
        {
            *option = !*option;
            Serial.println("right  ");
            Serial.print(*option);  
            printBoolOption(option);
        }
        if(buttonPushed(LEFT_BUTTON))
        {
            Serial.print("left  ");
            *option = !*option;
            Serial.print(*option);  
            printBoolOption(option);
        }
    }
}


/* > Function convertMsTo3var
*******************************************************************************/
/**
 * @brief Converting ms to hours, minutes and seconds.
 * 
 * @param[in] timeInMs    Time in Ms to covert.
 * @param[in] hours       Hours in human format.
 * @param[in] minutes     Minutes in human format.
 * @param[in] seconds     Seconds in human format.
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


/* > Function printBoolOption
*******************************************************************************/
/**
 * @brief Printing boolean status of option on lcd.
 * 
 * @param[in] option        Status to modify
 * @return void
 * 
*******************************************************************************/
void printBoolOption(const bool* const option)
{
    lcd.setCursor(BOOL_OPTION_CURSOR_POS,1);
    if(*option == true)
    {
        lcd.print("TAK");
    }
    else
    {
        lcd.print("NIE");
    }
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
    gm->armingTime = (0 * HOURS_IN_MS + 50 * MINUTES_IN_MS + 40 * SECONDS_IN_MS);
    gm->defusingTime = (0 * HOURS_IN_MS + 50 * MINUTES_IN_MS + 35 * SECONDS_IN_MS);
    gm->enableSwitch = true;
    gm->slowReversing = false;
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
    gm->gameTime = (0 * HOURS_IN_MS + 0 * MINUTES_IN_MS + 10 * SECONDS_IN_MS);
    gm->fullTakeOverTime = (0 * HOURS_IN_MS + 0 * MINUTES_IN_MS + 50 * SECONDS_IN_MS);
    gm->takeOverTime = (0 * HOURS_IN_MS + 0 * MINUTES_IN_MS + 20 * SECONDS_IN_MS);
    gm->enableSwitch = false;
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
            printTime(&menuBase->gamemodeData.gamemodeBomb.armingTime, true);
            break;
        case 2:
            printTime(&menuBase->gamemodeData.gamemodeBomb.defusingTime, true);
            break;
        case 3:
            printBoolOption(&menuBase->gamemodeData.gamemodeBomb.enableSwitch);
            break;
        case 4:
            printBoolOption(&menuBase->gamemodeData.gamemodeBomb.slowReversing);
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
        case 3:
            printBoolOption(&menuBase->gamemodeData.gamemodeDomination.enableSwitch);
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
void validateStage1_1Position(menuBaseS* menuBase)
{
    if(menuBase->navigation.menuStage == 2)
    {
        if(menuBase->navigation.menuPosition[2] > STAGE_1_1_OPTIONS)
        {
            menuBase->navigation.menuPosition[2] = STAGE_1_1_OPTIONS;
        } //TODO
        switch (menuBase->navigation.menuPosition[1])
        {
        case 0:
            setTime(&menuBase->gamemodeData.gamemodeBomb.gameTime, false);
            break;
        case 1:
            setTime(&menuBase->gamemodeData.gamemodeBomb.armingTime, true);
            break;
        case 2:
            setTime(&menuBase->gamemodeData.gamemodeBomb.defusingTime, true);
            break;
        case 3:
            setBoolean(&menuBase->gamemodeData.gamemodeBomb.enableSwitch);
            break;
        case 4:
            setBoolean(&menuBase->gamemodeData.gamemodeBomb.slowReversing);
            break;
        case 5:
            startGame(menuBase);
            break;    
        default:
            break;
        }
    }
    else
    {
        if (menuBase->navigation.menuPosition[1] == STAGE_1_1_OPTIONS)
        {
            menuBase->navigation.menuPosition[1] = 0;
        }
        else if (menuBase->navigation.menuPosition[1] == 65535) //TODO change this
        {
            menuBase->navigation.menuPosition[1] = STAGE_1_1_OPTIONS - 1;
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
void validateStage1_2Position(menuBaseS* menuBase)
{
    if(menuBase->navigation.menuStage == 2)
    {
        switch (menuBase->navigation.menuPosition[1])
        {
        case 0:
            setTime(&menuBase->gamemodeData.gamemodeDomination.gameTime, false);
            break;
        case 1:
            setTime(&menuBase->gamemodeData.gamemodeDomination.fullTakeOverTime, true);
            break;
        case 2:
            setTime(&menuBase->gamemodeData.gamemodeDomination.takeOverTime, true);
            break;
        case 3:
            setBoolean(&menuBase->gamemodeData.gamemodeDomination.enableSwitch);
            break;
        case 4:
            startGame(menuBase);
        default:
            break;
        }
    }
    else
    {
        if (menuBase->navigation.menuPosition[1] == STAGE_1_2_OPTIONS)
        {
            menuBase->navigation.menuPosition[1] = 0;
        }
        else if (menuBase->navigation.menuPosition[1] == 65535) //TODO change this
        {
            menuBase->navigation.menuPosition[1] = STAGE_1_2_OPTIONS - 1;
        }
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
void validateMenuPositionWrapper(menuBaseS* menuBase)
{   
    unsigned short stage = menuBase->navigation.menuStage;
    if (stage == 0)
    { 
        validateStage0Position(&menuBase->navigation.menuPosition[0]);
        menuBase->navigation.menuPosition[1] = 0;
    }
    else if (stage == 1 || stage == 2)
    {
        if (menuBase->navigation.menuPosition[0] == 0)
        {
            validateStage1_1Position(menuBase);
            menuBase->navigation.menuStage = 1;
        }
        else if(menuBase->navigation.menuPosition[0] == 1)
        {
            validateStage1_2Position(menuBase);
            menuBase->navigation.menuStage = 1;
        }
    }
    if (stage == MENU_STAGES)
    {
        menuBase->navigation.menuStage = MENU_STAGES - 1;
    }
    else if (stage == 65535)
    {
        menuBase->navigation.menuStage = 0;        
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


    initializeMenu(&menuBase);
    bool isButtonPushed = false;

    while(true)
    {   
        isButtonPushed = false;

        if (buttonPushed(RIGHT_BUTTON))
        {
            Serial.println("right");
            isButtonPushed = true;
            menuBase.navigation.menuPosition[menuBase.navigation.menuStage]++;
        }
        if (buttonPushed(LEFT_BUTTON))
        {
            Serial.println("left");
            isButtonPushed = true;
            menuBase.navigation.menuPosition[menuBase.navigation.menuStage]--;
        }
        if (buttonPushed(DOWN_BUTTON))
        {
            Serial.println("down");
            isButtonPushed = true;
            menuBase.navigation.menuStage++;
            //menuBase.navigation.menuPosition[menuBase.navigation.menuStage] = 0;
        }
        if (buttonPushed(UP_BUTTON))
        {
            Serial.println("up");
            isButtonPushed = true;
            menuBase.navigation.menuStage--;
        }

        if(isButtonPushed)
        {
            validateMenuPositionWrapper(&menuBase);
            printMenu(&menuBase);
        }
    }
}