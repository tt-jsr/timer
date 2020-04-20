#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "timer_buffer.h"
#include "timer_app.h"
#include "test.h"
#include "timer_app.h"

#define GET_CURRENT_TIMER() currentTimer_
#define SET_CURRENT_TIMER(t) currentTimer_ = t

TimerApp::TimerApp()
:currentTimer_(timer_ns::TIMER_INVALID)
,buzzerOn_(false)
,buzzerRunning_(false)
,hookUp_(false)
,recording_(0)
{
}

int TimerApp::inputTime()
{
    TimerBuffer tbuf;

    display_ns::print(
            display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES, 
            0, 0, 
            "Enter time:");

    while(true)
    {
        int msg, arg;
        if (read_input(msg, arg))
        {
            switch (msg)
            {
            case EVENT_MSG_KEY:
                if (arg == '#')
                    return tbuf.getSeconds();
                else if (arg == 'R')
                    return 0;
                else
                    tbuf.addChar(arg);
                break;
            case EVENT_SWITCH_HOOK_DOWN:
                return tbuf.getSeconds();
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

// EVENT_CREATE_NEW_TIMER
int TimerApp::OnCreateNewTimer(int arg)
{
    int secs = inputTime();
    if (secs == 0)
        return 0;
    if (hookUp_)
        messageQueue_.post_message(EVENT_START_RECORDING, secs);
    else
    {
        int timerno = timer_ns::createTimer(secs);
        if (timerno != timer_ns::TIMER_INVALID)
        {
            SET_CURRENT_TIMER(timerno);
        }
    }

    return 0;
}

// EVENT_CANCEL_TIMER
int TimerApp::OnCancelTimer(int arg)
{
    timer_ns::clearTimer(arg);
    SET_CURRENT_TIMER(timer_ns::nextTimer());
    noTone(BUZZER);
    messageQueue_.cancel_timer(BUZZER_TIMER);
    buzzerRunning_ = false;

    return 0;
}

// EVENT_TIMER_EXPIRED
int TimerApp::OnTimerExpired(int arg)
{
    if (!buzzerRunning_)
    {
        messageQueue_.create_timer(BUZZER_TIMER, 500, true);
        tone(BUZZER, 1000);
        buzzerOn_ = true;
        buzzerRunning_ = true;
    }

    return 0;
}

// EVENT_SWITCH_TO_TIMER
int TimerApp::OnSwitchToTimer(int arg)
{
    if (timer_ns::isTimerRunning(arg))
    {
        SET_CURRENT_TIMER(arg);
    }

    return 0;
}

// EVENT_DRAW_TIMER
int TimerApp::OnDrawTimer(int arg)
{
    if (recording_) 
        return;
    // If our current timer is invalid, get the oldest expired timer
    if (GET_CURRENT_TIMER() == timer_ns::TIMER_INVALID)
        SET_CURRENT_TIMER(timer_ns::getExpiredTimer());

    // If our current timer is still invalid, get the next timer to expire
    if (GET_CURRENT_TIMER() == timer_ns::TIMER_INVALID)
        SET_CURRENT_TIMER(timer_ns::nextTimer());

    if (GET_CURRENT_TIMER() != timer_ns::TIMER_INVALID)
    {
        int timeRemaining = timer_ns::checkTimer(arg);
        if (timeRemaining < 0)
            display_ns::showTimerExpired(arg, timeRemaining);
        else
            display_ns::showTimerRunning(arg, timeRemaining);
    }
    else
    {
        display_ns::showNoTimers();
    }

    return 0;
}

// EVENT_SWITCH_HOOK_UP
int TimerApp::OnSwitchHookUp(int arg)
{
    if (timer_ns::isTimerExpired(GET_CURRENT_TIMER()))
    {
        messageQueue_.post_message(EVENT_CANCEL_TIMER, GET_CURRENT_TIMER());
        messageQueue_.post_message(EVENT_PLAY_MESSAGE, GET_CURRENT_TIMER());
    }
    else
    {  
        messageQueue_.post_message(EVENT_CREATE_NEW_TIMER, GET_CURRENT_TIMER());
    }

    return 0;
}

// EVENT_SWITCH_HOOK_DOWN
int TimerApp::OnSwitchHookDown(int arg)
{
    if (recording_)
        messageQueue_.post_message(EVENT_STOP_RECORDING, 0);

    return 0;
}

// EVENT_MSG_KEY
int TimerApp::OnKey(int arg)
{
    if (arg == 'R')
    {
        messageQueue_.post_message(EVENT_CANCEL_TIMER, GET_CURRENT_TIMER());
    }
    if (arg == '#')
    {
        if (timer_ns::isTimerExpired(GET_CURRENT_TIMER()))
            messageQueue_.post_message(EVENT_CANCEL_TIMER, GET_CURRENT_TIMER()/*, 0*/);
        else
            messageQueue_.post_message(EVENT_CREATE_NEW_TIMER, 0);
    }
    if (arg >= '0' && arg <= '9')
        messageQueue_.post_message(EVENT_SWITCH_TO_TIMER, arg - '0');

    return 0;
}

// EVENT_PLAY_MESSAGE
int TimerApp::OnPlayMessage(int arg)
{
    return 0;
}

// EVENT_START_RECORDING
int TimerApp::OnStartRecording(int arg)
{
    recording_ = arg;  // arg is the number of seconds for the timer
                       // STOP_RECORDING will then set the timer
    display_ns::print(
        display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES
        , 0, 0
        , "Recording\nmessage...");

    return 0;
}

// EVENT_STOP_RECORDING
int TimerApp::OnStopRecording(int arg)
{
    int timerno = timer_ns::createTimer(recording_);
    if (timerno != timer_ns::TIMER_INVALID)
    {
        SET_CURRENT_TIMER(timerno);
    }
    recording_ = 0;

    return 0;
}

// EVENT_CHECK_FOR_EXPIRED_TIMERS
int TimerApp::OnCheckForExpiredTimers(int arg)
{
    int t = timer_ns::getExpiredTimer();
    if (t != timer_ns::TIMER_INVALID)
    {
        messageQueue_.post_message(EVENT_SWITCH_TO_TIMER, t);
        messageQueue_.post_message(EVENT_TIMER_EXPIRED, t);
    }

    return 0;
}

// TIMER_EVENT
int TimerApp::OnTimerEvent(int arg)
{
    if (arg == BUZZER_TIMER)
    {
        if (buzzerOn_)
            noTone(BUZZER);
        else
            tone(BUZZER, 1000);
        buzzerOn_ = !buzzerOn_;
    }

    return 0;
}

// IDLE_EVENT
int TimerApp::OnIdleEvent(int arg)
{
    // If the queue is empty and it is time, we will
    // draw the current timer and check for expired timers
    if (drawTimer_.check())
    {
        messageQueue_.post_message(EVENT_DRAW_TIMER, GET_CURRENT_TIMER());
        messageQueue_.post_message(EVENT_CHECK_FOR_EXPIRED_TIMERS, 0);
    }

    return 0;
}

int TimerApp::OnUnknown(int msg, int arg)
{
    Serial.print("message_proc: unknown msg: ");
    Serial.print(msg);
    Serial.print(" arg: ");
    Serial.print(arg);

    return 0;
}

int TimerApp::message_proc(int msg, int arg)
{
    //printMessage("message_proc", msg, arg);
    switch (msg)
    {
    case EVENT_CREATE_NEW_TIMER:
        return OnCreateNewTimer(arg);
    case EVENT_CANCEL_TIMER:
        return OnCancelTimer(arg);
    case EVENT_TIMER_EXPIRED:
        return OnTimerExpired(arg);
    case EVENT_SWITCH_TO_TIMER:
        return OnSwitchToTimer(arg);
    case EVENT_DRAW_TIMER:
        return OnDrawTimer(arg);
    case EVENT_SWITCH_HOOK_UP:
        return OnSwitchHookUp(arg);
    case EVENT_SWITCH_HOOK_DOWN:
        return OnSwitchHookDown(arg);
    case EVENT_MSG_KEY:
        return OnKey(arg);
    case EVENT_PLAY_MESSAGE:
        return OnPlayMessage(arg);
    case EVENT_START_RECORDING:
        return OnStartRecording(arg);
    case EVENT_STOP_RECORDING:
        return OnStopRecording(arg);
    case EVENT_CHECK_FOR_EXPIRED_TIMERS:
        return OnCheckForExpiredTimers(arg);
    case TIMER_EVENT:
        return OnTimerEvent(arg);
    case IDLE_EVENT:
        return OnIdleEvent(arg);
    default:
        return OnUnknown(msg, arg);
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
    case EVENT_CREATE_NEW_TIMER:
        sprintf(buf, "%s: msg: CREATE_TIMER, arg: %d", text, arg);
        break;
    case EVENT_CANCEL_TIMER:
        sprintf(buf, "%s: msg: CANCEL_TIMER, arg: %d", text, arg);
        break;
    case EVENT_TIMER_EXPIRED:
        sprintf(buf, "%s: msg: TIMER_EXPIRED, arg: %d", text, arg);
        break;
    case EVENT_SWITCH_TO_TIMER:
        sprintf(buf, "%s: msg: SWITCH_TO_TIMER, arg: %d", text, arg);
        break;
    case EVENT_MSG_KEY:
        sprintf(buf, "%s: msg: MSG_KEY, arg: %c", text, (char)arg);
        break;
    case EVENT_SWITCH_HOOK_UP:
        sprintf(buf, "%s: msg: SWITCH_HOOK_UP, arg: %d", text, arg);
        break;
    case EVENT_SWITCH_HOOK_DOWN:
        sprintf(buf, "%s: msg: SWITCH_HOOK_DOWN, arg: %d", text, arg);
        break;
    case EVENT_PLAY_MESSAGE:
        sprintf(buf, "%s: msg: PLAY_MESSAGE, arg: %d", text, arg);
        break;
    case EVENT_START_RECORDING:
        sprintf(buf, "%s: msg: START_RECORDING, arg: %d", text, arg);
        break;
    case EVENT_DRAW_TIMER:
        return;
        sprintf(buf, "%s: msg: DRAW_TIMER, arg: %d", text, arg);
        break;
    case EVENT_CHECK_FOR_EXPIRED_TIMERS:
        return;
        sprintf(buf, "%s: msg: CHECK_FOR_EXPIRED_TIMERS, arg: %d", text, arg);
        break;
    case TIMER_EVENT:
        sprintf(buf, "%s: msg: TIMER_EVENT, arg: %d", text, arg);
        break;
    case IDLE_EVENT:
        sprintf(buf, "%s: msg: IDLE_EVENT, arg: %d", text, arg);
        break;
    default:
        sprintf(buf, "%s: msg: %d, arg: %d", text, msg, arg);
    }
    Serial.println(buf);
}

bool TimerApp::readSwitchHook(bool hookUp)
{
     bool state = digitalRead(HOOK) == LOW;
     if (hookUp != state)
     {
        delay(1000);
        bool r = digitalRead(HOOK) == LOW;
        return r;
     }
     return hookUp;
}

// Read from the hardware, keypress or switch hook
bool TimerApp::read_input(int& msg, int& arg)
{
     char c(0);

     bool b = readSwitchHook(hookUp_);
     if (!hookUp_ && b)
     {
         hookUp_= true;
         msg = EVENT_SWITCH_HOOK_UP;
         arg = 0;
         return true;
     }
     if (hookUp_ && !b)
     {
         hookUp_= false;
         msg = EVENT_SWITCH_HOOK_DOWN;
         arg = 0;
         return true;
     }

     c = keypad_ns::getKeyPress();
     if (c)
     {
         msg = EVENT_MSG_KEY;
         arg = c;
         return true;
     }
     msg = NULL_EVENT;
     arg = 0;
     return false;
}

void TimerApp::loop()
{
    int msg, arg;

    // Pump a message into our message_proc 
    messageQueue_.pump_message();

    // Do we have any input to deal with?
    if (read_input(msg, arg))
    {
        //printMessage("read_input", msg, arg);
        messageQueue_.post_message(msg, arg);
    }
}

extern Application *pApp;

int message_proc(int msg, int arg)
{
    return ((TimerApp *)pApp)->message_proc(msg, arg);
}

void TimerApp::setup()
{
    messageQueue_.register_proc(::message_proc);
    display_ns::display.clearDisplay();
    display_ns::showNoTimers();
    drawTimer_.set(500);
    hookUp_ = false;
}

