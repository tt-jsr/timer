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

// EVENT_CREATE_NEW_TIMER
int TimerApp::OnCreateNewTimer(int character)
{
    int secs = inputTime((char)character);
    if (secs == 0)
        return 0;
    int timerno = timer_ns::createTimer(secs);
    if (timerno == timer_ns::TIMER_INVALID)
        return;
    if (hookUp_)
        messageQueue_.post_message(EVENT_START_RECORDING, timerno);
    else
    {
        timer_ns::startTimer(timerno);
        currentTimer_ = timerno;
    }

    return 0;
}

// EVENT_CANCEL_TIMER
int TimerApp::OnCancelTimer(int timerno)
{
    timer_ns::clearTimer(timerno);
    currentTimer_ = timer_ns::nextRunningTimer();
    noTone(BUZZER);
    messageQueue_.cancel_timer(BUZZER_TIMER);
    buzzerRunning_ = false;

    return 0;
}

// EVENT_TIMER_EXPIRED
int TimerApp::OnTimerExpired(int timerno)
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
int TimerApp::OnSwitchToTimer(int timerno)
{
    if (timer_ns::isTimerRunning(timerno))
    {
        currentTimer_ = timerno;
    }

    return 0;
}

// EVENT_DRAW_TIMER
int TimerApp::OnDrawTimer(int unused)
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

    return 0;
}

// DIGITAL_READ_EVENT
int TimerApp::OnDigitalRead(int arg)
{
    int pin = message_queue_pin(arg);
    int state = message_queue_state(arg);
    if (pin == HOOK)
    {
        if (state == LOW)
            OnSwitchHookUp(0);
        else
            OnSwitchHookDown(0);
    }
}

// EVENT_SWITCH_HOOK_UP
int TimerApp::OnSwitchHookUp(int unused)
{
    if (timer_ns::isTimerExpired(currentTimer_))
    {
        messageQueue_.post_message(EVENT_CANCEL_TIMER, currentTimer_);
        messageQueue_.post_message(EVENT_PLAY_MESSAGE, currentTimer_);
    }
    else
    {  
        messageQueue_.post_message(EVENT_CREATE_NEW_TIMER, 0);
    }

    return 0;
}

// EVENT_SWITCH_HOOK_DOWN
int TimerApp::OnSwitchHookDown(int unused)
{
    if (recordingTimer_ != timer_ns::TIMER_INVALID)
    {
        messageQueue_.post_message(EVENT_STOP_RECORDING, recordingTimer_);
        recordingTimer_ = timer_ns::TIMER_INVALID;
    }

    if (playingTimer_ != timer_ns::TIMER_INVALID)
    {
        messageQueue_.post_message(EVENT_STOP_PLAYING, playingTimer_);
        playingTimer_ = timer_ns::TIMER_INVALID;
    }

    return 0;
}

// EVENT_MSG_KEY
int TimerApp::OnKey(int character)
{
    if (character == 'R')
    {
        messageQueue_.post_message(EVENT_CANCEL_TIMER, currentTimer_);
    }
    if (character == '#')
    {
        if (timer_ns::isTimerExpired(currentTimer_))
            messageQueue_.post_message(EVENT_CANCEL_TIMER, currentTimer_);
        else
            messageQueue_.post_message(EVENT_CREATE_NEW_TIMER, 0);
    }
    if (character >= '0' && character <= '9')
    {
        if (currentTimer_ == timer_ns::TIMER_INVALID)
        {
            messageQueue_.post_message(EVENT_CREATE_NEW_TIMER, character);
        }
        else
            messageQueue_.post_message(EVENT_SWITCH_TO_TIMER, character - '0');
    }

    return 0;
}

// EVENT_PLAY_MESSAGE
int TimerApp::OnPlayMessage(int timerno)
{
    playingTimer_ = timerno;
    display_ns::print(
        display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES
        , 0, 0
        , "Playing\n   message...");
    audio_ns::startPlaying(timerno);
    return 0;
}

// EVENT_STOP_PLAYING
int TimerApp::OnStopPlayingMessage(int timerno)
{
    audio_ns::stopPlaying(timerno);
    audio_ns::eraseRecording(timerno);
    return 0;
}

// EVENT_START_RECORDING
int TimerApp::OnStartRecording(int timerno)
{
    recordingTimer_ = timerno; 
                            // STOP_RECORDING will then start the timer
    display_ns::print(
        display_ns::FLAG_CLEAR | display_ns::FLAG_SMALL_FONT | display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES
        , 0, 0
        , "Record\n   message...");

    audio_ns::startRecording(timerno);
    return 0;
}

