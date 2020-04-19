#ifndef TIMER_BUFFER_H_
#define TIMER_BUFFER_H_

#define BUFFER_SIZE 8 // must be power of two

struct TimerBuffer
{
    TimerBuffer();
    void addChar(char c);
    char getChar(size_t idx);
    size_t size();
    int getSeconds();
    void format(char *buf, size_t buflen);

    char buf[BUFFER_SIZE];
    size_t pos;
};

#endif
