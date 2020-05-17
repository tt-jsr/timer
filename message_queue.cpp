#include "Arduino.h"
#include "message_queue.h"

MessageQueue::MessageQueue()
:consumer_(0)
,producer_(0)
, debug_(false)
{
    for (int i = 0; i < MAX_MESSAGES;++i)
    {
        msg_queue_[i].msg_type = 0;
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
        pins_[i].id = 0;
        pins_[i].pin = 0;
        pins_[i].debounceTime = 0;
        pins_[i].triggerComplete = 0;
    }
    for (int i = 0; i < MAX_VALUES; ++i)
    {
        values_[i].id = 0;
        values_[i].value = 0;
    }
}

bool MessageQueue::empty()
{
    return consumer_ == producer_;
}

void MessageQueue::get_message(int& msg, int& arg1, int& arg2)
{
    if (consume_message(msg, arg1, arg2))
        return;

    OnGenerator();
    check_timers();
    check_pins();
    msg = IDLE_EVENT;
    arg1 = 0;
    arg2 = 0;
}

void MessageQueue::post_message(int msg, int arg1, int arg2)
{
    msg_queue_[producer_].msg_type = msg;
    msg_queue_[producer_].arg1 = arg1;
    msg_queue_[producer_].arg2 = arg2;
    producer_ = (++producer_) & (MAX_MESSAGES-1);
    if (producer_ == consumer_)
    {
        // If we just posted and reached the consumer_, our queue is full
        // so advance the consumer and drop the message
        ++consumer_;
        if (debug_)
            Serial.println("MessageQueue: Queue full, message dropped");
    }
}

bool MessageQueue::consume_message(int& msg, int& arg1, int& arg2)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = 0;
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
        msg_queue_[consumer_].msg_type = 0;
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
            if (debug_)
            {
                Serial.print("MessageQueue: create_timer(), id=");
                Serial.println(id);
            }
            return true;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: create_timer() failed, id=");
        Serial.println(id);
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
            if (debug_)
            {
                Serial.print("MessageQueue: cancel_timer(), id=");
                Serial.println(id);
            }
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: cancel_timer() failed, id=");
        Serial.println(id);
    }
}

void MessageQueue::reset_timer(int id, unsigned long interval, bool repeat)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == id)
        {
            timers_[n].interval = interval;
            timers_[n].repeat = repeat;
            timers_[n].nextTrigger = millis() + timers_[n].interval;
            if (debug_)
            {
                Serial.print("MessageQueue: reset_timer(id, interval, repeat), id=");
                Serial.print(id);
                Serial.print(" interval=");
                Serial.print(interval);
                Serial.print(" repeat=");
                Serial.println(repeat);
            }
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: reset_timer() failed, id=");
        Serial.println(id);
    }
}

void MessageQueue::reset_timer(int id)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == id)
        {
            timers_[n].nextTrigger = millis() + timers_[n].interval;
            if (debug_)
            {
                Serial.println("MessageQueue: reset_timer(), id=");
            }
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: reset_timer() failed, id=");
        Serial.println(id);
    }
}

void MessageQueue::pause_timer(int id)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == id)
        {
            timers_[n].nextTrigger = 0;
            if (debug_)
            {
                Serial.println("MessageQueue: pause_timer(), id=");
            }
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: pause_timer() failed, id=");
        Serial.println(id);
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
            if (timer.nextTrigger && timer.nextTrigger <= now)
            {
                post_message(TIMER_EVENT, timer.id, now-timer.nextTrigger);
                if (timer.repeat)
                    timer.nextTrigger = now + timer.interval;
                else
                    timer.nextTrigger = 0;
            }
        }
    }
}

bool MessageQueue::digitalRead(int id, int pin, int def, unsigned long debounceTimeMicros)
{
    for (int i = 0; i < MAX_PINS; ++i)
    {
        if (pins_[i].id == 0)
        {
            pins_[i].id = id;
            pins_[i].pin = pin;
            pins_[i].digitalRead = true;
            pins_[i].debounceTime = debounceTimeMicros;
            pins_[i].triggerComplete = 0;
            if (debug_)
            {
                Serial.print("MessageQueue: digitalRead(), id=");
                Serial.println(id);
            }
            create_value(id, def);
            return true;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: digitalRead() failed, id=");
        Serial.println(id);
    }
    return false;
}

bool MessageQueue::analogRead(int id, int pin, unsigned long timeMicros)
{
    for (int i = 0; i < MAX_PINS; ++i)
    {
        if (pins_[i].id == 0)
        {
            pins_[i].id = id;
            pins_[i].pin = pin;
            pins_[i].digitalRead = false;
            pins_[i].debounceTime = timeMicros;
            pins_[i].triggerComplete = 0;
            if (debug_)
            {
                Serial.print("MessageQueue: analogRead(), id=");
                Serial.println(id);
            }
            create_value(id, 0);
            return true;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: analogRead() failed, id=");
        Serial.println(id);
    }
    return false;

}

void MessageQueue::check_pins()
{
    for (int idx = 0; idx < MAX_PINS; idx++)
    {
        if (pins_[idx].id)
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
    if (pin.debounceTime == 0)
    {
        set_value(pin.id, s);
    }
    else if(pin.triggerComplete == 0)
    {
        pin.triggerComplete = micros() + pin.debounceTime;
    }
    else if (pin.triggerComplete < micros())
    {
        set_value(pin.id, s);
        pin.triggerComplete = 0;
    }
}

void MessageQueue::analog_check(Pin& pin)
{
    if (pin.debounceTime == 0)
    {
        int v = ::analogRead(pin.pin);

        set_value(pin.id, v);
    }
    if (pin.triggerComplete == 0)
    {
        pin.triggerComplete = micros() + pin.debounceTime;
    }

    if (pin.triggerComplete < micros())
    {
        int v = ::analogRead(pin.pin);
        set_value(pin.id, v);
        pin.triggerComplete = micros() + pin.debounceTime;
    }
}

bool MessageQueue::create_value(int id, int v)
{
    for (size_t idx = 0; idx < MAX_VALUES; idx++)
    {
        if (values_[idx].id == 0)
        {
            values_[idx].id = id;
            values_[idx].value = v;
            if (debug_)
            {
                Serial.print("MessageQueue: create_value() success, id=");
                Serial.println(id);
            }
            return true;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: create_value() failed, id=");
        Serial.println(id);
    }
    return false;
}

void MessageQueue::set_value(int id, int v)
{
    for (size_t idx = 0; idx < MAX_VALUES; idx++)
    {
        if (values_[idx].id == id)
        {
            if (values_[idx].value != v)
            {
                values_[idx].value = v;
                post_message(VALUE_EVENT, id, v);
            }
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: set_value() failed, id=");
        Serial.println(id);
    }
}

int MessageQueue::get_value(int id)
{
    for (size_t idx = 0; idx < MAX_VALUES; idx++)
    {
        if (values_[idx].id == id)
        {
            return values_[idx].value;
        }
    }
    return -1;
}

void MessageQueue::toggle_value(int id)
{
    for (size_t idx = 0; idx < MAX_VALUES; idx++)
    {
        if (values_[idx].id == id)
        {
            values_[idx].value = !values_[idx].value;
            post_message(VALUE_EVENT, id, values_[idx].value);
            return;
        }
    }
    if (debug_)
    {
        Serial.print("MessageQueue: toggle_value() failed, id=");
        Serial.println(id);
    }
}

void MessageQueue::OnGenerator()
{
}

void MessageQueue::setDebug(bool dbg)
{
    debug_ = dbg;
}

