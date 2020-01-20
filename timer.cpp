#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
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

void setupTimer() 
{
  memset(timers, 0, sizeof(int)*MAX_TIMERS);
}
