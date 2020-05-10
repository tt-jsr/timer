#ifndef MESSAGE_QUEUE_IMPL_H_
#define MESSAGE_QUEUE_IMPL_H_

struct Message
{
    int msg_type;
    int arg1;
    int arg2;
};

struct Timer
{
    unsigned long interval;
    unsigned long nextTrigger;
    bool repeat;
    int id;
};

struct Pin
{
    int id;
    int pin;
    bool digitalRead;
    unsigned long  debounceTime;  
    unsigned long  triggerComplete;    
};

struct Value
{
    int id;
    int value;
};

const int NULL_EVENT = 32000;

typedef int (*EVENT_CALLBACK)(int, int, int);

#endif
