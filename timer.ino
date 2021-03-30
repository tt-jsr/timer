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

  bool debug = false;
  timer_display_ns::setup(debug);
  timer_ns::setup(debug);
  keypad_ns::setup(false);
  audio_ns::setup(false);

  pApp->setup(debug);
}

    
void loop() {
    pApp->loop();
}
