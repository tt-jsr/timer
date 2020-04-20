#ifndef BUZZER_H_
#define BUZZER_H_

#include "timer.h"
#include "interval_timer.h"

struct Buzzer
{
    void startBuzzer();
    void endBuzzer();
    void setup();
    void loop();

    bool running_;
    bool buzzerOn_;
    IntervalTimer intervalTimer_;
};

#endif
