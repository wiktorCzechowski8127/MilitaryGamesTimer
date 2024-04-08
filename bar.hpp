#include <stdint.h>
#ifndef BAR_H
#define BAR_H

/* > Include  *****************************************************************/
#include <LiquidCrystal_I2C.h>
/* > Defines ******************************************************************/
#define CHAR_BOTTOM 0
#define CHAR_FILLED 1

#define CHAR_RIGHT_STRIP 2
#define CHAR_RIGHT_STRIP_FILLED 3

#define CHAR_LEFT_STRIP 4
#define CHAR_LEFT_STRIP_FILLED 5

#define CHAR_TWO_STRIPS 6
#define CHAR_TWO_STRIPS_FILLED 7

#define FORCE_PRINTING 1

#define LCD_SIZE 16
#define LCD_HALF 8
/* > Typedefs *****************************************************************/

/* > Externs ******************************************************************/

/* > Structures ***************************************************************/

/* > Classes ******************************************************************/
class progressBarBaseC
{
  public:

    /**
    * Constructor
    *
    * @param lcd      Pointer to LiquidCrystal_I2C class.
    * @param size     Ammount of progress bar sections. Allowe values: 2 - 16.
    * @param xPos	    Position of first progress bar section on LCD.
    * @param yPos   	Position in Y demension of progress bar section on LCD.
    */
    progressBarBaseC(LiquidCrystal_I2C *lcd, uint8_t size, uint8_t xPos, uint8_t yPos)
    {
      _lcd = lcd;
      _size = size;
      _xPos = xPos;
      _yPos = yPos;
      _progress = 0;
      _layoutMask = 0;
      createProgressBarCharacters();
    }

    void printBaseClass();
  //public:

  private:
    void createProgressBarCharacters();

    LiquidCrystal_I2C *_lcd;
  //private:

  protected:
    uint8_t _progressBarLayout[16];
    uint8_t _size : 5;
    uint8_t _xPos : 4;
    uint8_t _yPos : 1;
    uint8_t _progress : 5;
    uint8_t padding : 1;
    uint16_t _layoutMask;

    void printProgressBarOnLcd();

    void changeBarPosition(uint8_t xPos, uint8_t yPos);
  //protected:
};

/*******************************************************************************
 * Constructor: printClassicProgressBarData
 * Base class:  progressBarBaseC
 * 
 * @param lcd        Pointer to LiquidCrystal_I2C class.
 * @param maxValue   ProgressBar maximal value.
 * @param size       Ammount of progress bar sections. Allowe values: 2 - 16.
 * @param xPos	      Position of first progress bar section on LCD.
 * @param yPos   	  Position in Y demension of progress bar section on LCD.
 * 
*******************************************************************************/
class classicProgressBarC: private progressBarBaseC
{
  public:
    classicProgressBarC(LiquidCrystal_I2C *lcd,
                        unsigned long maxValue, 
                        uint8_t size, 
                        uint8_t xPos, 
                        uint8_t yPos)
     :progressBarBaseC(lcd, size, xPos, yPos)
    {
      _maxValue = maxValue;
      _scaleFactor = _maxValue / _size;
      generateProgressBarLayout();
      printBaseClass();
      printClassicProgressBarData();
    }

    //using progressBarBaseC::printProgressBarOnLcd; TODO TO DELETE
    /* > Variables ***************************************************************/

    /* > Functions ***************************************************************/
    /**
    * classicProgressBarC: printProgressBar
    * @brief Function prints progres bar on lcd.
    **/ 
    void printProgressBar(unsigned long value);

    /**
    * classicProgressBarC: calculateProgressAndPrintIfDifferent
    * @brief Function calculate progress and prints progress bar is its needed
    *        or when user force printing.
    **/
    void calculateProgressAndPrintIfDifferent(unsigned long value, uint8_t forcePrint = false);

  //public:

  private:
    /* > Variables ***************************************************************/
    unsigned long _maxValue;
    unsigned long _scaleFactor;

    /* > Functions ***************************************************************/
    /**
    * classicProgressBarC: generateProgressBarLayout
    * @brief Generationg progress bar layout.
    **/
    void generateProgressBarLayout();

    /**
    * classicProgressBarC: printClassicProgressBarData
    * @brief Function print classic progress bar setting on serial.
    **/
    void printClassicProgressBarData();
  //private 
};


/*******************************************************************************
 * Constructor: twoDeviationProgressBarC
 * Base class:  progressBarBaseC
 * 
 * @param lcd             Pointer to LiquidCrystal_I2C class.
 * @param maxValue        ProgressBar maximal value.
 * @param specialValue    Value that describes special event in progress bar.
 * @param yPos   	        Position in Y demension of progress bar section on LCD.
 * 
******************************************************************************/
class twoDeviationProgressBarC: private progressBarBaseC
{
  public:

    twoDeviationProgressBarC(LiquidCrystal_I2C *lcd, long maxValue, long specialValue, uint8_t yPos)
      :progressBarBaseC(lcd, 16, 0, yPos)
    {
      _maxValue = maxValue;
      _specialValue = specialValue;
      generateProgressBarLayout();
      progressBarBaseC::printBaseClass();
    }

    void calculateProgressAndPrintIfDifferent(long value, uint8_t forcePrint = false);

  private:
    /* > Variables **************************************************************/
    long _maxValue;
    long _specialValue;
    uint8_t _specialValuePlace;

    unsigned long _maxValueScaleFactor;
    unsigned long _specialValueScaleFactor;

    /* > Functiosn **************************************************************/

    void generateProgressBarLayout();

};

#endif
