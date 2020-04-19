#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"
#include "buzzer.h"
#include "timer_buffer.h"
#include "timer_app.h"
#include "test.h"
#include "timer_app.h"

#define GET_CURRENT_TIMER() currentTimer_
#define SET_CURRENT_TIMER(t) currentTimer_ = t

TimerApp::TimerApp()
:currentTimer_(timer_ns::TIMER_INVALID)
,hookUp_(false)
,consumer_(0)
,producer_(0)
{
    for (int i = 0; i < MAX_MESSAGES;++i)
    {
        msg_queue_[i].msg_type = MSG_NONE;
        msg_queue_[i].arg=0;
    }
}

void TimerApp::displayNoTimers()
{
    display_ns::setSmallFont();
    int x, y, w, h;
    display_ns::display.getTextBounds("No timers", 0, 0, &x, &y, &w, &h);
    y = 64/2+h/2;
    x = 128/2-w/2;
    display_ns::display.setCursor(x, y);
    display_ns::display.print("No Timers");
    display_ns::display.display();
}

int TimerApp::createTimer()
{
    TimerBuffer tbuf;
    display_ns::display.clearDisplay();
    display_ns::setSmallFont();
    display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
    display_ns::display.print("Enter time:");
    display_ns::display.display();

    Serial.println("enter createTimer");
    while(true)
    {
        bool starttimer(false);
        int msg, arg;
        if (read_input(msg, arg))
        {
            switch (msg)
            {
            case MSG_KEY:
                if (arg == '#')
                    starttimer = true;
                else if (arg == 'R')
                    return timer_ns::TIMER_INVALID;
                else
                    tbuf.addChar(arg);
                break;
            case SWITCH_HOOK_DOWN:
                starttimer = true;
                break;
            }
        }
        if (starttimer)
        {
            if (tbuf.size() == 0)
                return timer_ns::TIMER_INVALID;
            int secs = tbuf.getSeconds();
            return timer_ns::createTimer(secs);
        }
        display_ns::display.clearDisplay();
        display_ns::setSmallFont();
        display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
        display_ns::display.print("Enter time:");
        display_ns::setLargeFont();
        display_ns::display.setCursor(display_ns::TIMER_X, display_ns::TIMER_Y);

        char dbuf[10];
        tbuf.format(dbuf, sizeof(dbuf));
        display_ns::display.print(dbuf);
        display_ns::display.display();
    }
    Serial.println("exit createTimer");
}

bool TimerApp::readSwitchHook(bool hookUp)
{
     bool state = digitalRead(HOOK) == LOW;
     if (hookUp != state)
     {
        delay(1000);
        bool r = digitalRead(HOOK) == LOW;
        Serial.println("hook state: ");
        Serial.println(hookUp_);
        return r;
     }
     return hookUp;
}

void TimerApp::message_proc(int msg, int arg)
{
    //printMessage("message_proc", msg, arg);
    switch (msg)
    {
    case CREATE_NEW_TIMER:
        {
            int t = createTimer();
            if (t != timer_ns::TIMER_INVALID)
                SET_CURRENT_TIMER(t);
        }
        break;
    case CANCEL_TIMER:
        timer_ns::clearTimer(arg);
        SET_CURRENT_TIMER(timer_ns::nextTimer());
        buzzer_.endBuzzer();
        break;
    case TIMER_EXPIRED:
        // Current timer should be the expired one
        buzzer_.startBuzzer();
        break;
    case SWITCH_TO_TIMER:
        {
            int timerno = arg - '0';
            if (timer_ns::isTimerRunning(timerno))
            {
                SET_CURRENT_TIMER(timerno);
            }
        }
        break;
    case DRAW_TIMER:
        // If our current timer is invalid, get the oldest expired timer
        if (GET_CURRENT_TIMER() == timer_ns::TIMER_INVALID)
            SET_CURRENT_TIMER(timer_ns::getExpiredTimer());

        // If our current timer is still invalid, get the next timer to expire
        if (GET_CURRENT_TIMER() == timer_ns::TIMER_INVALID)
            SET_CURRENT_TIMER(timer_ns::nextTimer());

        if (GET_CURRENT_TIMER() != timer_ns::TIMER_INVALID)
        {
            Serial.println("Timer not invalid");
            int timeRemaining = timer_ns::checkTimer(arg);
            if (timeRemaining < 0)
                display_ns::showTimerExpired(arg, timeRemaining);
            else
                display_ns::showTimerRunning(arg, timeRemaining);
        }
        else
        {
            displayNoTimers();
        }
        break;
    case SWITCH_HOOK_UP:
        if (timer_ns::isTimerExpired(GET_CURRENT_TIMER()))
        {
            post_message(CANCEL_TIMER, GET_CURRENT_TIMER());
            post_message(PLAY_MESSAGE, GET_CURRENT_TIMER());
        }
        else
        {
            post_message(CREATE_NEW_TIMER, GET_CURRENT_TIMER());
            post_message(RECORD_MESSAGE, GET_CURRENT_TIMER());
        }
        break;
    case SWITCH_HOOK_DOWN:
        break;
    case MSG_KEY:
        if (arg == 'R')
        {
            if (timer_ns::isTimerExpired(GET_CURRENT_TIMER()))
                post_message(CANCEL_TIMER, GET_CURRENT_TIMER());
        }
        if (arg == '#')
        {
            if (timer_ns::isTimerExpired(GET_CURRENT_TIMER()))
                post_message(CANCEL_TIMER, GET_CURRENT_TIMER()/*, 0*/);
            else
                post_message(CREATE_NEW_TIMER, 0);
        }
        if (arg >= '0' && arg <= '9')
            post_message(SWITCH_TO_TIMER, arg);
        break;
    case PLAY_MESSAGE:
        break;
    case RECORD_MESSAGE:
        break;
    case CHECK_FOR_EXPIRED_TIMERS:
        {
            int t = timer_ns::getExpiredTimer();
            if (t != timer_ns::TIMER_INVALID)
            {
                post_message(SWITCH_TO_TIMER, t);
                post_message(TIMER_EXPIRED, t);
            }
        }
        break;
    default:
        Serial.print("message_proc: unknown msg: ");
        Serial.print(msg);
        Serial.print(" arg: ");
        Serial.print(arg);
    }
}

