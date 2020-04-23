#include "Arduino.h"
#include "message_queue.h"

MessageQueue::MessageQueue()
:consumer_(0)
,producer_(0)
,cb_(nullptr)
{
    for (int i = 0; i < MAX_MESSAGES;++i)
    {
        msg_queue_[i].msg_type = NULL_EVENT;
        msg_queue_[i].arg1 = 0;
        msg_queue_[i].arg2 = 0;
    }

    for (int i = 0; i < MAX_TIMERS;++i)
    {
        timers_[i].interval = 0;
        timers_[i].repeat = 0;
        timers_[i].nextTrigger = 0;
        timers_[i].id = 0;
    }

    for (int i = 0; i < MAX_PINS; ++i)
    {
        pins_[i].pin = 0;
        pins_[i].enabled = false;
        pins_[i].currentState = LOW;
        pins_[i].debounceTime = 0;
        pins_[i].triggerComplete = 0;
    }
}

bool MessageQueue::empty()
{
    return consumer_ == producer_;
}

void MessageQueue::register_event_handler(EVENT_CALLBACK cb)
{
    cb_ = cb;
}

int MessageQueue::OnEvent(int msg, int arg1, int arg2)
{
    return 0;
}

int MessageQueue::callback(int msg, int arg1, int arg2)
{
    if (cb_)
        return (*cb_)(msg, arg1, arg2);
    else
        return OnEvent(msg, arg1, arg2);
}

void MessageQueue::pump_message()
{
    int msg, arg1, arg2;
    check_timers();
    check_pins();
    if (get_message(msg, arg1, arg2))
    {
        callback(msg, arg1, arg2);
        return;
    }
    else
        callback(IDLE_EVENT, 0, 0);
}

void MessageQueue::post_message(int msg, int arg1, int arg2)
{
    msg_queue_[producer_].msg_type = msg;
    msg_queue_[producer_].arg1 = arg1;
    msg_queue_[producer_].arg2 = arg2;
    producer_ = (++producer_) & (MAX_MESSAGES-1);
}

int MessageQueue::send_message(int msg, int arg1, int arg2)
{
    return callback(msg, arg1, arg2);
}

bool MessageQueue::get_message(int& msg, int& arg1, int& arg2)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = NULL_EVENT;
        msg_queue_[consumer_].arg1 = 0;
        msg_queue_[consumer_].arg2 = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg1 = msg_queue_[consumer_].arg1;
    arg2 = msg_queue_[consumer_].arg2;
    consumer_ = (++consumer_) & (MAX_MESSAGES-1);
    return true;
}

bool MessageQueue::peek_message(int& msg, int& arg1, int& arg2)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = NULL_EVENT;
        msg_queue_[consumer_].arg1 = 0;
        msg_queue_[consumer_].arg2 = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg1 = msg_queue_[consumer_].arg1;
    arg2 = msg_queue_[consumer_].arg2;
    return true;
}

bool MessageQueue::create_timer(int id, unsigned long interval, bool repeat)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == 0)
        {
            timers_[n].interval = interval;
            timers_[n].id = id;
            timers_[n].repeat = repeat;
            timers_[n].nextTrigger = millis() + interval;
            return true;
        }
    }
    return false;
}

void MessageQueue::cancel_timer(int id)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == id)
        {
            timers_[n].interval = 0;
            timers_[n].id = 0;
            timers_[n].repeat = false;
            timers_[n].nextTrigger = 0;
            return;
        }
    }
}

void MessageQueue::check_timers()
{
    unsigned long now = millis();
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        Timer& timer = timers_[n];
        if (timer.id)
        {
            if (timer.nextTrigger < now)
            {
                callback(TIMER_EVENT, timer.id, 0);
                if (timer.repeat)
                    timer.nextTrigger = now + timer.interval;
                else
                    cancel_timer(n);
            }
        }
    }
}

bool MessageQueue::digitalRead(int pin, int def, unsigned long debounceTimeMicros)
{
    for (int i = 0; i < MAX_PINS; ++i)
    {
        if (pins_[i].enabled == false)
        {
            pins_[i].pin = pin;
            pins_[i].digitalRead = true;
            pins_[i].enabled = true;
            pins_[i].currentState = def;
            pins_[i].debounceTime = debounceTimeMicros;
            pins_[i].triggerComplete = 0;
            return true;
        }
    }
    return false;
}

bool MessageQueue::analogRead(int pin, unsigned long timeMicros)
{
    for (int i = 0; i < MAX_PINS; ++i)
    {
        if (pins_[i].enabled == false)
        {
            pins_[i].pin = pin;
            pins_[i].digitalRead = false;
            pins_[i].enabled = true;
            pins_[i].currentState = LOW;
            pins_[i].debounceTime = timeMicros;
            pins_[i].triggerComplete = 0;
            return true;
        }
    }
    return false;

}

void MessageQueue::check_pins()
{
    for (int idx = 0; idx < MAX_PINS; idx++)
    {
        if (pins_[idx].enabled)
        {
            if (pins_[idx].digitalRead)
                digital_check(pins_[idx]);
            else
                analog_check(pins_[idx]);
        }
    }
}

void MessageQueue::digital_check(Pin& pin)
{
    int s = ::digitalRead(pin.pin);
    if(pin.currentState != s)
    {
        if (pin.debounceTime == 0)
        {
            callback(DIGITAL_READ_EVENT, pin.pin, s);
        }
        else if(pin.triggerComplete == 0)
        {
            pin.triggerComplete = micros() + pin.debounceTime;
        }
        else
        {
            if (pin.triggerComplete < micros())
            {
                int s = ::digitalRead(pin.pin);
                if (s != pin.currentState)
                {
                    pin.currentState = s;
                    callback(DIGITAL_READ_EVENT, pin.pin, s);
                    pin.triggerComplete = 0;
                }
            }
        }
    }
}

void MessageQueue::analog_check(Pin& pin)
{
    if (pin.debounceTime == 0)
    {
        int v = ::analogRead(pin.pin);

        callback(ANALOG_READ_EVENT, pin.pin, v);
        return;
    }
    if (pin.triggerComplete == 0)
    {
        pin.triggerComplete = micros() + pin.debounceTime;
        return;
    }

    if (pin.triggerComplete < micros())
    {
        int v = ::analogRead(pin.pin);
        callback(ANALOG_READ_EVENT, pin.pin, v);
        pin.triggerComplete = micros() + pin.debounceTime;
        return;
    }
}

