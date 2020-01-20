#ifndef TIMER_H_
#define TIMER_H_

// Assign pins
static const int ROW_1 = 16;
static const int ROW_2 = 17;
static const int ROW_3 = 20;
static const int ROW_4  = 20;
static const int SRCLK = 14;
static const int SER_DATA = 15;
static const int SRCLR = 24;
static const int RCLK = 23;

// Bit to set the column in the shift register
static const int COL_1C1 = 1;
static const int COL_1C2 = 2;
static const int COL_1C3 = 3;
static const int COL_1C4 = 4;
static const int COL_2C1 = 5;
static const int COL_2C2 = 6;
static const int COL_2C3 = 7;

// Display settings
static const int SCREEN_WIDTH = 128;
static const int SCREEN_HEIGHT = 64;
static const int OLED_RESET = 4;

extern Adafruit_SSD1306 display;


// Timer settigs
static const int MAX_TIMERS = 15;
extern int timers[MAX_TIMERS];


/*********************************************
 * timer functions
 */
int createTimer(int timeout);
void clearTimer(int timerno);
void setupTimer();

/********************************************
 * Display functions
 */
void showTimerExpired(int timerno, int secs);
void showTimerRunning(int timerno, int secs);
void showTimer(int timerno);
void setupDisplay();

/*******************************************
 * Keypad functions
 */
static const int KEY_0 = 0;
static const int KEY_1 = 1;
static const int KEY_2 = 2;
static const int KEY_3 = 3;
static const int KEY_4 = 4;
static const int KEY_5 = 5;
static const int KEY_6 = 6;
static const int KEY_7 = 7;
static const int KEY_8 = 8;
static const int KEY_9 = 9;
static const int KEY_HASH = 10;
static const int KEY_STAR = 11;
static const int KEY_REDIAL = 12;

int getKey();
void setupKeypad();

#endif TIMER_H_

