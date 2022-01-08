/*!
   @file TFTSevenSegmentClockDisplay.h

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Displays a clock with 4 or 6 seven segment modules


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#ifndef _TFTSevenSegmentClockDisplay_H_
#define _TFTSevenSegmentClockDisplay_H_

#include "arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "TFTSevenSegmentModule.h"
#include "TFTSevenSegmentDisplay.h"



/** Virtual seven segments clock display for Arduino TFT LCD Displays */
class TFTSevenSegmentClockDisplay : public TFTSevenSegmentDisplay {


  public:
    TFTSevenSegmentClockDisplay(  Adafruit_TFTLCD* ,  int16_t  ,  int16_t,  int16_t,  int16_t,  uint16_t,  uint16_t,  int16_t );
    /*!
      @brief Create a TFTSevenSegmentClockDisplay represent a clock display with 4 or 6 seven segment modules
         @param x                  x coordinate
         @param y                  y coordinate
         @param w                  seven segment module width
         @param h                  seven segment module height
         @param onColor            565 segment color when led segments are in on state
         @param offColor           565 segment color when led segment are in off state
         @param ledWidth           width in pixels of each segment led
         @param showHours          true show clock with hours-minutes-seconds, false show only minutes-seconds
         @param secondsHeightRatio reduction factor of the size of the digits representing the seconds

    */
    TFTSevenSegmentClockDisplay(Adafruit_TFTLCD * , int16_t , int16_t , int16_t , int16_t , uint16_t , uint16_t , int16_t , boolean, float );

    /*!
       @brief destructor deallocate memory reserved for seven segment modules
    */
    ~TFTSevenSegmentClockDisplay();

    /*!
       @brief    Display time from hour, minutes ans seconds
        @param   hours   number to display on the hours subgroup module
        @param   hours   number to display on the minutes subgroup module
        @param   hours   number to display on the seconds siubgroup module
        @param   units separator estate. true on, false off
    */
    void display(int16_t ,  int16_t , int16_t, boolean );

    /*!
      @brief    Display time from seconds
      @param   timeSeconds   Seconds to be represented as HH:MM:SS
      @param   units separator estate. true on, false off
    */
    void displaySeconds(long, boolean );

    /*!
       @brief    Display time from milliseconds
        @param   timeMillis   Seconds to be represented as HH:MM:SS
        @param   units separator estate. true on, false off
    */
    void displayMillis(long, boolean );

    /*!
      @brief    Change next drawing position of the display
      @param x                  x coordinate
      @param y                  y coordinate
    */
    void setPosition(int16_t , int16_t );

    /*!
       @brief    Change next onColor of the led segments
          @param color   565 segment color when led segments are in on state
    */
    void setOnColor(uint16_t ) override;

    /*!
      @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
    */
    void setOffColor(uint16_t ) override;

    /*!
      @brief    Change next led width of the segments
      @param ledWidth   led width in pixels
    */
    void setLedSegmentWidth(int16_t  ) override;

    /*!
       @brief    Change the next seven segment modules width
          @param w   seven segments width in pixels
    */
    void setSegmentModuleWidth(int16_t  ) override;

    /*!
       @brief    Change the next seven segment modules height
          @param h   seven segments height in pixels
    */
    void setSegmentModuleHeight(int16_t ) override;

    /*!
      @brief    Get the current seven segments width in pixels
      @returns the current seven segments width in pixels
    */
    int16_t getSegmentModuleWidth( ) override;

    /*!
      @brief    Get the current seven segments height in pixels
      @returns the current seven segments height in pixels
    */
    virtual int16_t getSegmentModuleHeight();

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


    virtual void paint();

  private:
    static const int16_t DIGITS = 6; // number of seven segment modules for the clock display
    boolean m_showHours{};
    float m_secondsHeightRatio{};
    TFTSevenSegmentModule* digits[DIGITS];
    enum Unit { HH1 = 0, HH2 , MM1, MM2, SS1, SS2};
    


};

#endif // _TFTSevenSegmentClockDisplay_H_
