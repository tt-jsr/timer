#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "test.h"

struct Buzzer
{
    void startBuzzer()
    {
        if (buzzerOn_ || buzzerOff_)
            return; // buzzer is already running

        buzzerOn_ = millis()-1;
        buzzerOff_ = 0;
    }

    void endBuzzer()
    {
        noTone(BUZZER);
        buzzerOn_ = 0;
        buzzerOff_ = 0;
    }

    void setup()
    {
        buzzerOn_ = 0;
        buzzerOff_ = 0;
    }

    void loop()
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

    long buzzerOn_;
    long buzzerOff_;
};

#define MAX_DIGITS 5
#define BUFFER_SIZE 8 // must be power of two

struct TimerBuffer
{
    TimerBuffer()
    {
        memset(buf, 0, BUFFER_SIZE);
        pos = 0;
    }

    void addChar(char c)
    {
        buf[(pos+1) & (BUFFER_SIZE-1)] = c;
        ++pos;
    }

    char getChar(size_t idx)
    {
        size_t s = size();
        if (idx >= s)
            return 0;
        idx = s-(idx+1);
        return buf[(pos-idx) & (BUFFER_SIZE-1)];
    }

    size_t size()
    {
        if (pos > MAX_DIGITS)
            return MAX_DIGITS;
        return pos;
    }

    int getSeconds()
    {
        int secs = 0;

        size_t idx = size()-1;
        char c = getChar(idx);
        secs += c - '0';

        c = getChar(--idx);
        if (c == 0)
            return secs;
        secs += (c - '0') * 10;

        c = getChar(--idx);
        if (c == 0)
            return secs;
        secs += (c - '0') * 60;

        c = getChar(--idx);
        if (c == 0)
            return secs;
        secs += (c - '0') * 600;

        c = getChar(--idx);
        if (c == 0)
            return secs;
        secs += (c - '0') * 3600;

        return secs;
    }

    void format(char *buf, size_t buflen)
    {
        size_t startpos = 0;
        if (pos > MAX_DIGITS)
            startpos = (pos-MAX_DIGITS) & (BUFFER_SIZE-1);
        switch (size())
        {
        case 5:
            buf[0] = getChar(0);
            buf[1] = ':';
            buf[2] = getChar(1);
            buf[3] = getChar(2);
            buf[4] = ':';
            buf[5] = getChar(3);
            buf[6] = getChar(4);
            buf[7] = 0;
            break;
        case 4:
            buf[0] = getChar(0);
            buf[1] = getChar(1);
            buf[2] = ':';
            buf[3] = getChar(2);
            buf[4] = getChar(3);
            buf[5] = 0;
            break;
        case 3:
            buf[0] = getChar(0);
            buf[1] = ':';
            buf[2] = getChar(1);
            buf[3] = getChar(2);
            buf[4] = 0;
            break;
        case 2:
            buf[0] = getChar(0);
            buf[1] = getChar(1);
            buf[2] = 0;
            break;
        case 1:
            buf[0] = getChar(0);
            buf[1] = 0;
            break;
        }
    }

    char buf[BUFFER_SIZE];
    size_t pos;
};

struct TimerApp : public Application
{
    void displayNoTimers()
    {
        display_ns::setSmallFont();
        int x, y, w, h;
        display_ns::display.getTextBounds("No timers", 0, 0, &x, &y, &w, &h);
        currentTimer_ = timer_ns::TIMER_INVALID;
        y = 64/2+h/2;
        x = 128/2-w/2;
        display_ns::display.setCursor(x, y);
        display_ns::display.print("No Timers");
        display_ns::display.display();
    }

