#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
//#include "buzzer.h"
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
    int OnStartRecording(int arg);
    int OnStopRecording(int arg);
    int OnCheckForExpiredTimers(int arg);
    int OnTimerEvent(int arg);
    int OnIdleEvent(int arg);
    int OnUnknown(int msg, int arg);


    // Read hardware inputs
    bool readSwitchHook(bool hookUp);
    bool read_input(int& msg, int& arg);

    // Debug helper
    void printMessage(char *text, int msg, int arg);

    // Event dispatch function
    int message_proc(int msg, int arg);

    void loop();
    void setup();
    int inputTime();

// data
    IntervalTimer drawTimer_;
    int currentTimer_;
    //Buzzer buzzer_;
    bool buzzerOn_;
    bool buzzerRunning_;
    bool hookUp_;
    int recording_;
    MessageQueue messageQueue_;
};

#endif

