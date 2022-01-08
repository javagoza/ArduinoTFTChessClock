/*!
   @file TFTSevenSegmentModule.h

   This is part of the TFT Virtual Segment Display for Arduino


   Written by Enrique Albertos, with
   contributions from the open source community.

   This code is in the public domain.

*/

#ifndef __TFTSevenSegmentModule_H__
#define __TFTSevenSegmentModule_H__


#include "arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library



class TFTSevenSegmentModule {
  public:
    /*!
       @brief Create a TFTSevenSegmentModule represent a seven segment module than can display one digit
          @param x         x coordinate
          @param y         y coordinate
          @param w         seven segment module width
          @param h         seven segment module height
          @param onColor   565 segment color when led segments are in on state
          @param offColor  565 segment color when led segment are in off state
          @param ledWidth  width in pixels of each segment led
          @param on        segment on state, true on state, false off state

    */
    TFTSevenSegmentModule(Adafruit_TFTLCD * , int16_t , int16_t , int16_t, int16_t, uint16_t , uint16_t , int16_t, boolean );

    /*!
       @brief    Display digit 0 to 9
        @param   digit to display
    */
    void display(int16_t digit);

    /*!
      @brief    Change next drawing position of the module
      @param x                  x coordinate
      @param y                  y coordinate
    */
    void setPosition( int16_t ,  int16_t );

    /*!
       @brief    Change next onColor of the led segments
          @param color   565 segment color when led segments are in on state
    */
    void setOnColor( uint16_t );

    /*!
      @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
    */
    void setOffColor( uint16_t );

    /*!
      @brief    Change next led width of the segments
      @param ledWidth   led width in pixels
    */
    void setLedWidth( int16_t  );

    /*!
      @brief    Change next module width
      @param h   width in pixels
    */

    void setWidth( int16_t  );

    /*!
      @brief    Change next module height
      @param h   heigh in pixels
    */
    void setHeight( int16_t );

    /*!
      @brief    Change Adafruit_TFTLCD instance to display on
         @param tft                pointer to Adafruit_TFTLCD
    */
    void setTft(Adafruit_TFTLCD * );


    /*!
      @brief    Change state to on
    */
    void on();

    /*!
      @brief    Change state to off
    */
    void off();

    /*!
      @brief    Get the current x coordinate of the module
      @returns the x coordinate
    */

    int16_t getX();

    /*!
      @brief    Get the current y coordinate of the module
      @returns the y coordinate
    */
    int16_t getY();

    /*!
      @brief    Get the current seven segment width in pixels
      @returns the current seven segments width in pixels
    */
    int16_t getWidth();


    /*!
      @brief    Get the current seven segments height in pixels
      @returns the current seven segments height in pixels
    */
    int16_t getHeight();

    /*!
      @brief    Get the current led segment with in pixels
      @returns the current seven segments height in pixels
    */
    int16_t getLedWidth();



  private:
    Adafruit_TFTLCD * m_tft;
    int16_t m_x;
    int16_t m_y;
    int16_t m_w;
    int16_t m_h;
    uint16_t m_onColor;
    uint16_t m_offColor;
    int16_t m_ledWidth;
    boolean m_on;

    void draw_F_LeftUpperLed(uint16_t ) ;
    void draw_E_LeftBottomLed(uint16_t ) ;
    void draw_B_RightUpperLed(uint16_t );
    void draw_C_RightBottomLed(uint16_t ) ;
    void draw_A_UpperLed(uint16_t );
    void draw_G_MiddleLed(uint16_t );
    void draw_D_BottomLed(uint16_t ) ;



};

#endif
