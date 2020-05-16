#ifndef TIMER_APP_H_
#define TIMER_APP_H_

#include "application.h"
#include "message_queue.h"

const int KEY_EVENT = USER_EVENT_BASE;

struct AppMessageQueue : MessageQueue
{
    void OnGenerator();
};

struct TimerApp : public Application
{
    TimerApp();

    // Event handlers
    int OnKey(int arg);
    int OnTimerEvent(int id);
    int OnIdleEvent();
    int OnUnknown(int msg, int arg1, int arg2);
    int OnCheckForExpiredTimers();
    int OnDrawTimer();
    int OnBuzzerTimer();
    int OnBuzzerStateChange(int value);

    // Functions
    int CreateNewTimer(char c);
    void CancelTimer(int timerno);
    void SwitchToTimer(int timerno);
    void PlayMessage(int timerno);
    void StopPlayingMessage(int timerno);
    void StartRecording(int timerno);
    void StopRecording(int timerno);
    void OnSwitchHookUp();
    void OnSwitchHookDown();
    void TimerExpired(int t);


    // Read hardware inputs
    bool read_keypad(char& c);

    void read_keypad_ungetc(char c);

    // Debug helper
    void printMessage(char *text, int msg, int arg1, int arg2);

    void loop();
    void setup(bool debug);
    int inputTime(char c);

// data
    int currentTimer_;      // The current timerno being displayed
    bool buzzerRunning_;    // Buzzer is running. This means BUZZER timer event is running
    int recordingTimer_;    // The timer being recorded
    int playingTimer_;      // The timer being played
    char ungetc_;           
    AppMessageQueue messageQueue_;
    bool debug_;
};

#endif

