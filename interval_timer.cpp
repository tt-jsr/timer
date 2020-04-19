#include <Arduino.h>
#include "interval_timer.h"

IntervalTimer::IntervalTimer()
: interval_(0)
, startTime_(0)
{}

void IntervalTimer::set(int intervalInMillis)
{
    interval_= intervalInMillis;
    startTime_ = millis();
}

bool IntervalTimer::check()
{
    if (interval_ == 0)
        return false;
    if (millis() > (interval_ + startTime_))
    {
        startTime_ = millis();
        return true;
    }
    return false;
}
