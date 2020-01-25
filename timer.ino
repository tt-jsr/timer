#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "test.h"

Application *pApp = new KeypadTest();

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
