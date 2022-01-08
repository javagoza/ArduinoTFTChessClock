/*!
   @file TFTSevenSegmentDisplay.h

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Abstract class for displays


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#ifndef _TFTSevenSegmentDisplay_H_
#define _TFTSevenSegmentDisplay_H_

#include "arduino.h"
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "TFTSevenSegmentModule.h"

/** Virtual seven segments clock display for Arduino TFT LCD Displays */
class TFTSevenSegmentDisplay {

  protected:
    Adafruit_TFTLCD * m_tft;
    int16_t m_x{};
    int16_t m_y{};
    int16_t m_w{};
    int16_t m_h{};
    uint16_t m_onColor{};
    uint16_t m_offColor{};
    int16_t m_ledWidth{};

    TFTSevenSegmentDisplay() {}
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
    TFTSevenSegmentDisplay(Adafruit_TFTLCD* tft,
                           int16_t x = 0,
                           int16_t y = 0,
                           int16_t w = 16,
                           int16_t h = 32,
                           uint16_t onColor = 255,
                           uint16_t offColor = 0,
                           int16_t ledWidth = 3):
      m_tft{tft},
      m_x{x},
      m_y{y},
      m_w{w},
      m_h{h},
      m_onColor{onColor},
      m_offColor{offColor},
      m_ledWidth{ledWidth} {};;

    /*!
       @brief destructor deallocate memory reserved for seven segment modules
    */
    virtual ~TFTSevenSegmentDisplay() {};

  public:


    /*!
      @brief   paints content to tft. Refresh data on screen
    */
    virtual void paint();


    /*!
      @brief    Get the current width in pixels
      @returns the current seven segments width in pixels
    */
    virtual int16_t getWidth();

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
    virtual void setOnColor(uint16_t );

    /*!
      @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
    */
    virtual void setOffColor(uint16_t );

    /*!
      @brief    Change next led width of the segments
      @param ledWidth   led width in pixels
    */
    virtual void setLedSegmentWidth(int16_t  );

    /*!
       @brief    Change the next seven segment modules width
          @param w   seven segments width in pixels
    */
    virtual void setSegmentModuleWidth(int16_t  );

    /*!
       @brief    Change the next seven segment modules height
          @param h   seven segments height in pixels
    */
    virtual void setSegmentModuleHeight(int16_t );

    /*!
      @brief    Get the current seven segments width in pixels
      @returns the current seven segments width in pixels
    */
    virtual int16_t getSegmentModuleWidth( ) ;

    /*!
      @brief    Get the current seven segments height in pixels
      @returns the current seven segments height in pixels
    */
    virtual int16_t getSegmentModuleHeight();



    /*!
      @brief    Get the current height in pixels
      @returns the current seven segments height in pixels
    */
    virtual int16_t getHeight();



};

#endif // _TFTSevenSegmentDisplay_H_
