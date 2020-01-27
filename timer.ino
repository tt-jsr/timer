#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "test.h"

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
        char buf[10];
        memset(buf, 0, sizeof(buf));
        display_ns::display.clearDisplay();
        display_ns::setSmallFont();
        display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
        display_ns::display.print("Enter time:");
        display_ns::display.display();
        for(int idx = 0; idx < 10; ++idx)
        {
            char c = keypad_ns::getKey();
            if (c == '#')
            {
                if (buf[0] == '\0')
                    return;
                break;
            }
            else if (c == 'R')
            {
                memset(buf, 0, sizeof(buf));
                idx = -1;
            }
            else
            {
                buf[idx] = c;
            }
            display_ns::display.clearDisplay();
            display_ns::setSmallFont();
            display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
            display_ns::display.print("Enter time:");
            display_ns::setLargeFont();
            display_ns::display.setCursor(display_ns::TIMER_X, display_ns::TIMER_Y);
            display_ns::display.print(buf);
            display_ns::display.display();
        }
        int secs = atoi(buf);
        return timer_ns::createTimer(secs);
    }

    void loop()
    {
         char printbuf[64];
         char c = keypad_ns::getKeyPress();
         if (c == '#')
         {
             currentTimer_ = setTimer();
             sprintf(printbuf, "created timer: %d", currentTimer_);
             Serial.println(printbuf);
         }
         if (c == 'R')
         {
             timer_ns::clearTimer(currentTimer_);
             currentTimer_ = timer_ns::nextTimer();
         }
         if (c >= '0' && c <= '9')
         {
            int timerno = c - '0';
            if (timer_ns::isTimerRunning(timerno))
            {
                sprintf(printbuf, "set current timer: %d", timerno);
                Serial.println(printbuf);
                currentTimer_ = timerno;
            }
         }
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

    void setup()
    {
        display_ns::display.clearDisplay();
        displayNoTimers();
    }

    int currentTimer_;
};

Application *pApp = new TimerApp();


void setup() {
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

  display_ns::setup();
  timer_ns::setup();
  keypad_ns::setup();
  audio_ns::setup();

  pApp->setup();
}

    
void loop() {
    pApp->loop();
}
