/*!


   @file TFTSevenSegmentClockDisplay.h

  This is part of the TFT Virtual Segment Display for Arduino
   Represents a virtual seven segment display to display digits


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#include "TFTSevenSegmentModule.h"

// The codes below indicate which segments must be illuminated to display
// each number.
const unsigned char digitCodeMap[] = {
  //     GFEDCBA  Segments      7-segment map:
  B00111111,  // 0   "0"          AAA
  B00000110,  // 1   "1"         F   B
  B01011011,  // 2   "2"         F   B
  B01001111,  // 3   "3"          GGG
  B01100110,  // 4   "4"         E   C
  B01101101,  // 5   "5"         E   C
  B01111101,  // 6   "6"          DDD
  B00000111,  // 7   "7"
  B01111111,  // 8   "8"
  B01101111   // 9   "9"
};

/*!
  @brief Create a TFTSevenSegmentModule represent a seven segment module than can display one digit
   @param tft       pointer to Adafruit_TFTLCD
   @param x         x coordinate
   @param y         y coordinate
   @param w         seven segment module width
   @param h         seven segment module height
   @param onColor   565 segment color when led segments are in on state
   @param offColor  565 segment color when led segment are in off state
   @param ledWidth  width in pixels of each segment led
   @param on        segment on state, true on state, false off state

*/
TFTSevenSegmentModule::TFTSevenSegmentModule(Adafruit_TFTLCD* tft, int16_t x = 0, int16_t y = 0, int16_t w = 16, int16_t h = 32, uint16_t onColor = 0, uint16_t offColor = 0, int16_t ledWidth = 3, boolean on = true)
  : m_tft{ tft }, m_x{ x }, m_y{ y }, m_w{ w }, m_h{ h }, m_onColor{ onColor }, m_offColor{ offColor }, m_ledWidth{ ledWidth }, m_on{ on } {};

/*!
  @brief    Display digit 0 to 9
  @param   digit to display
*/
void TFTSevenSegmentModule::display(const int16_t digit) {
  const byte leds = digitCodeMap[digit];
  draw_A_UpperLed((leds & 1) && m_on ? m_onColor : m_offColor);
  draw_B_RightUpperLed((leds & 2) && m_on ? m_onColor : m_offColor);
  draw_C_RightBottomLed((leds & 4) && m_on ? m_onColor : m_offColor);
  draw_D_BottomLed((leds & 8) && m_on ? m_onColor : m_offColor);
  draw_E_LeftBottomLed((leds & 16) && m_on ? m_onColor : m_offColor);
  draw_F_LeftUpperLed((leds & 32) && m_on ? m_onColor : m_offColor);
  draw_G_MiddleLed((leds & 64) && m_on ? m_onColor : m_offColor);
}

