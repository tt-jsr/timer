#include "timer.h"
#include "timer_app.h"

Application *pApp = new TimerApp();

void setup() 
{
  Serial.begin(9600);

  pinMode(PIN_ROW_1, INPUT);
  pinMode(PIN_ROW_2, INPUT);
  pinMode(PIN_ROW_3, INPUT);
  pinMode(PIN_ROW_4, INPUT);
  pinMode(PIN_SRCLK, OUTPUT);
  pinMode(PIN_SER_DATA, OUTPUT);
  pinMode(PIN_SRCLR, OUTPUT);
  pinMode(PIN_RCLK, OUTPUT);
  pinMode(PIN_AUDIO_CS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_HOOK, INPUT_PULLUP);

  display_ns::setup();
  timer_ns::setup();
  keypad_ns::setup();
  audio_ns::setup();

  pApp->setup();
}

    
void loop() {
    pApp->loop();
}