// Read from the hardware, keypress or switch hook
bool TimerApp::read_input(int& msg, int& arg)
{
     char c(0);

     bool b = readSwitchHook(hookUp_);
     if (!hookUp_ && b)
     {
         hookUp_= true;
         msg = SWITCH_HOOK_UP;
         arg = 0;
         return true;
     }
     if (hookUp_ && !b)
     {
         hookUp_= false;
         msg = SWITCH_HOOK_DOWN;
         arg = 0;
         return true;
     }

     c = keypad_ns::getKeyPress();
     if (c)
     {
         msg = MSG_KEY;
         arg = c;
         return true;
     }
     msg = MSG_NONE;
     arg = 0;
     return false;
}

void TimerApp::printMessage(char *text, int msg, int arg)
{
    char buf[128];
    switch (msg)
    {
    case MSG_NONE:
        sprintf(buf, "%s: msg: NONE, arg: %d", text, arg);
        break;
    case CREATE_NEW_TIMER:
        sprintf(buf, "%s: msg: CREATE_TIMER, arg: %d", text, arg);
        break;
    case CANCEL_TIMER:
        sprintf(buf, "%s: msg: CANCEL_TIMER, arg: %d", text, arg);
        break;
    case TIMER_EXPIRED:
        sprintf(buf, "%s: msg: TIMER_EXPIRED, arg: %d", text, arg);
        break;
    case SWITCH_TO_TIMER:
        sprintf(buf, "%s: msg: SWITCH_TO_TIMER, arg: %d", text, arg);
        break;
    case DRAW_TIMER:
        return;
        sprintf(buf, "%s: msg: DRAW_TIMER, arg: %d", text, arg);
        break;
    case MSG_KEY:
        sprintf(buf, "%s: msg: MSG_KEY, arg: %c", text, (char)arg);
        break;
    case SWITCH_HOOK_UP:
        sprintf(buf, "%s: msg: SWITCH_HOOK_UP, arg: %d", text, arg);
        break;
    case SWITCH_HOOK_DOWN:
        sprintf(buf, "%s: msg: SWITCH_HOOK_DOWN, arg: %d", text, arg);
        break;
    case PLAY_MESSAGE:
        sprintf(buf, "%s: msg: PLAY_MESSAGE, arg: %d", text, arg);
        break;
    case RECORD_MESSAGE:
        sprintf(buf, "%s: msg: RECORD_MESSAGE, arg: %d", text, arg);
        break;
    case CHECK_FOR_EXPIRED_TIMERS:
        return;
        sprintf(buf, "%s: msg: CHECK_FOR_EXPIRED_TIMERS, arg: %d", text, arg);
        break;
    default:
        sprintf(buf, "%s: msg: %d, arg: %d", text, msg, arg);
    }
    Serial.println(buf);
}

void TimerApp::loop()
{
    int msg, arg;
    if (get_message(msg, arg))
    {
        message_proc(msg, arg);
    }
    if (read_input(msg, arg))
    {
        //printMessage("read_input", msg, arg);
        post_message(msg, arg);
    }
    if (producer_== consumer_)
    {
        post_message(DRAW_TIMER, GET_CURRENT_TIMER());
        post_message(CHECK_FOR_EXPIRED_TIMERS, 0);
    }
    buzzer_.loop();
}

void TimerApp::post_message(int msg, int arg)
{
    //printMessage("post_message", msg, arg);
    msg_queue_[producer_].msg_type = msg;
    msg_queue_[producer_].arg = arg;
    producer_ = (++producer_) & (MAX_MESSAGES-1);
    //Serial.print("producer: ");
    //Serial.println(producer_);
}

bool TimerApp::get_message(int& msg, int& arg)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = MSG_NONE;
        msg_queue_[consumer_].arg = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg = msg_queue_[consumer_].arg;
    //printMessage("get_message", msg, arg);
    //Serial.print("consumer: ");
    //Serial.println(consumer_);
    consumer_ = (++consumer_) & (MAX_MESSAGES-1);
    return true;
}

void TimerApp::setup()
{
    display_ns::display.clearDisplay();
    displayNoTimers();
    buzzer_.setup();
    hookUp_ = false;
}