/*!
  @brief    Draw F led Left Upper LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_F_LeftUpperLed(uint16_t color) {
  m_tft->startWrite();
  for (int i = 0; i < m_ledWidth; i++) {
    m_tft->writeFastVLine(m_x + i, m_y + i, m_h / 2 - 2 * i, color);
  }
  m_tft->endWrite();
}

/*!
  @brief    Draw E led Left Bottom LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_E_LeftBottomLed(uint16_t color) {
  m_tft->startWrite();
  for (int i = 0; i < m_ledWidth; i++) {
    m_tft->writeFastVLine(m_x + i, m_h / 2 + m_y + i + 1, m_h / 2 - 2 * i, color);
  }
  m_tft->endWrite();
}


/*!
  @brief    Draw B led Right Upper LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_B_RightUpperLed(uint16_t color) {
  m_tft->startWrite();
  for (int i = 0; i < m_ledWidth; i++) {
    m_tft->writeFastVLine(m_x + m_w - i, m_y + i, m_h / 2 - 2 * i, color);
  }
  m_tft->endWrite();
}



/*!
  @brief    Draw C led Right Bottom LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_C_RightBottomLed(uint16_t color) {
  m_tft->startWrite();
  for (int i = 0; i < m_ledWidth; i++) {
    m_tft->writeFastVLine(m_x + m_w - i, m_y + m_h / 2 + i + 1, m_h / 2 - 2 * i, color);
  }
  m_tft->endWrite();
}


/*!
  @brief    Draw G led Middle LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_G_MiddleLed(uint16_t color) {
  m_tft->startWrite();
  if (m_ledWidth < 2) {
    m_tft->writeFastHLine(m_x+1, m_y + m_h / 2, m_w, color);
  } else {
    int ledWidth = m_ledWidth < 2 ? 1 : m_ledWidth / 2;
    for (int i = 0; i < ledWidth + m_ledWidth % 2; i++) {
      m_tft->writeFastHLine(m_x + i + 2, m_y + m_h / 2 - i, m_w - 2 * i - 4, color);
      if (m_ledWidth > 1) {
        m_tft->writeFastHLine(m_x + i + 2, m_y + m_h / 2 + i + 1, m_w - 2 * i - 4, color);
      }
    }
  }
  m_tft->endWrite();
}

/*!
  @brief    Draw A led Upper LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_A_UpperLed(uint16_t color) {
  m_tft->startWrite();
  if (m_ledWidth < 2) {
    m_tft->writeFastHLine(m_x , m_y , m_w , color);
  } else {
    for (int i = 0; i < m_ledWidth; i++) {
      m_tft->writeFastHLine(m_x + i + 3, m_y + i, m_w - 2 * i - 5, color);
    }
  }
  m_tft->endWrite();

}


/*!
  @brief    Draw D led Bottom LED segment
  @param   565 segement color
*/
void TFTSevenSegmentModule::draw_D_BottomLed(uint16_t color) {
  m_tft->startWrite();
  if (m_ledWidth < 2) {
    m_tft->writeFastHLine(m_x, m_y + m_h , m_w, color);
  } else {
    for (int i = 0; i < m_ledWidth; i++) {
      m_tft->writeFastHLine(m_x + i + 3, m_y + m_h - i, m_w - 2 * i - 5, color);
    }
  }
  m_tft->endWrite();
}

/*!
  @brief    Change next drawing position of the module
  @param x                  x coordinate
  @param y                  y coordinate
*/
void TFTSevenSegmentModule::setPosition(int16_t x, int16_t y) {
  m_x = x;
  m_y = y;
}

/*!
  @brief    Change next onColor of the led segments
   @param color   565 segment color when led segments are in on state
*/
void TFTSevenSegmentModule::setOnColor(uint16_t color) {
  m_onColor = color;
}

/*!
  @brief    Change next offColor of the led segments
  @param color   565 segment color when led segments are in off state
*/
void TFTSevenSegmentModule::setOffColor(uint16_t color) {
  m_offColor = color;
}

/*!
  @brief    Change next led width of the segments
  @param ledWidth   led width in pixels
*/
void TFTSevenSegmentModule::setLedWidth(int16_t ledWidth) {
  m_ledWidth = ledWidth;
}


/*!
  @brief    Change next module width
  @param h   width in pixels
*/
void TFTSevenSegmentModule::setWidth(const int16_t w) {
  m_w = w;
}

/*!
  @brief    Change next module height
  @param h   heigh in pixels
*/
void TFTSevenSegmentModule::setHeight(const int16_t h) {
  m_h = h;
}


/*!
  @brief    Change Adafruit_TFTLCD instance to display on
  @param tft                pointer to Adafruit_TFTLCD
*/
void TFTSevenSegmentModule::setTft(Adafruit_TFTLCD* tft) {
  m_tft = tft;
}

/*!
  @brief    Change state to on
*/
void TFTSevenSegmentModule::on() {
  m_on = true;
}

/*!
  @brief    Change state to off
*/
void TFTSevenSegmentModule::off() {
  m_on = false;
}

/*!
  @brief    Get the current x coordinate of the module
  @returns the x coordinate
*/
int16_t TFTSevenSegmentModule::getX() {
  return m_x;
}

/*!
  @brief    Get the current y coordinate of the module
  @returns the y coordinate
*/
int16_t TFTSevenSegmentModule::getY() {
  return m_y;
}


/*!
  @brief    Get the current seven segment width in pixels
  @returns the current seven segments width in pixels
*/
int16_t TFTSevenSegmentModule::getWidth() {
  return m_w;
}

/*!
  @brief    Get the current seven segments height in pixels
  @returns the current seven segments height in pixels
*/
int16_t TFTSevenSegmentModule::getHeight() {
  return m_h;
}

/*!
  @brief    Get the current led segment with in pixels
  @returns the current seven segments height in pixels
*/
int16_t TFTSevenSegmentModule::getLedWidth() {
  return m_ledWidth;
}
