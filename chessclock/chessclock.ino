
/*!
   @file TFTSevenSegmentClockDisplay.cpp

   This is part of the TFT Virtual Segment Display for Arduino
   Displays a clock with 4 or 6 seven segment modules

   IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
   CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
   SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
   When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
   For the Arduino Uno, Duemilanove, Diecimila, etc.:
     D0 connects to digital pin 8  (Notice these are
     D1 connects to digital pin 9   NOT in order!)
     D2 connects to digital pin 2
     D3 connects to digital pin 3
     D4 connects to digital pin 4
     D5 connects to digital pin 5
     D6 connects to digital pin 6
     D7 connects to digital pin 7
   For the Arduino Mega, use digital pins 22 through 29
   (on the 2-row header at the end of the board).

   Written by Enrique Albertos, with
   contributions from the open source community.

   Public Domain
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "TFTSevenSegmentClockDisplay.h"
#include "TFTSevenSegmentDecimalDisplay.h"
#include "TFTPROGMEMData.h"

#define PLAYER_CLOCK_HEIGHT 130
#define MENU_COMMANDS_HEIGHT 48
#define INITIAL_ROTATION 0
#define countof(a) (sizeof(a) / sizeof(a[0]))
// Touch screen presure threshold
#define MINPRESSURE 50
#define MAXPRESSURE 1000

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// 'pause1', 32x32px
const unsigned char epd_bitmap_pause [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x3f, 0x00, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80,
  0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80,
  0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80,
  0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80,
  0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80,
  0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'reset', 32x32px
const unsigned char epd_bitmap_reset [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x40, 0x1f, 0xf8, 0x00, 0x60, 0x7f, 0xfe, 0x00, 0x71, 0xff, 0xff, 0x80,
  0x7b, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xf0, 0x1f, 0xf8,
  0x7f, 0xe0, 0x07, 0xf8, 0x7f, 0xc0, 0x03, 0xfc, 0x7f, 0xe0, 0x01, 0xfc, 0x7f, 0xf0, 0x00, 0xfe,
  0x7f, 0xf8, 0x00, 0xfe, 0x7f, 0xfc, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7e,
  0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0xfe,
  0x7f, 0x00, 0x00, 0xfe, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x03, 0xfc, 0x1f, 0xe0, 0x07, 0xf8,
  0x1f, 0xf0, 0x1f, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xc0,
  0x01, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'settings', 32x32px
const unsigned char epd_bitmap_settings [] PROGMEM = {
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x0f, 0xf0, 0xe0,
  0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8,
  0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xfc, 0x3f, 0xf0,
  0x1f, 0xf0, 0x0f, 0xf8, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff,
  0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0x1f, 0xf0, 0x0f, 0xf8,
  0x0f, 0xfc, 0x3f, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0,
  0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0,
  0x07, 0x0f, 0xf0, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00
};

// 'pawn', 16x16px
const unsigned char epd_bitmap_pawn [] PROGMEM = {
  0x03, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0,
  0x0f, 0xf0, 0x0f, 0xf0, 0x1f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc
};


enum Buttons {SETTINGS_BUTTON = 0, PAUSE_BUTTON, RESET_BUTON, BOTTOM_BUTTON, UPPER_BUTTON};

enum States {IDLE = 0, SETTINGS, WHITE_PLAYING, BLACK_PLAYING, WHITE_IN_PAUSE, BLACK_IN_PAUSE, END_GAME};
States state = IDLE;

enum IncrementType { DELAY = 0, // Delay the playeer's clock starts after the delay period
                     BRONSTEIN, // Players receive the used portion of the increment at the end of each turn
                     FISCHER    // Players receive the full increment at the end of each turn, with increment 0 is BLIZT or GUILLOTINE
                   };



IncrementType incrementType = FISCHER;
uint16_t incrementSeconds = 0;
uint16_t stagesNumber = 1;
int selectedGame = 11;
struct StageType {
  long duration;
  int moves;
};

struct GameType {
  IncrementType incrementType;
  uint16_t incrementSeconds;
  uint16_t stagesNumber;
  StageType stages[3];
};

static const GameType games[24]  PROGMEM = {
  { FISCHER, 0, 1, {{ 180, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{ 300, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{ 600, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{1500, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{3600, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{7200, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{ 180, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{ 300, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 1, {{ 600, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 2, 1, {{1500, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 2, 1, {{3600, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 2, 1, {{7200, 0}, {0, 0}, {0, 0}}},
  { BRONSTEIN, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { BRONSTEIN, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { BRONSTEIN, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { BRONSTEIN, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { DELAY, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { DELAY, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { DELAY, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { DELAY, 2, 1, {{180, 0}, {0, 0}, {0, 0}}},
  { FISCHER, 0, 2, {{7200, 40}, {1800, 0}, {0, 0}}},
  { FISCHER, 0, 2, {{7200, 40}, {3600, 0}, {0, 0}}},
  { FISCHER, 0, 3, {{7200, 40}, {3600, 20}, {1800, 0}}},
  { FISCHER, 0, 3, {{7200, 40}, {3600, 20}, {3600, 0}}}
};

int selectedGameIndex = 0 ;
GameType currentGame;

const int settingsRows = 6;
const int settingsCols = 4;

StageType stages[3] = {{180, 0}, {0, 0}, {0, 0}};

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

uint16_t blacksmoves = 0; // movement counter for blacks
uint16_t whitesmoves = 0; // movement counter for whitess

unsigned long blacksTimeMillis = 0; // Whites time left
unsigned long whitesTimeMillis = 0; // Blacks time left
unsigned long blacksOldTimeMillis = 0; // used to decide if clock needs refresh
unsigned long whitesOldTimeMillis = 0; // used to decide if clock needs refresh

unsigned long whitesTurnInitMillis = millis();
unsigned long blacksTurnInitMillis = millis();
unsigned long whitesEllapsedTimeMillis = millis();
unsigned long blacksEllapsedTimeMillis = millis();

bool isNewTurn; // new player turn change

bool isWhiteDown = false; // White's clock is the one at the bottom of the screen
unsigned long lastTimeTouch = 0; // time since last touch of the screen for de-bouncing

uint16_t blacksRotation =  2; // initial orientation of the black pieces until field is chosen
uint16_t whitesRotation =  0; // initial orientation of the black pieces until field is chosen

// Touch screen callibration
const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x9341
const int TS_LEFT = 918, TS_RT = 106, TS_TOP = 76, TS_BOT = 898;
const TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


uint16_t backgroundColor = BLUE;
uint16_t foregroundColor = WHITE;
uint16_t pauseColor = tft.color565(30, 30, 30);
uint16_t alertColor = RED;

// Clock displays
TFTSevenSegmentClockDisplay clockDisplayMinutes(&tft, 30, 220, 35, 70, WHITE, backgroundColor, 8, false, .75); // Short games
TFTSevenSegmentClockDisplay clockDisplayHours(&tft, 10, 220, 26, 60, WHITE, backgroundColor, 6, true, .75); // Long games

// Moves counter display
TFTSevenSegmentDecimalDisplay movesDisplay(&tft, 180, 290, 5, 8, foregroundColor, backgroundColor, 1);

// Current display
TFTSevenSegmentClockDisplay clockDisplay = clockDisplayMinutes ;




void setup(void) {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  resetGame();
}

void loop(void) {
  readUiSelection();
  if (state == WHITE_PLAYING) {
    whiteClockLoop();
  } else if (state == BLACK_PLAYING) {
    blackClockLoop();
  }
}



void whiteClockLoop() {
  if (isNewTurn && (currentGame.incrementType == DELAY)
      && ((millis() - whitesTurnInitMillis) < currentGame.incrementSeconds * 1000 )) {
    if ((millis() - whitesTurnInitMillis) / 1000 % 10 == 0) {
      printTime(whitesTimeMillis, whitesRotation, whitesmoves, true);
      printTime(blacksTimeMillis, blacksRotation, blacksmoves, false);
    }
    // wait delay
    return;
  } else if (isNewTurn && (currentGame.incrementType == DELAY)) {
    whitesTurnInitMillis = millis();
    whitesEllapsedTimeMillis = millis();
  }

  whitesTimeMillis = whitesTimeMillis - (millis() - whitesEllapsedTimeMillis );
  unsigned long whitesTime = whitesTimeMillis / 1000;
  whitesEllapsedTimeMillis = millis();
  if (whitesTime > 0 ) {
    if (whitesOldTimeMillis / 1000 != whitesTime || isNewTurn) {
      if (isNewTurn) {
        printTime(blacksTimeMillis, blacksRotation, blacksmoves, false);
      }
      printTime(whitesTimeMillis, whitesRotation, whitesmoves, true);
      whitesOldTimeMillis = whitesTimeMillis;
      isNewTurn = false;
    }
  } else {
    state = END_GAME;
    printTime(0, whitesRotation, whitesmoves, true);
  }
}

void blackClockLoop() {
  if (isNewTurn && (currentGame.incrementType == DELAY)
      && ((millis() - blacksTurnInitMillis) < currentGame.incrementSeconds * 1000 )) {
    // wait delay
    if ((millis() - blacksTurnInitMillis) / 1000 % 10 == 0) {
      printTime(whitesTimeMillis, whitesRotation, whitesmoves, false);
      printTime(blacksTimeMillis, blacksRotation, blacksmoves, true);
    }
    return;
  } else if (isNewTurn && (currentGame.incrementType == DELAY)) {
    blacksTurnInitMillis = millis();
    blacksEllapsedTimeMillis = millis();
  }

  blacksTimeMillis = blacksTimeMillis - (millis() - blacksEllapsedTimeMillis );
  unsigned long blacksTime = blacksTimeMillis / 1000;
  blacksEllapsedTimeMillis = millis();
  if (blacksTime > 0  ) {
    if ( blacksOldTimeMillis / 1000 != blacksTime || isNewTurn) {
      if (isNewTurn) {
        printTime(whitesTimeMillis, whitesRotation, whitesmoves, false);
      }
      printTime(blacksTimeMillis, blacksRotation, blacksmoves, true);
      blacksOldTimeMillis = blacksTimeMillis;
      isNewTurn = false;
    }
  } else {
    state = END_GAME;
    printTime(0, blacksRotation, blacksmoves, true);
  }
}



void resetGame(void) {
  PROGMEMData (&games[selectedGameIndex], currentGame);

  if (currentGame.stages[0].duration + currentGame.incrementSeconds >= 3600) {
    clockDisplay = clockDisplayHours;
  } else {
    clockDisplay = clockDisplayMinutes;
  }
  whitesmoves = 0;
  blacksmoves = 0;
  Serial.println("RESET");
  state = IDLE;
  tft.fillScreen(backgroundColor);

  isNewTurn = false;

  whitesTimeMillis = currentGame.stages[0].duration * 1000 + 999;
  whitesOldTimeMillis = whitesTimeMillis;
  whitesTurnInitMillis = millis();
  whitesEllapsedTimeMillis = millis();


  blacksTimeMillis = currentGame.stages[0].duration * 1000 + 999;
  blacksOldTimeMillis = blacksTimeMillis;
  blacksTurnInitMillis = millis();
  blacksEllapsedTimeMillis = millis();

  clockDisplay.setOffColor(backgroundColor);
  printTime(whitesTimeMillis, whitesRotation, 0, false);
  printTime(blacksTimeMillis, blacksRotation, 0, false);
  printClockMode(foregroundColor);
  paintResetSettingsIcons(foregroundColor);


}

void printClockMode(uint16_t color) {
  printClockMode(whitesRotation, color);
  printClockMode(blacksRotation, color);
}

void printStages(int16_t x, int16_t y, uint16_t color) {
  tft.setCursor(x, y);
  tft.print("STG");
  for (int k = 0; k < currentGame.stagesNumber; k++) {
    printStageData(currentGame, x + 20 + k * 60 , y, k, color);
  }
}

void printClockMode(uint16_t rotation, uint16_t color) {
  tft.setRotation(rotation);
  printClockModeName(currentGame, 16, tft.height() - PLAYER_CLOCK_HEIGHT + 8, color);
  printClockDelay(currentGame, 80, tft.height() - PLAYER_CLOCK_HEIGHT + 8 , color);
  printStages(16, tft.height() - 27, color);
  tft.setRotation(INITIAL_ROTATION);
}

void showSettings(void) {
  state = SETTINGS;
  paintSettings();
}

void printClockModeName (GameType game, int16_t x, int16_t y, uint16_t color) {
  tft.setTextColor(color);
  tft.setCursor(x, y);
  if (game.incrementType == DELAY) {
    tft.print(F("US DELAY") );
  } else if (game.incrementType == BRONSTEIN) {
    tft.print(F("BRONSTEIN") );
  } else if (game.incrementType == FISCHER) {
    if (game.incrementSeconds == 0) {
      tft.print(F("NO INC") );
    } else {
      tft.print(F("FISCHER") );
    }
  }
}

void printClockDelay(GameType game, int16_t x, int16_t y, uint16_t color) {
  tft.setCursor(x, y);
  if (game.incrementSeconds > 0) {
    tft.print(F("INC ") ); tft.print(game.incrementSeconds); tft.print(F("s") );
  }
}

void printStageData(GameType game, int16_t x, int16_t y, int k, uint16_t color) {
  tft.setCursor(x, y);
  tft.print(game.stages[k].duration / 60  );
  tft.print(F("m" ));
  if (game.stages[k].moves > 0) {
    tft.print(F("/" ));
    tft.print(game.stages[k].moves);
    tft.print(F("mv" ));
  }
}

void paintSettingCell(int i, int j) {
  int cellWidth = tft.width() / settingsCols;
  int cellHeight = tft.height() / settingsRows;

  int gameIndex = j * settingsCols + i;
  tft.drawRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight, CYAN);
  GameType game;
  PROGMEMData (&games [gameIndex], game);
  printClockModeName(game, i * cellWidth + 2, j * cellHeight + 2, WHITE);
  printClockDelay(game, i * cellWidth + 2, j * cellHeight + 2 + 9, WHITE);

  for (int k = 0; k < game.stagesNumber; k++) {
    printStageData(game, i * cellWidth + 2, j * cellHeight + 2 + 18 + 9 * k, k, WHITE);
  }
}

void paintSettings() {
  tft.setRotation(INITIAL_ROTATION);
  tft.fillScreen(backgroundColor);

  int cellWidth = tft.width() / settingsCols;
  int cellHeight = tft.height() / settingsRows;
  tft.fillRect(selectedGameIndex % settingsCols * cellWidth, selectedGameIndex / settingsCols * cellHeight, cellWidth, cellHeight, BLACK);
  for (int j = 0; j < settingsRows; ++j) {
    for (int i = 0; i < settingsCols; ++i) {
      paintSettingCell(i, j);
    }
  }
}



void paintSettingsIcon(uint16_t color) {
  tft.drawBitmap(tft.width() / 2 - 96, tft.height() / 2 - 16, epd_bitmap_settings, 32, 32, color);
}

void paintResetIcon(uint16_t color) {
  tft.drawBitmap(tft.width() / 2 + 64, tft.height() / 2 - 16, epd_bitmap_reset, 32, 32, color);
}

void paintResetSettingsIcons(uint16_t color) {
  paintSettingsIcon(color);
  paintResetIcon(color);

}

void paintPauseIcon(uint16_t color) {

  tft.drawBitmap(tft.width() / 2 - 16, tft.height() / 2 - 16, epd_bitmap_pause, 32, 32, color);

}

void paintPawnsIcons() {

  tft.setRotation(blacksRotation);
  tft.drawBitmap(tft.width() - 26, tft.height() - 34, epd_bitmap_pawn , 16, 16, BLACK);

  tft.setRotation(whitesRotation);
  tft.drawBitmap(tft.width() - 26, tft.height() - 34, epd_bitmap_pawn, 16, 16, WHITE);

  tft.setRotation(INITIAL_ROTATION);
}



void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color, uint16_t borderWidth) {
  tft.startWrite();
  for (uint16_t i = 0; i < borderWidth; i++) {
    tft.writeFastHLine(x + i, y + i, w - 2 * i, color);
    tft.writeFastHLine(x + i, y + h - 1 - i, w - 2 * i, color);
    tft.writeFastVLine(x + i, y + i, h - 2 * i, color);
    tft.writeFastVLine(x + w - 1 - i, y + i, h - 2 * i, color);

  }
  tft.endWrite();
}


void printTime(const long newTime, const int rotation, uint16_t moves, const bool selected) {
  static bool toggleSeparator;
  toggleSeparator = !toggleSeparator;
  tft.setRotation(rotation);
  int16_t  x1, y1;
  uint16_t w, h;

  if (selected) {
    if (newTime == 0) {
      drawRect(5 , (int) tft.height() - PLAYER_CLOCK_HEIGHT, tft.width() - 10, PLAYER_CLOCK_HEIGHT - 10, alertColor, 5);
      clockDisplay.setOnColor( alertColor);
      movesDisplay.setOnColor(alertColor);
    } else {
      drawRect(5 , (int) tft.height() - PLAYER_CLOCK_HEIGHT, tft.width() - 10, PLAYER_CLOCK_HEIGHT - 10, tft.color565(255, 255, 0), 5);
      clockDisplay.setOnColor(foregroundColor);
      movesDisplay.setOnColor(foregroundColor);
    }
  } else {
    drawRect(5 , (int) tft.height() - PLAYER_CLOCK_HEIGHT, tft.width() - 10, PLAYER_CLOCK_HEIGHT - 10, tft.color565(0, 0, 0), 5);
    clockDisplay.setOnColor(BLACK);
    movesDisplay.setOnColor(BLACK);
  }
  clockDisplay.displayMillis(newTime, toggleSeparator || !selected);
  movesDisplay.display(moves);
  tft.setRotation(INITIAL_ROTATION);
}


void printPauseTime(const long newTime, const int rotation, uint16_t moves) {
  tft.setRotation(rotation);
  int16_t  x1, y1;
  uint16_t w, h;

  clockDisplay.setOnColor(pauseColor);
  movesDisplay.setOnColor(pauseColor);

  clockDisplay.displayMillis(newTime, true);
  movesDisplay.display(moves);
  tft.setRotation(INITIAL_ROTATION);
}

void changeSettingsSelectionTo(int newSelectedGameIndex) {
  tft.setRotation(INITIAL_ROTATION);
  int cellWidth = tft.width() / settingsCols;
  int cellHeight = tft.height() / settingsRows;

  tft.fillRect(selectedGameIndex % settingsCols * cellWidth, selectedGameIndex / settingsCols * cellHeight, cellWidth, cellHeight, backgroundColor);
  paintSettingCell(selectedGameIndex % settingsCols, selectedGameIndex / settingsCols);

  tft.fillRect(newSelectedGameIndex % settingsCols * cellWidth, newSelectedGameIndex / settingsCols * cellHeight, cellWidth, cellHeight, BLACK);
  paintSettingCell(newSelectedGameIndex % settingsCols, newSelectedGameIndex / settingsCols);

  selectedGameIndex = newSelectedGameIndex;
}


uint16_t readUiSelection() {
  tft.setRotation(INITIAL_ROTATION);
  int16_t xpos, ypos;  //screen coordinates
  TSPoint tp = ts.getPoint();   //tp.x, tp.y are ADC values

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);


  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE && millis() - lastTimeTouch > 400 ) {
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    //    Serial.println(F("Touch"));
    xpos = map(tp.x, TS_RT, TS_LEFT, 0, tft.width());
    ypos = map(tp.y, TS_BOT, TS_TOP, 0, tft.height());

    lastTimeTouch = millis();
    if (state == END_GAME) {
      resetGame();
      return state;
    }
    if (state == SETTINGS) {
      int cellWidth = tft.width() / settingsCols;
      int cellHeight = tft.height() / settingsRows;
      int newSelectedGameIndex = (xpos / cellWidth)  + (ypos / cellHeight) *  settingsCols;
      if (newSelectedGameIndex == selectedGameIndex) {
        resetGame();
      } else {
        changeSettingsSelectionTo(newSelectedGameIndex);
      }
      return state;
    }

    // are we in buttons area ?
    if ( ypos > PLAYER_CLOCK_HEIGHT && ypos < tft.height() - PLAYER_CLOCK_HEIGHT ) {
      if (state == IDLE || (state == BLACK_IN_PAUSE || state == WHITE_IN_PAUSE) && (ypos >  tft.height() / 2 - 32) && (ypos <  tft.height() / 2 + 32)) {
        if ( xpos <  tft.width() / 3  ) {
          state = SETTINGS;
          showSettings();
          return state;
        } else if ( xpos > 2 * tft.width() / 3  ) {
          state = IDLE;
          resetGame();
          return state;
        } else if (state == BLACK_IN_PAUSE) {
          state = BLACK_PLAYING;
          blacksEllapsedTimeMillis = millis();
          paintPauseIcon(foregroundColor);
          paintResetSettingsIcons(backgroundColor);
          return state;
        } else if (state == WHITE_IN_PAUSE ) {
          state = WHITE_PLAYING;
          whitesEllapsedTimeMillis = millis();
          paintPauseIcon(foregroundColor);
          paintResetSettingsIcons(backgroundColor);
          return state;
        }
      } else if (state == BLACK_PLAYING) {
        state = BLACK_IN_PAUSE;
        printPauseTime(whitesTimeMillis, whitesRotation, whitesmoves);
        printPauseTime(blacksTimeMillis, blacksRotation, blacksmoves);
        paintPauseIcon(alertColor);
        paintResetSettingsIcons(alertColor);
        return state;

      } else if (state == WHITE_PLAYING) {
        state = WHITE_IN_PAUSE;
        printPauseTime(whitesTimeMillis, whitesRotation, whitesmoves);
        printPauseTime(blacksTimeMillis, blacksRotation, blacksmoves);
        paintPauseIcon(alertColor);
        paintResetSettingsIcons(alertColor);
        return state;
      }
    }

    if (state == IDLE) {
      printClockMode(BLACK);
      if (currentGame.incrementType == FISCHER || currentGame.incrementType == BRONSTEIN) {
        whitesTimeMillis += (currentGame.incrementSeconds * 1000);
      }
      state = WHITE_PLAYING;
      // assign white color
      if (ypos > PLAYER_CLOCK_HEIGHT) {
        // White down
        isWhiteDown = true;
        whitesRotation = 2;
        blacksRotation = 0;
      } else {
        // White up
        isWhiteDown = false;
        whitesRotation = 0;
        blacksRotation = 2;
      }
      paintPawnsIcons();
      paintPauseIcon(foregroundColor);
      isNewTurn = true;

      whitesTurnInitMillis = millis();
      whitesEllapsedTimeMillis = millis();
      return state;
    }

    if ((state == WHITE_PLAYING) &&
        (((ypos > PLAYER_CLOCK_HEIGHT) && !isWhiteDown )
         || (ypos < PLAYER_CLOCK_HEIGHT) && isWhiteDown )) {

      ++whitesmoves;
      if (currentGame.incrementType == BRONSTEIN) {
        unsigned long changeTimeMillis = millis();
        unsigned long timeExpendedMillis = changeTimeMillis - whitesTurnInitMillis;
        if ( timeExpendedMillis < currentGame.incrementSeconds * 1000 ) {
          whitesTimeMillis -= (currentGame.incrementSeconds * 1000 - timeExpendedMillis) ;
        }
      }

      state = BLACK_PLAYING;
      if (currentGame.incrementType == FISCHER || currentGame.incrementType == BRONSTEIN) {
        blacksTimeMillis += (currentGame.incrementSeconds * 1000);
      }
      blacksTurnInitMillis = millis();
      blacksEllapsedTimeMillis = millis();
      isNewTurn = true;

    } else if ((state == BLACK_PLAYING) &&
               (((ypos > PLAYER_CLOCK_HEIGHT) && isWhiteDown )
                || (ypos < PLAYER_CLOCK_HEIGHT) && !isWhiteDown )) {

      state = WHITE_PLAYING;
      if (currentGame.incrementType == FISCHER || currentGame.incrementType == BRONSTEIN) {
        whitesTimeMillis += (currentGame.incrementSeconds * 1000);
      }
      ++blacksmoves;
      if (currentGame.incrementType == BRONSTEIN) {
        unsigned long changeTimeMillis = millis();
        unsigned long timeExpendedMillis = changeTimeMillis - blacksTurnInitMillis;
        if ( timeExpendedMillis < currentGame.incrementSeconds * 1000 ) {
          blacksTimeMillis -=   (currentGame.incrementSeconds * 1000 - timeExpendedMillis) ;
        }
      }
      whitesTurnInitMillis = millis();
      whitesEllapsedTimeMillis = millis();
      isNewTurn = true;

    }

  }
  return -1;
}
