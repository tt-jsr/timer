#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "timer_buffer.h"
#include "timer_app.h"
#include "test.h"
#include "timer_app.h"

TimerApp::TimerApp()
:currentTimer_(timer_ns::TIMER_INVALID)
,buzzerOn_(false)
,buzzerRunning_(false)
,hookUp_(false)
,recordingTimer_(timer_ns::TIMER_INVALID)
,playingTimer_(timer_ns::TIMER_INVALID)
,ungetc_(0)
{
}

int TimerApp::inputTime(char c)
{
    TimerBuffer tbuf;

    display_ns::print(
            display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES, 
            0, 0, 
            "Enter time:");

    if (c)
    {
        read_keypad_ungetc(c);
    }
    while(true)
    {
        int h = readSwitchHook();
        if (h < 0)  // switchhook is down
            return tbuf.getSeconds();

        char c;
        if (read_keypad(c))
        {
            if (c == '#')
                return tbuf.getSeconds();
            else if (c == 'R')
                return 0;
            else
            {
                tbuf.addChar(c);
            }
        }
        display_ns::print(
            display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_LINES
            , 0, 0
            , "Enter time:");


        char dbuf[10];
        tbuf.format(dbuf, sizeof(dbuf));

        display_ns::print(
            display_ns::FLAG_LARGE_FONT | display_ns::FLAG_DISPLAY
            , display_ns::TIMER_X, display_ns::TIMER_Y 
            , dbuf);

    }
}

static const int BUZZER_TIMER = 1;
static const int DRAW_TIMER = 2;
static const int CHECK_TIMERS = 3;

int TimerApp::CreateNewTimer(char character)
{
    int secs = inputTime(character);
    if (secs == 0)
        return timer_ns::TIMER_INVALID;
    int timerno = timer_ns::createTimer(secs);
    if (timerno == timer_ns::TIMER_INVALID)
        return timer_ns::TIMER_INVALID;
    if (hookUp_)
        StartRecording(timerno);
    else
    {
        timer_ns::startTimer(timerno);
        currentTimer_ = timerno;
    }
    return timerno;
}

void TimerApp::CancelTimer(int timerno)
{
    timer_ns::clearTimer(timerno);
    currentTimer_ = timer_ns::nextRunningTimer();
    noTone(BUZZER);
    messageQueue_.cancel_timer(BUZZER_TIMER);
    buzzerRunning_ = false;
}

void TimerApp::TimerExpired(int timerno)
{
    if (!buzzerRunning_)
    {
        messageQueue_.create_timer(BUZZER_TIMER, 500, true);
        tone(BUZZER, 1000);
        buzzerOn_ = true;
        buzzerRunning_ = true;
    }
}

void TimerApp::SwitchToTimer(int timerno)
{
    if (timer_ns::isTimerRunning(timerno))
    {
        currentTimer_ = timerno;
    }
}

void TimerApp::DrawTimer()
{
    if (recordingTimer_ != timer_ns::TIMER_INVALID) // We are not going to paint the screen while recording
        return;
    if (playingTimer_ != timer_ns::TIMER_INVALID) // We are not going to paint the screen while playing
        return;
    // If our current timer is invalid, get the oldest expired timer
    if (currentTimer_ == timer_ns::TIMER_INVALID)
        currentTimer_ = timer_ns::nextExpiredTimer();

    // If our current timer is still invalid, get the next timer to expire
    if (currentTimer_ == timer_ns::TIMER_INVALID)
        currentTimer_ = timer_ns::nextRunningTimer();

    if (currentTimer_ != timer_ns::TIMER_INVALID)
    {
        int timeRemaining = timer_ns::timeRemaining(currentTimer_);
        if (timeRemaining < 0)
            display_ns::showTimerExpired(currentTimer_, timeRemaining);
        else
            display_ns::showTimerRunning(currentTimer_, timeRemaining);
    }
    else
    {
        display_ns::showNoTimers();
    }
}

// DIGITAL_READ_EVENT
int TimerApp::OnDigitalRead(int pin, int state)
{
    if (pin == HOOK)
    {
        if (state == LOW)
            OnSwitchHookUp();
        else
            OnSwitchHookDown();
    }
    return 0;
}

void TimerApp::OnSwitchHookUp()
{
    if (timer_ns::isTimerExpired(currentTimer_))
    {
        int t = currentTimer_;
        CancelTimer(currentTimer_);
        PlayMessage(t);
    }
    else
    {  
        CreateNewTimer(0);
    }
}

void TimerApp::OnSwitchHookDown()
{
    if (recordingTimer_ != timer_ns::TIMER_INVALID)
    {
        StopRecording(recordingTimer_);
        recordingTimer_ = timer_ns::TIMER_INVALID;
    }

    if (playingTimer_ != timer_ns::TIMER_INVALID)
    {
        StopPlayingMessage(playingTimer_);
        playingTimer_ = timer_ns::TIMER_INVALID;
    }
}

int TimerApp::OnKey(int character)
{
    if (character == 'R')
    {
        CancelTimer(currentTimer_);
    }
    if (character == '#')
    {
        if (timer_ns::isTimerExpired(currentTimer_))
            CancelTimer(currentTimer_);
        else
            CreateNewTimer(0);
    }
    if (character >= '0' && character <= '9')
    {
        if (currentTimer_ == timer_ns::TIMER_INVALID)
        {
            CreateNewTimer(character);
        }
        else
            SwitchToTimer(character - '0');
    }

    return 0;
}

