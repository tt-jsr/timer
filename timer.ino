#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

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

  setupDisplay();
  setupTimer();
  setupKeypad();
  setupAudio();

  int num = createTimer(1*60+13);
  if (num < 0)
  {
      display.print("error");
      display.display();
      for(;;);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  showTimer(0);
  delay(1000);
}
