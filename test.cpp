#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

void TimerTest::setup()
{
  int num = timer_ns::createTimer(1*60+13);
  if (num < 0)
  {
      display_ns::display.print("error");
      display_ns::display.display();
      for(;;);
  }
}

void TimerTest::loop()
{
  for (int timerno = 0; timerno < timer_ns::MAX_TIMERS; ++timerno)
  {
     int timeRemaining = timer_ns::checkTimer(timerno);
     if (timeRemaining < 0)
         display_ns::showTimerExpired(timerno, timeRemaining);
     else
         display_ns::showTimerRunning(timerno, timeRemaining);
  }
  delay(1000);
}

/*****************************************/

void KeypadTest::setup()
{

}

void KeypadTest::loop()
{
    int k = keypad_ns::getKey();
    char c = keypad_ns::keycode2char(k);
    display_ns::display.print(c);
}

/*****************************************/

void TimerTest2::setup()
{

}

void TimerTest2::loop()
{

}

