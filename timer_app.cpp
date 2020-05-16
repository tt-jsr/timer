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
,buzzerRunning_(false)
,recordingTimer_(timer_ns::TIMER_INVALID)
,playingTimer_(timer_ns::TIMER_INVALID)
,ungetc_(0)
,debug_(false)
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
        int msg, arg1, arg2;
        messageQueue_.get_message(msg, arg1, arg2);
        //printMessage("inputTime", msg, arg1, arg2);
        switch (msg)
        {
        case VALUE_EVENT:
            if (arg1 == HOOK_STATE && arg2 == SWITCH_HOOK_DOWN)
                return tbuf.getSeconds();
            break;
        case KEY_EVENT:
            if (arg1 == '#')
                return tbuf.getSeconds();
            else if (arg1 == 'R')
                return 0;
            else
            {
                tbuf.addChar(arg1);
            }
            break;
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

int TimerApp::CreateNewTimer(char character)
{
    int secs = inputTime(character);
    if (secs == 0)
        return timer_ns::TIMER_INVALID;
    int timerno = timer_ns::createTimer(secs);
    if (timerno == timer_ns::TIMER_INVALID)
        return timer_ns::TIMER_INVALID;
    if (messageQueue_.get_value(HOOK_STATE) == SWITCH_HOOK_UP)
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
    messageQueue_.cancel_timer(BUZZER_TIMER);
    messageQueue_.set_value(BUZZER_STATE, TURN_BUZZER_OFF);
    buzzerRunning_ = false;
}

void TimerApp::TimerExpired(int timerno)
{
    if (!buzzerRunning_)
    {
        messageQueue_.create_timer(BUZZER_TIMER, 500, true);
        messageQueue_.set_value(BUZZER_STATE, TURN_BUZZER_ON);
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

// DRAW_TIMER timer handler
int TimerApp::OnDrawTimer()
{
    if (recordingTimer_ != timer_ns::TIMER_INVALID) // We are not going to paint the screen while recording
        return 0;
    if (playingTimer_ != timer_ns::TIMER_INVALID) // We are not going to paint the screen while playing
        return 0;
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

    return 0;
}

// SWITCH_HOOK_EVENT handler
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

// SWITCH_HOOK_EVENT handler
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

// CHECK_TIMERS timer handler
int TimerApp::OnCheckForExpiredTimers()
{
    int t = timer_ns::nextExpiredTimer();
    if (t != timer_ns::TIMER_INVALID)
    {
        SwitchToTimer(t);
        TimerExpired(t);
    }
    return 0;
}

int TimerApp::OnBuzzerStateChange(int value)
{
    if (value == TURN_BUZZER_OFF)
        noTone(PIN_BUZZER);
    else
        tone(PIN_BUZZER, 1000);
}


int TimerApp::OnBuzzerTimer()
{
    // Will cause OnBuzzerStateChange to be called
    messageQueue_.toggle_value(BUZZER_STATE);
    return 0;
}

void TimerApp::OnUnknown(char *p, int arg1, int arg2)
{
    Serial.print("unknown msg: ");
    Serial.print(p);
    Serial.print(" arg1: ");
    Serial.print(arg1);
    Serial.print(" arg2: ");
    Serial.println(arg2);
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

void TimerApp::OnTimerEvent(int id)
{
    if (id == BUZZER_TIMER)
        OnBuzzerTimer();
    else if (id == DRAW_TIMER)
        OnDrawTimer();
    else if (id == CHECK_TIMERS)
        OnCheckForExpiredTimers();
    else
        OnUnknown("OnTimerEvent", id, 0);
}

void TimerApp::OnKeyEvent(char character)
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
}

void TimerApp::OnValueEvent(int id, int value)
{
    switch (id)
    {
        case BUZZER_STATE:
            OnBuzzerStateChange(value);
            break;
        case HOOK_STATE:
            if (value == SWITCH_HOOK_UP)
                OnSwitchHookUp();
            if (value == SWITCH_HOOK_DOWN)
                OnSwitchHookDown();
            break;
        default:
            OnUnknown("OnValueEvent", id, value);
            break;
    }
}

void TimerApp::OnIdleEvent()
{
}

void TimerApp::loop()
{
    // Process a message from the queue
    int msg, arg1, arg2;
    messageQueue_.get_message(msg, arg1, arg2);

    //printMessage("loop", msg, arg1, arg2);
    switch (msg)
    {
    case KEY_EVENT:
        OnKeyEvent((char)arg1);
        break;
    case TIMER_EVENT:
        OnTimerEvent(arg1);
        break;
    case IDLE_EVENT:
        OnIdleEvent();
        break;
    case VALUE_EVENT:
        OnValueEvent(arg1, arg2);
        break;
    default:
        {
            char buf[32];
            sprintf (buf, "%d", msg);
            OnUnknown(buf, arg1, arg2);
        }
        break;
    }

}

void TimerApp::printMessage(char *text, int msg, int arg1, int arg2)
{
    char buf[128];
    buf[0] = '\0';
    switch (msg)
    {
    case TIMER_EVENT:
        //sprintf(buf, "%s: msg: TIMER_EVENT, id: %d", text, arg1);
        break;
    case IDLE_EVENT:
        //sprintf(buf, "%s: msg: IDLE_EVENT, arg: %d", text, arg1);
        break;
    case VALUE_EVENT:
        sprintf(buf, "%s: msg: VALUE_EVENT, id: %d value: %d", text, arg1, arg2);
        break;
    case KEY_EVENT:
        sprintf(buf, "%s: msg: KEY_EVENT, char: %c", text, arg1);
        break;
    default:
        sprintf(buf, "%s: msg: %d, arg1: %d, arg2: %d", text, msg, arg1, arg2);
    }
    if (buf[0])
        Serial.println(buf);
}

void TimerApp::setup(bool debug)
{
    debug_ = debug;
    messageQueue_.setDebug(true);
    display_ns::display.clearDisplay();
    display_ns::showNoTimers();
    messageQueue_.digitalRead(HOOK_STATE, PIN_HOOK, HIGH, 1000);
    messageQueue_.create_timer(DRAW_TIMER, 500, true);
    messageQueue_.create_timer(CHECK_TIMERS, 500, true);
    messageQueue_.create_value(BUZZER_STATE, TURN_BUZZER_OFF);
}

/********************************************************************/

extern Application *pApp;

void AppMessageQueue::OnGenerator()
{
    TimerApp *pTimerApp = (TimerApp *)pApp;
    char c;
    // Do we have any input to deal with?
    if (pTimerApp->read_keypad(c))
    {
        post_message(KEY_EVENT, c, 0);
    }
}
