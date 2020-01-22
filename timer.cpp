#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

namespace timer_ns
{
    int timers[MAX_TIMERS];

    int createTimer(int timeout)
    {
        for (int i = 0; i < MAX_TIMERS; i++)
        {
            if (timers[i] == 0)
            {
                timers[i] = millis()/1000 + timeout;
#if defined(MONITOR)
                char buf[32];
                sprintf(buf, "Timer %d = %d secs\n", i, timeout);
                Serial.print(buf);
#endif
                return i;
            }
        }
        return -1;
    }

    void clearTimer(int timerno)
    {
       timers[timerno] = 0;
    }

    // Return  the remianing time. If < 0, it has expired and is 
    // now counting up.
    int checkTimer(int timerno)
    {
      int now = millis()/1000;
      return timers[timerno]-now;
    }

    void setup() 
    {
      memset(timers, 0, sizeof(int)*MAX_TIMERS);
    }
}
