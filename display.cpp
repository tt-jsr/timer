#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include "timer.h"

namespace display_ns
{
    int TEXT_HEIGHT = 0;
    int TIMER_X = 0;
    int TIMER_Y = 0;

    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void setLargeFont()
    {
      int x, y, w, h;

      display.setFont(&FreeSans18pt7b);
      display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
      TEXT_HEIGHT = h;
    }

    void setSmallFont()
    {
      int x, y, w, h;
      display.setFont(&FreeSans9pt7b);
      display.clearDisplay();
      display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
      TEXT_HEIGHT = h;
    }

    void showTimerExpired(int timerno, int secs)
    {
      char buf[64];

#if defined(MONITOR)
      snprintf(buf, sizeof(buf), "Timer %d = %d secs remaining\n", timerno, secs);
      Serial.print(buf);
#endif

      secs = -secs;
      int hours = secs/3600;
      secs = secs - hours*3600;
      int min = secs/60;
      secs = secs - min*60;

      setSmallFont();
      display.setCursor(0, TEXT_HEIGHT);     // Start at top-left corner
      snprintf(buf, sizeof(buf), "Timer %d Expired", timerno);
      display.print(buf);
      setLargeFont();
      snprintf(buf, sizeof(buf), "%d:%02d", min, secs);
      display.setCursor(TIMER_X, TIMER_Y);
      display.print(buf);
      display.display();
    }

    void showTimerRunning(int timerno, int secs)
    {
      char buf[64];

#if defined(MONITOR)
      snprintf(buf, sizeof(buf), "Timer %d = %d secs remaining\n", timerno, secs);
      Serial.print(buf);
#endif
      int hours = secs/3600;
      secs = secs - hours*3600;
      int min = secs/60;
      secs = secs - min*60;
      setSmallFont();
      display.setCursor(0, TEXT_HEIGHT);
      snprintf(buf, sizeof(buf), "Timer %d", timerno);
      display.print(buf);
      setLargeFont();
      if(hours)
        snprintf(buf, sizeof(buf), "%d:%02d:%02d", hours, min, secs);
      else
        snprintf(buf, sizeof(buf), "%d:%02d", min, secs);
      display.setCursor(TIMER_X, TIMER_Y);
      display.print(buf);
      display.display();
    }

    void showNoTimers()
    {
        display_ns::setSmallFont();
        int x, y, w, h;
        display_ns::display.getTextBounds("No timers", 0, 0, &x, &y, &w, &h);
        y = 64/2+h/2;
        x = 128/2-w/2;
        display_ns::display.setCursor(x, y);
        display_ns::display.print("No Timers");
        display_ns::display.display();
    }

    void clearDisplay()
    {
        display.clearDisplay();
        display.setCursor(0, TEXT_HEIGHT);
    }

    void setup() {
      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
      }

      display.clearDisplay();

      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.setRotation(0);
      display.setCursor(0, 0);     // Start at top-left corner
      display.cp437(true);

      setLargeFont();
      int x, y, w, h;
      display.getTextBounds("8:59:00", 0, 0, &x, &y, &w, &h);
      TIMER_Y = 64/2+h/2 + 10;
      TIMER_X = 128/2-w/2 - 5;

      setSmallFont();
    }
}
