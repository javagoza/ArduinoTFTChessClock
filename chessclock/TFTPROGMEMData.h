/*!
   @file TFTPROGMEMData.h

   This is part of the TFT Virtual Segment Display for Arduino
   Virtual seven segments clock display for Arduino TFT LCD Displays
   Displays a clock with 4 or 6 seven segment modules


   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain

*/

#ifndef _TFTPROGMEMData_H_
#define _TFTPROGMEMData_H_

#include <Arduino.h>

template <typename T> void PROGMEMData (const T * sce, T& dest)
  {
  memcpy_P (&dest, sce, sizeof (T));
  }

template <typename T> T PROGMEMData (const T * sce)
  {
  static T temp;
  memcpy_P (&temp, sce, sizeof (T));
  return temp;
  }

#endif //  _TFTPROGMEMData_H_ 
