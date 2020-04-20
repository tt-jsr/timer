#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

namespace timer_ns
{
    int timers[MAX_TIMERS];

    /**************************************/
    int createTimer(int timeout)
    {
        for (int i = 0; i < MAX_TIMERS; i++)
        {
            if (timers[i] == TIMER_INVALID)
            {
                timers[i] = millis()/1000 + timeout;
                return i;
            }
        }
        return -1;
    }

    /**************************************/
    int nextTimer()
    {
        int closest = TIMER_INVALID;
        int timerno = TIMER_INVALID;
        for (int i = 0; i < MAX_TIMERS; i++)
        {
            int t = checkTimer(i);
            if (t != TIMER_INVALID)
            {
                if (t > 0 && t < closest)
                {
                    closest = t;
                    timerno = i;
                }
            }
        }
        return timerno;
    }

    /**************************************/
    void clearTimer(int timerno)
    {
      if (timerno < 0 || timerno >= MAX_TIMERS)
          return;
       timers[timerno] = TIMER_INVALID;
    }

    /**************************************/
    bool isTimerRunning(int timerno)
    {
        if (timerno < 0 || timerno >= MAX_TIMERS)
            return false;
        return timers[timerno] != TIMER_INVALID;
    }

    /**************************************/
    bool isTimerExpired(int timerno)
    {
        int t = checkTimer(timerno);
        if (t == TIMER_INVALID)
            return false;
        if (t < 0)
            return true;
        return false;
    }

    /**************************************/
    int getExpiredTimer()
    {
        int oldestTime = 0;
        int oldestTimer = TIMER_INVALID;
        for (int timer = 0; timer < timer_ns::MAX_TIMERS; timer++)
        {
            int t = checkTimer(timer);
            if (t != TIMER_INVALID)
            {
                if (t < oldestTime)
                {
                    oldestTime = t;
                    oldestTimer = timer;
                }
            }
        }
        return oldestTimer;
    }

    /**************************************/
    int checkTimer(int timerno)
    {
      if (timerno < 0 || timerno >= MAX_TIMERS)
          return TIMER_INVALID;
      if (timers[timerno] == TIMER_INVALID)
          return TIMER_INVALID;

      int now = millis()/1000;
      return timers[timerno]-now;
    }


    /**************************************/
    void setup() 
    {
      for (int i= 0; i < MAX_TIMERS; i++)
          timers[i] = TIMER_INVALID;
    }
}
