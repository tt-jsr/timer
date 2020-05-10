#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
//#include "interval_timer.h"
#include "message_queue.h"

struct TimerApp : public Application
{
    TimerApp();

    // Event handlers
    int OnKey(int arg);
    int OnTimerEvent(int id);
    int OnDigitalRead(int pin, int val);
    int OnIdleEvent();
    int OnUnknown(int msg, int arg);

    // Functions
    int CreateNewTimer(char c);
    void CancelTimer(int timerno);
    void TimerExpired(int timerno);
    void SwitchToTimer(int timerno);
    void DrawTimer();
    void PlayMessage(int timerno);
    void StopPlayingMessage(int timerno);
    void StartRecording(int timerno);
    void StopRecording(int timerno);
    void CheckForExpiredTimers();
    void OnSwitchHookUp();
    void OnSwitchHookDown();


    // Read hardware inputs
    int readSwitchHook();
    bool readSwitchHookImpl(bool hookUp);
    bool read_keypad(char& c);

    void read_keypad_ungetc(char c);

    // Debug helper
    void printMessage(char *text, int msg, int arg);

    void loop();
    void setup();
    int inputTime(char c);

// data
    int currentTimer_;      // The current timerno being displayed
    bool buzzerOn_;         // Buzzer is currently on
    bool buzzerRunning_;    // Buzzer is running. This means BUZZER timer event is running
    bool hookUp_;           // The swicth hok is up
    int recordingTimer_;    // The timer being recorded
    int playingTimer_;      // The timer being played
    char ungetc_;           
    MessageQueue messageQueue_;
};

#endif

