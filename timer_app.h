#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
#include "message_queue.h"

const int KEY_EVENT = USER_EVENT_BASE;
// id's
static const int BUZZER_TIMER = 1;
static const int DRAW_TIMER = 2;
static const int CHECK_TIMERS = 3;
static const int BUZZER_STATE = 4;
static const int HOOK_STATE = 5;

// HOOK_STATE Values
static const int SWITCH_HOOK_UP = LOW;
static const int SWITCH_HOOK_DOWN = HIGH;

// BUZZER_STATE values
static const int TURN_BUZZER_ON = 1;
static const int TURN_BUZZER_OFF = 0;

struct AppMessageQueue : MessageQueue
{
    AppMessageQueue(int msg, int timers, int pins, int values, int pulses)
    :MessageQueue(msg, timers, pins, values, pulses)
    {}
    void OnGenerator();
};

struct TimerApp : public Application
{
    TimerApp();

    // Event handlers
    void OnKeyEvent(char c);
    void OnTimerEvent(int id);
    void OnIdleEvent();
    void OnValueEvent(int id, unsigned long value);
    void OnUnknown(char *, int arg1, unsigned long arg2);
    int OnCheckForExpiredTimers();
    int OnDrawTimer();
    int OnBuzzerTimer();
    int OnBuzzerStateChange(int value);

    // Functions
    int CreateNewTimer(char c);
    void CancelTimer(int timerno);
    void SwitchToTimer(int timerno);
    void PlayMessage(int timerno);
    void StopPlayingMessage(int timerno);
    void StartRecording(int timerno);
    void StopRecording(int timerno);
    void OnSwitchHookUp();
    void OnSwitchHookDown();
    void TimerExpired(int t);


    // Read hardware inputs
    bool read_keypad(char& c);

    void read_keypad_ungetc(char c);

    // Debug helper
    void printMessage(char *text, int msg, int arg1, unsigned long arg2);

    void loop();
    void setup(bool debug);
    int inputTime(char c);

// data
    int currentTimer_;      // The current timerno being displayed
    bool buzzerRunning_;    // Buzzer is running. This means BUZZER timer event is running
    int recordingTimer_;    // The timer being recorded
    int playingTimer_;      // The timer being played
    char ungetc_;           
    AppMessageQueue messageQueue_;
    bool debug_;
};

#endif

