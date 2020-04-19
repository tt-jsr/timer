#include "timer.h"
#include "timer_buffer.h"

#define MAX_DIGITS 5

TimerBuffer::TimerBuffer()
:pos(0)
{
    memset(buf, 0, BUFFER_SIZE);
}

void TimerBuffer::addChar(char c)
{
    if (pos == MAX_DIGITS+1)
    {
        memmove(buf, buf+1, MAX_DIGITS-1);
        buf[MAX_DIGITS] = '\0';
        --pos;
        buf[pos] = '\0';
    }
    buf[pos++] = c;
    buf[pos] = '\0';
}

int TimerBuffer::getSeconds()
{
    if (buf[0] == '\0')
        return 0;
    int idx = (int)pos-1;
    int secs = buf[idx] - '0';
    --idx;
    if (idx < 0)
        return secs;
    secs += (buf[idx] - '0') * 10;
    --idx;
    if (idx < 0)
        return secs;
    secs += (buf[idx] - '0') * 60;
    --idx;
    if (idx < 0)
        return secs;
    secs += (buf[idx] - '0') * 600;
    --idx;
    if (idx < 0)
        return secs;
    secs += (buf[idx] - '0') * 3600;
    return secs;
}

void TimerBuffer::format(char *outbuf, size_t buflen)
{
    int secs = getSeconds();
    int hours = secs/3600;
    secs -= hours*3600;
    int min = secs/60;
    secs -= min*60;
    if (hours)
        snprintf(outbuf, buflen, "%d:%02d:%02d", hours, min, secs);
    else
        snprintf(outbuf, buflen, "%d:%02d", min, secs);
}

