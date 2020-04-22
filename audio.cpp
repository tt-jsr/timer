#include "Arduino.h"
#include "timer.h"

namespace audio_ns
{
    bool HasRecording[timer_ns::MAX_TIMERS];

    int startRecording(int timerno)
    {
        Serial.print("startRecording for timer: ");
        Serial.println(timerno);
        return -1;
    }

    void stopRecording(int timerno)
    {
        Serial.print("stopRecording for timer: ");
        Serial.println(timerno);
        HasRecording[timerno] = true;
    }

    void startPlaying(int timerno)
    {
        Serial.print("startPlaying for timer: ");
        Serial.println(timerno);
    }

    void stopPlaying(int timerno)
    {
        Serial.print("stopPlaying for timer: ");
        Serial.println(timerno);
    }

    void eraseRecording(int timerno)
    {
        Serial.print("eraseRecording for timer: ");
        Serial.println(timerno);
        HasRecording[timerno] = false;
    }

    void playRingTone()
    {
        Serial.println("playRingTone");
    }

    bool isRecordingAvailable(int timerno)
    {
        if (timerno < 0 || timerno >= timer_ns::MAX_TIMERS)
            return false;
        return HasRecording[timerno];
    }

    void setup() 
    {
        for (int i = 0; i < timer_ns::MAX_TIMERS; i++)
        {
            HasRecording[i] = false;
        }
    }

}
