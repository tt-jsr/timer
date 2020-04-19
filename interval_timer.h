#ifndef INTERVAL_TIMER_H_
#define INTERVAL_TIMER_H_

class IntervalTimer
{
public:
    IntervalTimer();

    void set(int milli);

    bool check();
private:
    unsigned long interval_;
    unsigned long startTime_;
};

#endif
