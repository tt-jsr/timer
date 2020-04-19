#include "buzzer.h"

void Buzzer::startBuzzer()
{
    if (buzzerOn_ || buzzerOff_)
        return; // buzzer is already running

    buzzerOn_ = millis()-1;
    buzzerOff_ = 0;
}

void Buzzer::endBuzzer()
{
    noTone(BUZZER);
    buzzerOn_ = 0;
    buzzerOff_ = 0;
}

void Buzzer::setup()
{
    buzzerOn_ = 0;
    buzzerOff_ = 0;
}

void Buzzer::loop()
{
    long now = millis();
    if (buzzerOn_ != 0)
    {
        if (now < buzzerOn_)
            return;  // not yet
        tone(BUZZER, 1000);
        buzzerOff_ = now + 1000;
        buzzerOn_ = 0;
        return;
    }
    if (buzzerOff_ != 0)
    {
        if (now < buzzerOff_)
            return;  // not yet
        noTone(BUZZER);
        buzzerOff_ = 0;
        buzzerOn_ = now + 1000;
    }
}

