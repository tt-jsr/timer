#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
#include "buzzer.h"
#include "interval_timer.h"
#include "message_queue.h"

struct TimerApp : public Application
{
    TimerApp();
    int inputTime();
    bool readSwitchHook(bool hookUp);
    int message_proc(int msg, int arg);
    int getExpiredTimer();
    bool read_input(int& msg, int& arg);
    bool get_message(int& msg, int& arg);
    void post_message(int msg, int arg);
    void loop();
    void setup();

    void printMessage(char *text, int msg, int arg);
    IntervalTimer drawTimer_;
    int currentTimer_;
    Buzzer buzzer_;
    bool hookUp_;
    int recording_;
    MessageQueue messageQueue_;
};

#endif

