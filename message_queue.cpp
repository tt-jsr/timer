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
        msg_queue_[i].arg=0;
    }

    for (int i = 0; i < MAX_TIMERS;++i)
    {
        timers_[i].interval = 0;
        timers_[i].repeat = 0;
        timers_[i].nextTrigger = 0;
        timers_[i].id = 0;
    }
}

bool MessageQueue::empty()
{
    return consumer_ == producer_;
}

void MessageQueue::register_proc(PROC_CALLBACK cb)
{
    cb_ = cb;
}

void MessageQueue::pump_message()
{
    int msg, arg;
    check_timers();
    if (get_message(msg, arg))
    {
        (*cb_)(msg, arg);
        return;
    }
    else
        (*cb_)(IDLE_EVENT, 0);
}

void MessageQueue::post_message(int msg, int arg)
{
    msg_queue_[producer_].msg_type = msg;
    msg_queue_[producer_].arg = arg;
    producer_ = (++producer_) & (MAX_MESSAGES-1);
}

int MessageQueue::send_message(int msg, int arg)
{
    return (*cb_)(msg, arg);
}

bool MessageQueue::get_message(int& msg, int& arg)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = NULL_EVENT;
        msg_queue_[consumer_].arg = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg = msg_queue_[consumer_].arg;
    consumer_ = (++consumer_) & (MAX_MESSAGES-1);
    return true;
}

bool MessageQueue::peek_message(int& msg, int& arg)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = NULL_EVENT;
        msg_queue_[consumer_].arg = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg = msg_queue_[consumer_].arg;
    return true;
}

void MessageQueue::create_timer(int id, unsigned long interval, bool repeat)
{
    for (int n = 0; n < MAX_TIMERS; ++n)
    {
        if (timers_[n].id == 0)
        {
            timers_[n].interval = interval;
            timers_[n].id = id;
            timers_[n].repeat = repeat;
            timers_[n].nextTrigger = millis() + interval;
        }
    }
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
            if ((timer.interval + timer.nextTrigger) < now)
            {
                (*cb_)(TIMER_EVENT, timer.id);
                if (timer.repeat)
                {
                    timer.nextTrigger = now + timer.interval;
                }
                else
                {
                    timer.interval = 0;
                    timer.id = 0;
                    timer.repeat = false;
                    timer.nextTrigger = 0;
                }
            }
        }
    }
}
