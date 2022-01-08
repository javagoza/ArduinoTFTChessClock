/*!
   @file TFTSevenSegmentDisplay.cpp

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Abstract class for displays


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/
#include "TFTSevenSegmentClockDisplay.h"


/*!
   @brief    Change next drawing position of the display
      @param x                  x coordinate
      @param y                  y coordinate
*/
void TFTSevenSegmentDisplay::setPosition(int16_t x, int16_t y) {
  m_x = x;
  m_y = y;
}

/*!
   @brief    Change next onColor of the led segments
      @param color   565 segment color when led segments are in on state
*/
void TFTSevenSegmentDisplay::setOnColor(uint16_t color) {
  m_onColor = color;
}


/*!
   @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
*/
void TFTSevenSegmentDisplay::setOffColor(uint16_t color) {
  m_offColor = color;
}

/*!
   @brief    Change next led width of the segments
      @param ledWidth   led width in pixels
*/
void TFTSevenSegmentDisplay::setLedSegmentWidth(int16_t ledWidth) {
  m_ledWidth = ledWidth;
}

/*!
   @brief    Change the next seven segment modules width
      @param w   seven segments width in pixels
*/
void TFTSevenSegmentDisplay::setSegmentModuleWidth(int16_t w) {
  m_w = w;
}


/*!
   @brief    Change the next seven segment modules height
      @param h   seven segments height in pixels
*/
void TFTSevenSegmentDisplay::setSegmentModuleHeight(int16_t h) {
  m_h = h;
}

/*!
   @brief    Get the current seven segment base width in pixels
      @returns the current seven segments width in pixels
*/
int16_t TFTSevenSegmentDisplay::getSegmentModuleWidth() {
  return m_w;
}

/*!
   @brief    Get the current seven segments base height in pixels
      @returns the current seven segments height in pixels
*/
int16_t TFTSevenSegmentDisplay::getSegmentModuleHeight() {
  return m_h;
}
