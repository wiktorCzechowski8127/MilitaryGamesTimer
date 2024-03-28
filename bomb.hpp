#include <stdint.h>
#ifndef BOMB_H
#define BOMB_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
#include "common.hpp"
#include "gamemodeCommon.hpp"
#include "lcd.hpp"
#include "buttons.hpp"

/* > Defines ******************************************************************/
#define BOMB_UNARMED 0
#define BOMB_ARMED 1

#define CLEAR_BOMB_STATUS false
/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/
extern LiquidCrystal_I2C lcd;

/* > Structures ***************************************************************/

/*******************************************************************************
 * struct: bombHistoryS
 * 
 * @brief Structure with history of games results .
 * 
*******************************************************************************/
struct bombHistoryS
{
  msTimeT unarmedTotalTime;
  msTimeT armedTotalTime;
  uint8_t bombStatus;
};

/*******************************************************************************
 * struct: gamemodeBombS
 * 
 * @brief Bomb gamemode options. Ammount of options: 3
 * 
*******************************************************************************/
struct gamemodeBombS
{   
    // STAGE_1_1_OPTIONS 3
    msTimeT gameTime;
    msTimeT armingTime;
    msTimeT defusingTime;
    msTimeT alarmSpeaker;
    msTimeT explosionTime;
    bool enableSwitch;
    bool slowReversing;
    bool isDefuseEndGame;
    bool explosionTimeReset;
    bombHistoryS history[MAX_HISTORY_RECORDS];
};

struct bombDataS
{
  msTimeWithSingT pointsInMs;
  msTimeT lastPushedButtonTimeStamp;
  msTimeT timeToExplosion;
  msTimeT endGameDefaultTime;
  msTimeT armedTotalTime;
  msTimeT unarmedTotalTime;
  uint8_t swithStatus: 1;
  uint8_t isButtonsPushed: 1;
  uint8_t currentBombStatus: 1;
  uint8_t noBombStatus: 1;
  uint8_t padding: 4;
};

/* > Functions ****************************************************************/

void bombInitializeProgressBarData(progressBarDataS* data, const gamemodeBombS* const gm);
void calculateFilledSquaresBomb(const gamemodeBombS* const gm, progressBarDataS* progressBarData, const bombDataS* const bombData);
void calculateTotalTimes(gamemodeTiming* timing, bombDataS* bombData);

void printSummary(const msTimeT* const unarmedTotalTime,
                  const msTimeT* const armedTotalTime,
                  uint8_t bombStatus);

void printBombStatus(bombDataS* const data);
void processBomb(const gamemodeBombS* const gm);

void saveResult(bombHistoryS* const history, 
                const msTimeT* const unarmedTotalTime,
                const msTimeT* const armedTotalTime,
                const uint8_t bombStatus);

void printBombGamemodeSettingsOnSerial(const gamemodeBombS* const gm); //DEBUG

void printBombHisotry(const bombHistoryS* const history);
#endif
