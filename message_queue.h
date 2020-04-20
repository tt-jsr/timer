#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#define MAX_MESSAGES 16 // power of two
#define MAX_TIMERS   10

struct Message
{
    int msg_type;
    int arg;
};

struct Timer
{
    unsigned long interval;
    unsigned long nextTrigger;
    bool repeat;
    int id;
};

const int NULL_EVENT = 32000;
const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;

typedef int (*PROC_CALLBACK)(int, int);
class MessageQueue
{
public:
    MessageQueue();
    void register_proc(PROC_CALLBACK);

    // Post a message to the queue
    void post_message(int msg, int arg);

    // Send a message to the proc synchronously.
    // The return value is the return value of the callback
    // This call bypasses the queue and will be executed ahead
    // of any queued messages
    int send_message(int msg, int arg);

    // Get and remove a message from the queue
    bool get_message(int& msg, int& arg);

    // Get a message without removing it from the queue
    bool peek_message(int& msg, int& arg);

    // Is the queue empty?
    bool empty();

    // Must be called periodically to invoke the callback
    // with posted messages. If the queue is empty
    // IDLE_EVENT is placed on the callback if the app
    // needs to do any processing during idle times.
    void pump_message();

    // Create a timer. TIMER_EVENT will be posted to the callback
    // with the id as an argument when the timer fires
    // Returns true if the timer is created
    bool create_timer(int id, unsigned long interval_ms, bool repeat);

    // Cancel a timer given the id
    void cancel_timer(int id);
private:
    void check_timers();
    Message msg_queue_[MAX_MESSAGES];
    Timer timers_[MAX_TIMERS];
    int consumer_;
    int producer_;
    PROC_CALLBACK cb_;
};


#endif
