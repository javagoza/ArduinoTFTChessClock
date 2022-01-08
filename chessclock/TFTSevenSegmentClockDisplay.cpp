/*!
   @file TFTSevenSegmentClockDisplay.cpp

   This is part of the TFT Virtual Segment Display for Arduino
   Displays a clock with 4 or 6 seven segment modules


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain
*/

#include "TFTSevenSegmentClockDisplay.h"


#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24
#define MILLIS_IN_SECOND 1000


/*!
   @brief Create a TFTSevenSegmentClockDisplay represent a clock display with 4 or 6 seven segment modules
   @param tft                pointer to Adafruit_TFTLCD
   @param x                  x coordinate
   @param y                  y coordinate
   @param w                  seven segment module width
   @param h                  seven segment module height
   @param onColor            565 segment color when led segments are in on state
   @param offColor           565 segment color when led segment are in off state
   @param ledWidth           width in pixels of each segment led

*/
TFTSevenSegmentClockDisplay::TFTSevenSegmentClockDisplay(
  Adafruit_TFTLCD* tft,
  int16_t x = 0,
  int16_t y = 0,
  int16_t w = 16,
  int16_t h = 32,
  uint16_t onColor = 255,
  uint16_t offColor = 0,
  int16_t ledWidth = 3) {
  TFTSevenSegmentClockDisplay(tft, x, y, w, h, onColor, offColor, ledWidth, true, 1.0);
}


/*!
   @brief Create a TFTSevenSegmentClockDisplay represent a clock display with 4 or 6 seven segment modules
   @param tft                pointer to Adafruit_TFTLCD
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
TFTSevenSegmentClockDisplay::TFTSevenSegmentClockDisplay(
  Adafruit_TFTLCD* tft,
  int16_t x = 0,
  int16_t y = 0,
  int16_t w = 16,
  int16_t h = 32,
  uint16_t onColor = 255,
  uint16_t offColor = 0,
  int16_t ledWidth = 3,
  boolean showHours = true,
  float secondsHeightRatio = 3 / 4)
  :
  TFTSevenSegmentDisplay (tft, x, y, w, h, onColor, offColor, ledWidth),
  m_showHours{showHours},
  m_secondsHeightRatio{secondsHeightRatio}
{
  int groupOffset = 0;
  int digit = 0;
  int offsetx = 0;
  int segmentWidth = m_ledWidth;
  for (int i = m_showHours ? 0 : 2; i < DIGITS; i++) {
    if (i < SS2 ) {
      offsetx = digit * (w + w / 8 + 3) + groupOffset;
    } else {
      offsetx = offsetx + (w  + w / 8 ) * m_secondsHeightRatio + 3;
    }

    if (i > MM2 && m_ledWidth > 2) {
      segmentWidth = m_ledWidth * m_secondsHeightRatio + 1;
    }

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

    if (i % 2 != 0) {
      groupOffset += w / 2;
    }
  }
  digits[SS1]->setHeight((float)h * m_secondsHeightRatio);
  digits[SS2]->setHeight((float)h * m_secondsHeightRatio);
  digits[SS1]->setWidth((float)w * m_secondsHeightRatio);
  digits[SS2]->setWidth((float)w * m_secondsHeightRatio);
}

/*!
   @brief    Display time from seconds
    @param   timeSeconds   Seconds to be represented as HH:MM:SS
    @param   units separator estate. true on, false off
*/
void TFTSevenSegmentClockDisplay::displaySeconds(long timeSeconds, boolean isSeparatorOn) {
  display((timeSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR)) % HOURS_IN_DAY,
          (timeSeconds / SECONDS_IN_MINUTE) % MINUTES_IN_HOUR,
          timeSeconds % SECONDS_IN_MINUTE, isSeparatorOn);
}


/*!
   @brief    Display time from milliseconds
    @param   timeMillis   Seconds to be represented as HH:MM:SS
    @param   units separator estate. true on, false off
*/
void TFTSevenSegmentClockDisplay::displayMillis(long timeMillis, boolean isSeparatorOn) {
  displaySeconds(timeMillis / MILLIS_IN_SECOND, isSeparatorOn);
}

