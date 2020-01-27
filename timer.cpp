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
                //char buf[32];
                //sprintf(buf, "Timer %d = %d secs\n", i, timeout);
                //Serial.print(buf);
                return i;
            }
        }
        return -1;
    }

    /**************************************/
    int nextTimer()
    {
        int closest = TIMER_INVALID;
        for (int i = 0; i <MAX_TIMERS; i++)
        {
            int t = checkTimer(i);
            if (t != TIMER_INVALID)
            {
                if (t < 0)
                    t -=t;
                if (t < closest)
                    closest = t;
            }
        }
        return closest;
    }

    /**************************************/
    void clearTimer(int timerno)
    {
      if (timerno < 0 || timerno >MAX_TIMERS)
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
    int checkTimer(int timerno)
    {
      if (timerno < 0 || timerno > MAX_TIMERS)
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