void TimerApp::PlayMessage(int timerno)
{
    playingTimer_ = timerno;
    display_ns::print(
        display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES
        , 0, 0
        , "Playing\n   message...");
    audio_ns::startPlaying(timerno);
}

void TimerApp::StopPlayingMessage(int timerno)
{
    audio_ns::stopPlaying(timerno);
    audio_ns::eraseRecording(timerno);
}

void TimerApp::StartRecording(int timerno)
{
    recordingTimer_ = timerno; 
                            // STOP_RECORDING will then start the timer
    display_ns::print(
        display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES
        , 0, 0
        , "Record\n   message...");

    audio_ns::startRecording(timerno);
}

void TimerApp::StopRecording(int timerno)
{
    audio_ns::stopRecording(timerno);
    if (timer_ns::startTimer(timerno))
    {
        currentTimer_ = timerno;
    }
    else
        timer_ns::clearTimer(timerno);
}

void TimerApp::CheckForExpiredTimers()
{
    int t = timer_ns::nextExpiredTimer();
    if (t != timer_ns::TIMER_INVALID)
    {
        SwitchToTimer(t);
        TimerExpired(t);
    }
}

// TIMER_EVENT
// This is an event timer, not a application timer
// See TimerExpired for application timers
int TimerApp::OnTimerEvent(int event_timerno)
{
    if (event_timerno == BUZZER_TIMER)
    {
        if (buzzerOn_)
            noTone(BUZZER);
        else
            tone(BUZZER, 1000);
        buzzerOn_ = !buzzerOn_;
    }

    if (event_timerno == DRAW_TIMER)
    {
        DrawTimer();
    }

    if (event_timerno == CHECK_TIMERS)
    {
        CheckForExpiredTimers();
    }

    return 0;
}

// IDLE_EVENT
int TimerApp::OnIdleEvent()
{
    char c;
    // Do we have any input to deal with?
    if (read_keypad(c))
    {
        OnKey(c);
    }
}

int TimerApp::OnUnknown(int msg, int arg)
{
    Serial.print("message_proc: unknown msg: ");
    Serial.print(msg);
    Serial.print(" arg: ");
    Serial.print(arg);

    return 0;
}

int TimerApp::readSwitchHook()
{
     bool b = readSwitchHookImpl(hookUp_);
     if (!hookUp_ && b)
     {
         hookUp_= true;
         return 1;
     }
     if (hookUp_ && !b)
     {
         hookUp_= false;
         return -1;
     }
     return 0;
}

bool TimerApp::readSwitchHookImpl(bool hookUp)
{
     bool state = digitalRead(HOOK) == LOW;
     if (hookUp != state)
     {
        delay(1);
        bool r = digitalRead(HOOK) == LOW;
        return r;
     }
     return hookUp;
}

// Read a keypress
bool TimerApp::read_keypad(char& c)
{
     if (ungetc_)
     {
        c = ungetc_;
        ungetc_= 0;
        return true;
     }
     c = keypad_ns::getKeyPress();
     if (c)
     {
         return true;
     }
     return false;
}

void TimerApp::read_keypad_ungetc(char c)
{
    ungetc_ = c;
}

void TimerApp::loop()
{
    // Pump a message into our message_proc 
    messageQueue_.pump_message();

}

extern Application *pApp;

int event_handler(int msg, int arg1, int arg2)
{
    TimerApp *pTimerApp = (TimerApp *)pApp;
    //pTimerApp->printMessage("message_proc", msg, arg);
    switch (msg)
    {
    case TIMER_EVENT:
        return pTimerApp->OnTimerEvent(arg1);
    case IDLE_EVENT:
        return pTimerApp->OnIdleEvent();
    case DIGITAL_READ_EVENT:
        return pTimerApp->OnDigitalRead(arg1, arg2);
    default:
        return pTimerApp->OnUnknown(msg, arg1);
    }
}

void TimerApp::printMessage(char *text, int msg, int arg)
{
    char buf[128];
    switch (msg)
    {
    case NULL_EVENT:
        sprintf(buf, "%s: msg: NULL_EVENT, arg: %d", text, arg);
        break;
    case TIMER_EVENT:
        sprintf(buf, "%s: msg: TIMER_EVENT, arg: %d", text, arg);
        break;
    case IDLE_EVENT:
        sprintf(buf, "%s: msg: IDLE_EVENT, arg: %d", text, arg);
        break;
    case DIGITAL_READ_EVENT:
        sprintf(buf, "%s: msg: DIGITAL_READ_EVENT, arg: %d", text, arg);
        break;
    default:
        sprintf(buf, "%s: msg: %d, arg: %d", text, msg, arg);
    }
    Serial.println(buf);
}

void TimerApp::setup()
{
    messageQueue_.register_event_handler(::event_handler);
    display_ns::display.clearDisplay();
    display_ns::showNoTimers();
    hookUp_ = false;
    messageQueue_.digitalRead(HOOK, HIGH, 500);
    messageQueue_.create_timer(DRAW_TIMER, 500, true);
    messageQueue_.create_timer(CHECK_TIMERS, 500, true);
}