// EVENT_STOP_RECORDING
int TimerApp::OnStopRecording(int timerno)
{
    audio_ns::stopRecording(timerno);
    if (timer_ns::startTimer(timerno))
    {
        currentTimer_ = timerno;
    }
    else
        timer_ns::clearTimer(timerno);

    return 0;
}

// EVENT_CHECK_FOR_EXPIRED_TIMERS
int TimerApp::OnCheckForExpiredTimers(int unused)
{
    int t = timer_ns::nextExpiredTimer();
    if (t != timer_ns::TIMER_INVALID)
    {
        messageQueue_.post_message(EVENT_SWITCH_TO_TIMER, t);
        messageQueue_.post_message(EVENT_TIMER_EXPIRED, t);
    }

    return 0;
}

// TIMER_EVENT
// This is an event timer, not a application timer
// See OnTimerExpired for application timers
int TimerApp::OnTimerEvent(int queue_timerno)
{
    if (queue_timerno == BUZZER_TIMER)
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
int TimerApp::OnIdleEvent()
{
    // If the queue is empty and it is time, we will
    // draw the current timer and check for expired timers
    if (drawTimer_.check())
    {
        messageQueue_.post_message(EVENT_DRAW_TIMER, 0);
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


void TimerApp::printMessage(char *text, int msg, int arg)
{
    char buf[128];
    switch (msg)
    {
    case NULL_EVENT:
        sprintf(buf, "%s: msg: NULL_EVENT, arg: %d", text, arg);
        break;
    case EVENT_CREATE_NEW_TIMER:
        sprintf(buf, "%s: msg: CREATE_NEW_TIMER, arg: %d", text, arg);
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

// Read from the hardware, keypress or switch hook
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
    char c;
    // Do we have any input to deal with?
    if (read_keypad(c))
    {
        //printMessage("read_keypad", msg, arg);
        messageQueue_.post_message(EVENT_MSG_KEY, (int)c);
    }

    // Pump a message into our message_proc 
    messageQueue_.pump_message();

}

extern Application *pApp;

int message_proc(int msg, int arg)
{
    TimerApp *pTimerApp = (TimerApp *)pApp;
    //pTimerApp->printMessage("message_proc", msg, arg);
    switch (msg)
    {
    case EVENT_CREATE_NEW_TIMER:
        return pTimerApp->OnCreateNewTimer(arg);
    case EVENT_CANCEL_TIMER:
        return pTimerApp->OnCancelTimer(arg);
    case EVENT_TIMER_EXPIRED:
        return pTimerApp->OnTimerExpired(arg);
    case EVENT_SWITCH_TO_TIMER:
        return pTimerApp->OnSwitchToTimer(arg);
    case EVENT_DRAW_TIMER:
        return pTimerApp->OnDrawTimer(arg);
    case EVENT_SWITCH_HOOK_UP:
        return pTimerApp->OnSwitchHookUp(arg);
    case EVENT_SWITCH_HOOK_DOWN:
        return pTimerApp->OnSwitchHookDown(arg);
    case EVENT_MSG_KEY:
        return pTimerApp->OnKey(arg);
    case EVENT_PLAY_MESSAGE:
        return pTimerApp->OnPlayMessage(arg);
    case EVENT_STOP_PLAYING:
        return pTimerApp->OnStopPlayingMessage(arg);
    case EVENT_START_RECORDING:
        return pTimerApp->OnStartRecording(arg);
    case EVENT_STOP_RECORDING:
        return pTimerApp->OnStopRecording(arg);
    case EVENT_CHECK_FOR_EXPIRED_TIMERS:
        return pTimerApp->OnCheckForExpiredTimers(arg);
    case TIMER_EVENT:
        return pTimerApp->OnTimerEvent(arg);
    case IDLE_EVENT:
        return pTimerApp->OnIdleEvent();
    case DIGITAL_READ_EVENT:
        return pTimerApp->OnDigitalRead(arg);
    default:
        return pTimerApp->OnUnknown(msg, arg);
    }
}
void TimerApp::setup()
{
    messageQueue_.register_proc(::message_proc);
    display_ns::display.clearDisplay();
    display_ns::showNoTimers();
    drawTimer_.set(500);
    hookUp_ = false;
    messageQueue_.digitalRead(HOOK, HIGH, 500);
}

