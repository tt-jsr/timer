#ifndef TIMER_BUFFER_H_
#define TIMER_BUFFER_H_

#define BUFFER_SIZE 10 

struct TimerBuffer
{
    TimerBuffer();
    void addChar(char c);
    int getSeconds();
    void format(char *buf, size_t buflen);

    char buf[BUFFER_SIZE];
    size_t pos;
};

#endif
