#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
#include "interval_timer.h"
#include "message_queue.h"

struct TimerApp : public Application
{
    TimerApp();

    // Event handlers
    int OnCreateNewTimer(int arg);
    int OnCancelTimer(int arg);
    int OnTimerExpired(int arg);
    int OnSwitchToTimer(int arg);
    int OnDrawTimer(int arg);
    int OnSwitchHookUp(int arg);
    int OnSwitchHookDown(int arg);
    int OnKey(int arg);
    int OnPlayMessage(int arg);
    int OnStopPlayingMessage(int timerno);
    int OnStartRecording(int arg);
    int OnStopRecording(int arg);
    int OnCheckForExpiredTimers(int arg);
    int OnTimerEvent(int arg);
    int OnIdleEvent();
    int OnUnknown(int msg, int arg);


    // Read hardware inputs
    bool readSwitchHook(bool hookUp);
    bool read_input(int& msg, int& arg);

    // push back one character to be returned by read_input()
    // as a EVENT_MSG_KEY
    void read_input_ungetc(char c);

    // Debug helper
    void printMessage(char *text, int msg, int arg);

    void loop();
    void setup();
    int inputTime(char c);

// data
    IntervalTimer drawTimer_;
    int currentTimer_;      // The current timerno being displayed
    bool buzzerOn_;         // Buzzer is currently on
    bool buzzerRunning_;    // Buzzer is running. This means BUZZER timer event is running
    bool hookUp_;           // The swicth hok is up
    int recordingTimer_;    // The timer being recorded
    int playingTimer_;      // The timer being played
    char ungetc_;           
    MessageQueue messageQueue_;
};

#endif

