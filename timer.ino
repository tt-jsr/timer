#include "timer.h"
#include "timer_app.h"

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
  pinMode(HOOK, INPUT_PULLUP);

  display_ns::setup();
  timer_ns::setup();
  keypad_ns::setup();
  audio_ns::setup();

  pApp->setup();
}

    
void loop() {
    pApp->loop();
}
