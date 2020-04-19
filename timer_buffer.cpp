#include "timer.h"
#include "timer_buffer.h"

#define MAX_DIGITS 5

TimerBuffer::TimerBuffer()
{
    memset(buf, 0, BUFFER_SIZE);
    pos = 0;
}

void TimerBuffer::addChar(char c)
{
    buf[(pos+1) & (BUFFER_SIZE-1)] = c;
    ++pos;
}

char TimerBuffer::getChar(size_t idx)
{
    size_t s = size();
    if (idx >= s)
        return 0;
    idx = s-(idx+1);
    return buf[(pos-idx) & (BUFFER_SIZE-1)];
}

size_t TimerBuffer::size()
{
    if (pos > MAX_DIGITS)
        return MAX_DIGITS;
    return pos;
}

int TimerBuffer::getSeconds()
{
    int secs = 0;

    size_t idx = size()-1;
    char c = getChar(idx);
    secs += c - '0';

    c = getChar(--idx);
    if (c == 0)
        return secs;
    secs += (c - '0') * 10;

    c = getChar(--idx);
    if (c == 0)
        return secs;
    secs += (c - '0') * 60;

    c = getChar(--idx);
    if (c == 0)
        return secs;
    secs += (c - '0') * 600;

    c = getChar(--idx);
    if (c == 0)
        return secs;
    secs += (c - '0') * 3600;

    return secs;
}

void TimerBuffer::format(char *buf, size_t buflen)
{
    size_t startpos = 0;
    if (pos > MAX_DIGITS)
        startpos = (pos-MAX_DIGITS) & (BUFFER_SIZE-1);
    switch (size())
    {
    case 5:
        buf[0] = getChar(0);
        buf[1] = ':';
        buf[2] = getChar(1);
        buf[3] = getChar(2);
        buf[4] = ':';
        buf[5] = getChar(3);
        buf[6] = getChar(4);
        buf[7] = 0;
        break;
    case 4:
        buf[0] = getChar(0);
        buf[1] = getChar(1);
        buf[2] = ':';
        buf[3] = getChar(2);
        buf[4] = getChar(3);
        buf[5] = 0;
        break;
    case 3:
        buf[0] = getChar(0);
        buf[1] = ':';
        buf[2] = getChar(1);
        buf[3] = getChar(2);
        buf[4] = 0;
        break;
    case 2:
        buf[0] = getChar(0);
        buf[1] = getChar(1);
        buf[2] = 0;
        break;
    case 1:
        buf[0] = getChar(0);
        buf[1] = 0;
        break;
    }
}

