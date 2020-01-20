#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

int TEXT_X = 0;
int TEXT_Y = 0;
int TIMER_X = 0;
int TIMER_Y = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void showTimerExpired(int timerno, int secs)
{
  char buf[64];

#if defined(MONITOR)
  snprintf(buf, sizeof(buf), "Timer %d = %d secs remaining\n", timerno, secs);
  Serial.print(buf);
#endif

  secs = -secs;
  int min = secs/60;
  secs = secs - min*60;

  display.setFont(&FreeSans9pt7b);
  display.clearDisplay();
  display.setCursor(TEXT_X, TEXT_Y);     // Start at top-left corner
  snprintf(buf, sizeof(buf), "Timer %d Expired", timerno);
  display.print(buf);
  display.setFont(&FreeSans24pt7b);
  snprintf(buf, sizeof(buf), "%02d:%02d", min, secs);
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
  int min = secs/60;
  secs = secs - min*60;
  display.setFont(&FreeSans9pt7b);
  display.clearDisplay();
  display.setCursor(TEXT_X, TEXT_Y);     // Start at top-left corner
  snprintf(buf, sizeof(buf), "Timer %d", timerno);
  display.print(buf);
  display.setFont(&FreeSans24pt7b);
  snprintf(buf, sizeof(buf), "%02d:%02d", min, secs);
  display.setCursor(TIMER_X, TIMER_Y);
  display.print(buf);
  display.display();
}

void showTimer(int timerno)
{
  int now = millis()/1000;
  int secs = timers[timerno]-now;
  if (secs < 0)
      showTimerExpired(timerno, secs);
  else
      showTimerRunning(timerno, secs);
}

void setupDisplay() {
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

  int x, y, w, h;
  display.setFont(&FreeSans9pt7b);
  display.clearDisplay();
  display.getTextBounds("Timer", 0, 0, &x, &y, &w, &h);
  TEXT_X = 0;
  TEXT_Y = h;
  display.setFont(&FreeSans24pt7b);
  display.getTextBounds("59:00", 0, 0, &x, &y, &w, &h);
  TIMER_Y = 64/2+h/2 + 10;
  TIMER_X = 128/2-w/2 - 5;
}

