#ifndef TIMER_H_
#define TIMER_H_

// Assign pins
static const int BUZZER = 3;
static const int RCLK = 5;
static const int SRCLR = 6;
static const int HOOK = 9;
static const int AUDIO_CS = 10;     // ISD1760
static const int SRCLK = 14;
static const int SER_DATA = 15;
static const int ROW_1 = 16;
static const int ROW_2 = 17;
static const int ROW_3 = 20;
static const int ROW_4  = 21;

// Bit to set the column in the shift register
static const int COL_1C1 = 1;
static const int COL_1C2 = 2;
static const int COL_1C3 = 3;
static const int COL_1C4 = 4;
static const int COL_2C1 = 5;
static const int COL_2C2 = 6;
static const int COL_2C3 = 7;

/*********************************************
 * timer functions
 */
namespace timer_ns
{
    static const int MAX_TIMERS = 15;
    extern int timers[MAX_TIMERS];

    int createTimer(int timeout);
    void clearTimer(int timerno);
    int checkTimer(int timerno);
    void setup();
}

/********************************************
 * Display functions
 */
namespace display_ns
{
    // Display settings
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = 4;
    extern Adafruit_SSD1306 display;

    void showTimerExpired(int timerno, int secs);
    void showTimerRunning(int timerno, int secs);
    void showTimer(int timerno);
    void setup();
}

/*******************************************
 * Keypad functions
 */
namespace keypad_ns
{
    static const int KEY_NONE = 0;
    static const int KEY_0 = 1;
    static const int KEY_1 = 2;
    static const int KEY_2 = 3;
    static const int KEY_3 = 4;
    static const int KEY_4 = 5;
    static const int KEY_5 = 6;
    static const int KEY_6 = 7;
    static const int KEY_7 = 8;
    static const int KEY_8 = 9;
    static const int KEY_9 = 10;
    static const int KEY_HASH = 11;
    static const int KEY_STAR = 12;
    static const int KEY_REDIAL = 13;

    int getKey();
    char keycode2char(int k);
    void setup();
}

/******************************************
 * Audio functions
 */
namespace audio_ns
{
    int recordMessage();
    void playMessage(int);
    void ringTimer();
    void buzzer(int freq);

    void setup();
}
/*****************************************
 * Test code
 */

struct Loop
{
    virtual void loop();
    virtual void setup();
};

struct TimerTest : public Loop
{
    void setup();
    void loop();
};

struct TimerTest2 : public Loop
{
    void setup();
    void loop();
};

struct KeypadTest : public Loop
{
    void setup();
    void loop();
};

#endif TIMER_H_

