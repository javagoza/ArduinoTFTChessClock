/*!
   @file TFTSevenSegmentDecimalDisplay.h

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Displays a virtual decimal seven segment display


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#ifndef _TFTSevenSegmentDecimalDisplay_H_
#define _TFTSevenSegmentDecimalDisplay_H_

#include "arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "TFTSevenSegmentModule.h"
#include "TFTSevenSegmentDisplay.h"


/** Virtual seven segments clock display for Arduino TFT LCD Displays */
class TFTSevenSegmentDecimalDisplay : public TFTSevenSegmentDisplay {


  public:

    /*!
      @brief Create a TFTSevenSegmentClockDisplay represent a clock display with 4 or 6 seven segment modules
         @param x                  x coordinate
         @param y                  y coordinate
         @param w                  seven segment module width
         @param h                  seven segment module height
         @param onColor            565 segment color when led segments are in on state
         @param offColor           565 segment color when led segment are in off state
         @param ledWidth           width in pixels of each segment led

    */
    TFTSevenSegmentDecimalDisplay(Adafruit_TFTLCD * , int16_t , int16_t , int16_t , int16_t , uint16_t , uint16_t , int16_t  );

    /*!
       @brief destructor deallocate memory reserved for seven segment modules
    */
    ~TFTSevenSegmentDecimalDisplay();

    /*!
       @brief    Display decimal number 000 to 999
       @param    number   number to display
    */
    void display(int16_t  );

    /*!
       @brief    Display time from hour, minutes ans seconds
        @param   hundreds number to display on the hours subgroup module
        @param   tens     number to display on the minutes subgroup module
        @param   ones     number to display on the seconds siubgroup module

    */
    void display(int16_t , int16_t , int16_t );


    /*!
      @brief    Get the current width in pixels
      @returns the current seven segments width in pixels
    */
    virtual int16_t getWidth();


    /*!
      @brief    Get the current height in pixels
      @returns the current seven segments height in pixels
    */
    virtual int16_t getHeight();


    /*!
      @brief    Change next onColor of the led segments
      @param color   565 segment color when led segments are in on state
    */
    virtual void setOnColor(uint16_t color) ;


    /*!
       @brief    Change next offColor of the led segments
          @param color   565 segment color when led segments are in off state
    */
    virtual void setOffColor(uint16_t color);


    virtual void paint();

  private:
    static const int16_t DIGITS = 3; // number of seven segment modules for the clock display
    TFTSevenSegmentModule* digits[DIGITS];
    enum Unit {HUNDREDS , TENS, ONES};


};

#endif // _TFTSevenSegmentDecimalDisplay_H_
