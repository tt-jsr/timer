#include "buzzer.h"

void Buzzer::startBuzzer()
{
    if (running_)
        return; // buzzer is already running

    running_ = true;
    intervalTimer_.set(500);
    buzzerOn_ = false;
}

void Buzzer::endBuzzer()
{
    noTone(BUZZER);
    running_ = false;
    buzzerOn_ = false;
}

void Buzzer::setup()
{
    running_ = false;
    buzzerOn_ = false;
}

void Buzzer::loop()
{
    if (running_ == false)
        return;

    if (intervalTimer_.check())
    {
        if (buzzerOn_)
            noTone(BUZZER);
        else
            tone(BUZZER, 1000);
        buzzerOn_ = !buzzerOn_;
    }
}

