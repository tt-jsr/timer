#ifndef BUZZER_H_
#define BUZZER_H_

#include "timer.h"

struct Buzzer
{
    void startBuzzer();
    void endBuzzer();
    void setup();
    void loop();

    long buzzerOn_;
    long buzzerOff_;
};

#endif
