#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#define MAX_MESSAGES 16 // must be a power of two
#define MAX_TIMERS   10
#define MAX_PINS     20

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

struct Pin
{
    bool enabled;
    bool currentState;  // for digital read only
    unsigned long  debounceTime;   // for digital read only
    unsigned long  triggerComplete;    // for digital read only
};

const int NULL_EVENT = 32000;
const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;
const int DIGITAL_READ_EVENT = 32003;

int message_queue_pin(int eventValue);
int message_queue_state(int eventValue);

typedef int (*PROC_CALLBACK)(int, int);
class MessageQueue
{
public:
    MessageQueue();
    void register_proc(PROC_CALLBACK);


    // If no callback is registered, this virtual
    // function will be called to be implemented in a derived class
    virtual int OnEvent(int msg, int arg);

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

    // Generate DIGITAL_READ_EVENT messages when this given pin
    // changes state
    // def: LOW or HIGH
    // debounce: Use a delay to debounce the reading
    bool digitalRead(int pin, int def, unsigned long debounceTimeMicros);
private:
    void check_timers();
    void check_pins();
    int callback(int msg, int arg);
    Message msg_queue_[MAX_MESSAGES];
    Pin pins_[MAX_PINS];
    Timer timers_[MAX_TIMERS];
    int consumer_;
    int producer_;
    PROC_CALLBACK cb_;
};


#endif
