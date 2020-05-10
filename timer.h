#ifndef TIMER_H_
#define TIMER_H_

#include <Adafruit_SSD1306.h>

#define PRINT1(a1, a2) Serial.print(a1);Serial.println(a2)
#define PRINT2(a1, a2, a3) Serial.print(a1);Serial.print(a2);Serial.println(a3)

// Assign pins
static const int PIN_BUZZER = 2;
static const int PIN_RCLK = 5;
static const int PIN_SRCLR = 6;
static const int PIN_HOOK = 9;
static const int PIN_AUDIO_CS = 10;     // ISD1760
static const int PIN_SRCLK = 14;
static const int PIN_SER_DATA = 15;
static const int PIN_ROW_1 = 16;
static const int PIN_ROW_2 = 17;
static const int PIN_ROW_3 = 20;
static const int PIN_ROW_4  = 21;

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
    static const int MAX_TIMERS = 10;
    static const int TIMER_INVALID = 32767;

    // Return the timerno on success, otherwise -1
    int createTimer(int timeout);

    // Start the timer
    bool startTimer(int timerno);

    // Cancel a timer
    void clearTimer(int timerno);

    // Returns true if the time is not TIMER_INVALID
    bool isTimerRunning(int timerno);

    // Returns true if the timer has expired
    bool isTimerExpired(int timerno);

    // returns the next timer to expire
    int nextRunningTimer();

    // Returns the oldest expired timer
    int nextExpiredTimer();

    // Return  the remaining time. If < 0, it has expired and is 
    // now counting up.
    // returns TIMER_INVALID if the timer is not valid
    int timeRemaining(int timerno);


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

    static const int FLAG_CLEAR = 0x01;         // Clear the display
    static const int FLAG_SMALL_FONT = 0x02;    // Use small font
    static const int FLAG_LARGE_FONT = 0x04;    // use large font
    static const int FLAG_DISPLAY = 0x08;       // Write to screen
    static const int FLAG_LINES = 0x10;         // y-coordinate is in lines

    // Write a message to the screen
    void print(int flags, int x, int y, char *text);

    // Clesar the display
    void clearDisplay();

    // Set the current font to small
    void setSmallFont();

    // Set the current font to large
    void setLargeFont();

    // Display an expired timer
    void showTimerExpired(int timerno, int secs);

    // Display a running timer
    void showTimerRunning(int timerno, int secs);

    // Display that there are no timers running
    void showNoTimers();

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

    // Return a keypress. Blocks until a key is pressed
    char getKey();

    // Return a key, or 0 if none pressed
    // Returns immediately, does not block waiting for a keypress
    char getKeyPress();

    void setup();
}

/******************************************
 * Audio functions
 */
namespace audio_ns
{
    int startRecording(int);
    void stopRecording(int);
    void startPlaying(int);
    void stopPlaying(int);
    void eraseRecording(int);
    void playRingTone();
    bool isRecordingAvailable(int timerno);

    void setup();
}

#endif TIMER_H_

