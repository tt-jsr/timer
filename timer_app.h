#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
#include "buzzer.h"

struct Message
{
    int msg_type;
    int arg;
};

#define MAX_MESSAGES 16 // power of two

struct TimerApp : public Application
{
    TimerApp();
    void displayNoTimers();
    int createTimer();
    bool readSwitchHook(bool hookUp);
    void message_proc(int msg, int arg);
    int getExpiredTimer();
    bool read_input(int& msg, int& arg);
    bool get_message(int& msg, int& arg);
    void post_message(int msg, int arg);
    void loop();
    void setup();

    void printMessage(char *text, int msg, int arg);
    int currentTimer_;
    Buzzer buzzer_;
    bool hookUp_;
    Message msg_queue_[MAX_MESSAGES];
    int consumer_;
    int producer_;
};

#endif

