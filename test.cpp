#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "test.h"

void TimerTest::setup()
{
  display_ns::setSmallFont();

  timerno_ = timer_ns::createTimer(1*60+13);
  if (timerno_ < 0)
  {
      display_ns::display.print("error");
      display_ns::display.display();
      for(;;);
  }
}

void TimerTest::loop()
{
 int timeRemaining = timer_ns::checkTimer(timerno_);
 if (timeRemaining < 0)
     display_ns::showTimerExpired(timerno_, timeRemaining);
 else
     display_ns::showTimerRunning(timerno_, timeRemaining);
  delay(1000);
}

/*****************************************/

void KeypadTest::setup()
{
    display_ns::setSmallFont();
    nchar = 0;
    display_ns::display.display();
}

void KeypadTest::loop()
{
    keypad_ns::KeyCode k = keypad_ns::getKey();
    if (k == keypad_ns::KEY_NONE)
        return;
    char c = keypad_ns::keycode2char(k);
    display_ns::display.print(c);
    display_ns::display.display();
    ++nchar;
    if (nchar > 20)
    {
        display_ns::clearDisplay();
        nchar = 0;
    }
}

/*****************************************/

void TimerTest2::setup()
{

}

void TimerTest2::loop()
{
    keypad_ns::KeyCode k = keypad_ns::getKey();
    if (k == keypad_ns::KEY_NONE)
        return;

}

