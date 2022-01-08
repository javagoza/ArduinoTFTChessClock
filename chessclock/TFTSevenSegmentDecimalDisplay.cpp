/*!
   @file TFTSevenSegmentDecimalDisplay.cpp

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Displays a virtual decimal seven segment display


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#include "TFTSevenSegmentDecimalDisplay.h"


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
TFTSevenSegmentDecimalDisplay::TFTSevenSegmentDecimalDisplay(Adafruit_TFTLCD* tft,
    int16_t x = 0,
    int16_t y = 0,
    int16_t w = 16,
    int16_t h = 32,
    uint16_t onColor = 255,
    uint16_t offColor = 0,
    int16_t ledWidth = 3 ) :
  TFTSevenSegmentDisplay (tft, x, y, w, h, onColor, offColor, ledWidth)
{
  int groupOffset = 0;
  int digit = 0;
  int offsetx = 0;
  int segmentWidth = m_ledWidth;
  for (int i = 0 ; i < DIGITS; i++) {
    offsetx = offsetx + (w  + w / 8 ) + 3;
    digits[i] = new TFTSevenSegmentModule(tft,
                                          x + offsetx ,
                                          y,
                                          w,
                                          h,
                                          m_onColor,
                                          m_offColor,
                                          segmentWidth,
                                          true);

    ++digit;
  }
}

/*!
   @brief destructor deallocate memory reserved for seven segment modules
*/
TFTSevenSegmentDecimalDisplay::~TFTSevenSegmentDecimalDisplay () {
  for (int i = 0; i < DIGITS; i++) {
    delete digits[i];
  }
}
/*!
   @brief    Display decimal number 000 to 999
   @param    number   number to display
*/
void TFTSevenSegmentDecimalDisplay::display(int16_t number ) {
  display((number / 100 ) % 10, (number / 10 ) % 10, number % 10);
}

/*!
   @brief    Display time from hour, minutes ans seconds
    @param   hundreds number to display on the hours subgroup module
    @param   tens     number to display on the minutes subgroup module
    @param   ones     number to display on the seconds siubgroup module

*/
void TFTSevenSegmentDecimalDisplay::display(int16_t hundreds, int16_t tens, int16_t ones) {

  if (hundreds > 0) {
    digits[HUNDREDS]->on();
    digits[HUNDREDS]->display(hundreds % 10);
  } else {
    digits[HUNDREDS]->off();
    digits[HUNDREDS]->display(0);
  }

  if ( tens > 0 || hundreds > 0 ) {
    digits[TENS]->on();
    digits[TENS]->display(tens % 10);
  } else {
    digits[TENS]->off();
    digits[TENS]->display(0);
  }

  if ( tens > 0 || hundreds > 0 || ones > 0 ) {
    digits[ONES]->on();
    digits[ONES]->display(ones % 10);
  } else {
    digits[ONES]->off();
    digits[ONES]->display(0);
  }

}


/*!
  @brief    Get the current width in pixels
  @returns the current seven segments width in pixels
*/
int16_t TFTSevenSegmentDecimalDisplay::getWidth() {
  return digits[ONES]->getX() + digits[ONES]->getWidth() - digits[HUNDREDS]->getX();
}


/*!
  @brief    Get the current height in pixels
  @returns the current seven segments height in pixels
*/
int16_t TFTSevenSegmentDecimalDisplay::getHeight() {
  return m_h;
}

/*!
   @brief    Change next onColor of the led segments
      @param color   565 segment color when led segments are in on state
*/
void TFTSevenSegmentDecimalDisplay::setOnColor(uint16_t color) {
  for (int i = 0; i < DIGITS; i++) {
    digits[i]->setOnColor(color);
  }
}


/*!
   @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
*/
void TFTSevenSegmentDecimalDisplay::setOffColor(uint16_t color) {
  for (int i = 0; i < DIGITS; i++) {
    digits[i]->setOffColor(color);
  }
}


void TFTSevenSegmentDecimalDisplay::paint() {}
