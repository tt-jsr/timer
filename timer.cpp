#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

namespace timer_ns
{
    bool debug = false;
    struct Timer
    {
        int interval;
        int trigger;
        bool running;
        bool recording_available;
    };

    Timer timers[timer_ns::MAX_TIMERS];

    /**************************************/
    int createTimer(int timeout)
    {
        for (int i = 0; i < timer_ns::MAX_TIMERS; i++)
        {
            if (timers[i].interval == 0)
            {
                timers[i].interval = timeout;
                timers[i].trigger = 0;
                timers[i].running = false;
                timers[i].recording_available = false;
                if (debug)
                {
                    Serial.print("app timer created, secs: ");
                    Serial.println(timeout);
                }
                return i;
            }
        }
        return -1;
    }

    /**************************************/
    bool startTimer(int timerno)
    {
        if (timers[timerno].interval == 0)
            return false;
        if (timers[timerno].running)
            return true;

        timers[timerno].trigger = millis()/1000 + timers[timerno].interval;
        timers[timerno].running = true;
        if (debug)
            Serial.println("app timer started");
        return true;
    }

    /**************************************/
    int nextRunningTimer()
    {
        int closest = TIMER_INVALID;
        int timerno = TIMER_INVALID;
        int now = millis()/1000;
        for (int i = 0; i < timer_ns::MAX_TIMERS; i++)
        {
            if (timers[i].running && timers[i].trigger > now)
            {
                if (timers[i].trigger < closest)
                {
                    closest = timers[i].trigger;
                    timerno = i;
                }
            }
        }
        return timerno;
    }

    /**************************************/
    void clearTimer(int timerno)
    {
      if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
          return;
       timers[timerno].interval = 0;
       timers[timerno].trigger = 0;
       timers[timerno].running = 0;
       timers[timerno].recording_available = false;
        if (debug)
            Serial.println("app timer cleared");
    }

    /**************************************/
    bool isTimerRunning(int timerno)
    {
        if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
            return false;
        return timers[timerno].running;
    }

    /**************************************/
    bool isTimerExpired(int timerno)
    {
        int t = timeRemaining(timerno);
        if (t == TIMER_INVALID)
            return false;
        if (t < 0)
            return true;
        return false;
    }

    /**************************************/
    int nextExpiredTimer()
    {
        int oldest = millis()/1000;
        int timerno = TIMER_INVALID;
        for (int i = 0; i < timer_ns::MAX_TIMERS; i++)
        {
            if (timers[i].running && timers[i].trigger < oldest)
            {
                oldest = timers[i].trigger;
                timerno = i;
            }
        }
        return timerno;
    }

    /**************************************/
    int timeRemaining(int timerno)
    {
      if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
          return TIMER_INVALID;
      if (timers[timerno].interval == 0)
          return TIMER_INVALID;
      if (timers[timerno].running == false)
          return TIMER_INVALID;

      int now = millis()/1000;
      return timers[timerno].trigger-now;
    }

    /**************************************/
    void setRecordingAvailable(int timerno)
    {
      if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
          return;
      timers[timerno].recording_available = true;
    }

    /**************************************/
    bool getRecordingAvailable(int timerno)
    {
      if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
          return false;
      return timers[timerno].recording_available;
    }

    /**************************************/
    void setup(bool dbg) 
    {
        debug = dbg;
        for (int i= 0; i < timer_ns::MAX_TIMERS; i++)
        {
            timers[i].interval = 0;
            timers[i].trigger = 0;
            timers[i].running = 0;
            timers[i].recording_available = true;
        }
    }
}
