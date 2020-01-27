#ifndef TIMER_H_
#define TIMER_H_

// Assign pins
static const int BUZZER = 2;
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
static const int KS_COL_1C1 = 0;
static const int KS_COL_1C2 = 1;
static const int KS_COL_1C3 = 2;
static const int KS_COL_1C4 = 3;
static const int KS_COL_2C1 = 4;
static const int KS_COL_2C2 = 5;
static const int KS_COL_2C3 = 6;

static const int KS_ROW_1 = 0;
static const int KS_ROW_2 = 1;
static const int KS_ROW_3 = 2;
static const int KS_ROW_4 = 3;

/*********************************************
 * timer functions
 */
namespace timer_ns
{
    static const int MAX_TIMERS = 15;
    static const int TIMER_INVALID = 1000000;
    extern int timers[MAX_TIMERS];

    // Return the timerno on success, otherwise -1
    int createTimer(int timeout);

    void clearTimer(int timerno);

    bool isTimerRunning(int timerno);

    int nextTimer();

    // Return  the remianing time. If < 0, it has expired and is 
    // now counting up.
    // returns TIMER_INVALID if the timer is not valid
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
    extern int TEXT_HEIGHT; // Height of current font
    extern int TIMER_X;
    extern int TIMER_Y;

    void clearDisplay();
    void setSmallFont();
    void setLargeFont();
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
    enum KeyCode {
        KEY_NONE = 0
        , KEY_0 = 1
        , KEY_1 = 2
        , KEY_2 = 3
        , KEY_3 = 4
        , KEY_4 = 5
        , KEY_5 = 6
        , KEY_6 = 7
        , KEY_7 = 8
        , KEY_8 = 9
        , KEY_9 = 10
        , KEY_HASH = 11
        , KEY_STAR = 12
        , KEY_REDIAL = 13
    };

    // Returns a keypresss. Blocks until q key is pressed
    char getKey();

    // Return a keyy, or 0 if none pressed
    char getKeyPress();

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
 * Application loop
 */

struct Application
{
    virtual void loop();
    virtual void setup();
};

#endif TIMER_H_