/*!
   @brief    Display time from hour, minutes ans seconds
    @param   hours   number to display on the hours subgroup module
    @param   hours   number to display on the minutes subgroup module
    @param   hours   number to display on the seconds siubgroup module
    @param   units separator estate. true on, false off
*/
void TFTSevenSegmentClockDisplay::display(int16_t hours, int16_t minutes, int16_t seconds, boolean isSeparatorOn) {

  if (minutes > 9 || m_showHours) {
    digits[MM1]->on();
    digits[MM1]->display((minutes / 10) % 10);
  } else {
    digits[MM1]->off();
    digits[MM1]->display(0);
  }

  digits[MM2]->display(minutes % 10);

  digits[SS1]->display((seconds / 10) % 10);

  digits[SS2]->display(seconds % 10);

  if (m_showHours) {
    if (hours > 9) {
      digits[HH1]->on();
      digits[HH1]->display((hours / 10) % 10);
    } else {
      digits[HH1]->off();
      digits[HH1]->display(0);
    }


    digits[HH2]->display(hours % 10);

    m_tft->fillRect(digits[HH2]->getX() + digits[HH2]->getWidth() + (digits[MM1]->getX() - (digits[HH2]->getX() + digits[HH2]->getWidth())) / 2 -  digits[MM1]->getLedWidth() / 4,
                    digits[HH2]->getY() + digits[MM1]->getHeight() / 4,
                    m_ledWidth / 2 + 1 ,
                    m_ledWidth / 2 + 1 ,
                    isSeparatorOn == true ? m_onColor : m_offColor);

    m_tft->fillRect(digits[HH2]->getX()
                    + digits[HH2]->getWidth() + (digits[MM1]->getX() - (digits[HH2]->getX() + digits[HH2]->getWidth())) / 2 -  digits[MM1]->getLedWidth() / 4,
                    digits[HH2]->getY() + 3 * digits[MM1]->getHeight() / 4 -  digits[MM1]->getLedWidth() / 2,
                    m_ledWidth / 2 + 1 ,
                    m_ledWidth / 2 + 1 ,
                    isSeparatorOn == true ? m_onColor : m_offColor);

    m_tft->fillRect(digits[MM2]->getX() + digits[MM2]->getWidth() + (digits[SS1]->getX() - (digits[MM2]->getX() + digits[MM2]->getWidth())) / 2 -  digits[MM1]->getLedWidth() / 4,
                    digits[MM2]->getY() + digits[SS1]->getHeight() / 4,
                    m_ledWidth / 2 + 1 ,
                    m_ledWidth / 2 + 1 ,
                    isSeparatorOn == true ? m_onColor : m_offColor);

    m_tft->fillRect(digits[MM2]->getX()
                    + digits[MM2]->getWidth() + (digits[SS1]->getX() - (digits[MM2]->getX() + digits[MM2]->getWidth())) / 2 -  digits[MM1]->getLedWidth() / 4,
                    digits[MM2]->getY() + 3 * digits[SS1]->getHeight() / 4 -  digits[MM1]->getLedWidth() / 2,
                    m_ledWidth / 2 + 1 ,
                    m_ledWidth / 2 + 1 ,
                    isSeparatorOn == true ? m_onColor : m_offColor);
  }
}


/*!
   @brief    Change next onColor of the led segments
      @param color   565 segment color when led segments are in on state
*/
void TFTSevenSegmentClockDisplay::setOnColor(uint16_t color) {
  for (int i = m_showHours ? 0 : 2; i < DIGITS; i++) {
    digits[i]->setOnColor(color);
  }
  m_onColor = color;
}


/*!
   @brief    Change next offColor of the led segments
      @param color   565 segment color when led segments are in off state
*/
void TFTSevenSegmentClockDisplay::setOffColor(uint16_t color) {
  for (int i = m_showHours ? 0 : 2; i < DIGITS; i++) {
    digits[i]->setOffColor(color);
  }
  m_offColor = color;
}

/*!
   @brief    Change next led width of the segments
      @param ledWidth   led width in pixels
*/
void TFTSevenSegmentClockDisplay::setLedSegmentWidth(int16_t ledWidth) {
  for (int i = m_showHours ? 0 : 2; i < DIGITS; i++) {
    digits[i]->setLedWidth(ledWidth);
  }
  m_ledWidth = ledWidth;
}

/*!
   @brief    Change the next seven segment modules width
      @param w   seven segments width in pixels
*/
void TFTSevenSegmentClockDisplay::setSegmentModuleWidth(int16_t w) {
  m_w = w;
}


/*!
   @brief    Change the next seven segment modules height
      @param h   seven segments height in pixels
*/
void TFTSevenSegmentClockDisplay::setSegmentModuleHeight(int16_t h) {
  m_h = h;
}

/*!
   @brief    Get the current seven segment base width in pixels
      @returns the current seven segments width in pixels
*/
int16_t TFTSevenSegmentClockDisplay::getSegmentModuleWidth() {
  return digits[MM1]->getWidth();
}

/*!
   @brief    Get the current seven segments base height in pixels
      @returns the current seven segments height in pixels
*/
int16_t TFTSevenSegmentClockDisplay::getSegmentModuleHeight() {
  return digits[MM1]->getHeight();
}


/*!
  @brief    Get the current width in pixels
  @returns the width in pixels
*/
int16_t TFTSevenSegmentClockDisplay::getWidth() {
  if (m_showHours) {
    return digits[SS2]->getX() + digits[SS2]->getWidth() - digits[HH1]->getX();
  } else {
    return digits[SS2]->getX() + digits[SS2]->getWidth() - digits[MM1]->getX();
  }
}

/*!
  @brief    Get the current height in pixels
  @returns the height in pixels
*/
int16_t TFTSevenSegmentClockDisplay::getHeight() {
  if (digits[SS1]->getHeight() > digits[MM1]->getHeight() ) {
    return digits[SS1]->getHeight() ;
  } else {
    return digits[MM1]->getHeight() ;
  }
}

void TFTSevenSegmentClockDisplay::paint() {

}

/*!
   @brief destructor deallocate memory reserved for seven segment modules
*/
TFTSevenSegmentClockDisplay::~TFTSevenSegmentClockDisplay() {
  for (int i = m_showHours ? 0 : 2; i < DIGITS; i++) {
    delete digits[i];
  }
}