    int setTimer()
    {
        TimerBuffer tbuf;
        display_ns::display.clearDisplay();
        display_ns::setSmallFont();
        display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
        display_ns::display.print("Enter time:");
        display_ns::display.display();
        while(true)
        {
            char c = keypad_ns::getKey();
            if (c == '#' || digitalRead(HOOK) == LOW)
            {
                if (tbuf.size() == 0)
                    return timer_ns::TIMER_INVALID;
                break;
            }
            else if (c == 'R')  // cancel
            {
                return timer_ns::TIMER_INVALID;
            }
            else
            {
                tbuf.addChar(c);
            }
            display_ns::display.clearDisplay();
            display_ns::setSmallFont();
            display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
            display_ns::display.print("Enter time:");
            display_ns::setLargeFont();
            display_ns::display.setCursor(display_ns::TIMER_X, display_ns::TIMER_Y);

            char dbuf[10];
            tbuf.format(dbuf, sizeof(dbuf));
            display_ns::display.print(dbuf);
            display_ns::display.display();
        }
        int secs = tbuf.getSeconds();
        return timer_ns::createTimer(secs);
    }

// Redial (R): Cancel the current timer
// '#' or hook up: Create a timer
// 0-9: Switch to a running timer
    void keyDispatch()
    {
         char printbuf[64];
         char c = keypad_ns::getKeyPress();
         if (c == 0)
             return;

         // Cancel a timer
         if (c == 'R')
         {
             timer_ns::clearTimer(currentTimer_);
             currentTimer_ = timer_ns::nextTimer();
             buzzer_.endBuzzer();
             return;
         }

         if (currentTimer_ != timer_ns::TIMER_INVALID)
         {
            // We can't create or switch timers if the current
            // one has expired
            if (timer_ns::checkTimer(currentTimer_) <= 0)
                return;
         }

         // Create a new timer
         if (c == '#' || digitalRead(HOOK) == HIGH)
         {
             int t = setTimer();
             if (t != timer_ns::TIMER_INVALID)
                currentTimer_ = t;
             return;
         }

         // Switch to a timer
         if (c >= '0' && c <= '9')
         {
            int timerno = c - '0';
            if (timer_ns::isTimerRunning(timerno))
            {
                currentTimer_ = timerno;
            }
            return;
         }
    }

    void switchToExpiredTimer()
    {
         //if (currentTimer_ != timer_ns::TIMER_INVALID && timer_ns::checkTimer(currentTimer_) < 0)
         //{
             //return;  // Current timer is expired
         //}

         // Switch current timer if one has expired
         for (int t = 0; t < timer_ns::MAX_TIMERS; t++)
         {
             int timeRemaining = timer_ns::checkTimer(t);
             if (timeRemaining != timer_ns::TIMER_INVALID && timeRemaining < 0)
             {
                 currentTimer_ = t;
                 buzzer_.startBuzzer();
                 return;
             }
         }
    }

    void showCurrentTimer()
    {
         if (currentTimer_ != timer_ns::TIMER_INVALID)
         {
             int timeRemaining = timer_ns::checkTimer(currentTimer_);
             if (timeRemaining < 0)
                 display_ns::showTimerExpired(currentTimer_, timeRemaining);
             else
                 display_ns::showTimerRunning(currentTimer_, timeRemaining);
         }
         else
         {
            displayNoTimers();
         }
    }

    void loop()
    {
        switchToExpiredTimer();
        keyDispatch();
        showCurrentTimer();
        buzzer_.loop();
    }

    void setup()
    {
        display_ns::display.clearDisplay();
        displayNoTimers();
        buzzer_.setup();
    }

    int currentTimer_;
    Buzzer buzzer_;
};

Application *pApp = new TimerApp();


void setup() 
{
  Serial.begin(9600);

  pinMode(ROW_1, INPUT);
  pinMode(ROW_2, INPUT);
  pinMode(ROW_3, INPUT);
  pinMode(ROW_4, INPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SER_DATA, OUTPUT);
  pinMode(SRCLR, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(AUDIO_CS, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  display_ns::setup();
  timer_ns::setup();
  keypad_ns::setup();
  audio_ns::setup();

  pApp->setup();
}

    
void loop() {
    pApp->loop();
}
