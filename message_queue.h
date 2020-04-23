#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

// Each queue slot takes 6 bytes
#define MAX_MESSAGES 8 // must be a power of two

// Each timer consumes 12 bytes
#define MAX_TIMERS   5

// Each pin consumes 16 bytes
// Set to the number of pins you will use
#define MAX_PINS     4

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
    int pin;
    bool digitalRead;
    bool enabled;
    bool currentState;  // for digital read only
    unsigned long  debounceTime;  
    unsigned long  triggerComplete;    
};

const int NULL_EVENT = 32000;
const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;
const int DIGITAL_READ_EVENT = 32003;
const int ANALOG_READ_EVENT = 32004;

typedef int (*EVENT_CALLBACK)(int, int, int);
class MessageQueue
{
public:
    MessageQueue();

    /************************************************************
     Callbacks
    ************************************************************/

    // Regster an event handler to receive events. It must have a signiture of
    // int foo(int msg, int arg1, int arg2);
    //
    // If no callback is registered the virtual function OnEvent()
    // willl be called to be handled by a derived class
    void register_event_handler(EVENT_CALLBACK);


    // If no handler is registered, this virtual
    // function will be called to be implemented in a derived class
    virtual int OnEvent(int msg, int arg1, int arg2);

    /*************************************************************
     Message and queing
    *************************************************************/

    // Post a message to the queue
    void post_message(int msg, int arg1, int arg2);

    // Send a message to the event handler synchronously.
    // The return value is the return value of the handler
    // This call bypasses the queue and will be executed ahead
    // of any queued messages
    int send_message(int msg, int arg1, int arg2);

    // Get and remove a message from the queue
    bool get_message(int& msg, int& arg1, int& arg2);

    // Get a message without removing it from the queue
    bool peek_message(int& msg, int& arg, int& arg2);

    // Is the queue empty?
    bool empty();

    // Must be called periodically to invoke the callback
    // with posted messages. If the queue is empty
    // IDLE_EVENT is placed on the callback if the app
    // needs to do any processing during idle times.
    void pump_message();

    /****************************************************************
    Timers
    ****************************************************************/

    // Create a timer. TIMER_EVENT will be posted to the callback
    // with the id as an argument when the timer fires
    // Returns true if the timer is created
    bool create_timer(int id, unsigned long interval_ms, bool repeat);

    // Cancel a timer given the id
    void cancel_timer(int id);

    /****************************************************************
     Reading pins
    ****************************************************************/

    // Generate DIGITAL_READ_EVENT messages when this given pin
    // changes state
    // def: LOW or HIGH
    // debounce: Time in microseconds to debounce the input. 
    //           Zero for no debounce
    bool digitalRead(int pin, int def, unsigned long debounceTimeMicros);

    // Generate ANALOG_READ_EVENT messages
    // timeMicros: set to 0 to post an event on every call to the message_pump(),
    // otherwise, post a read every timeMicros microseconds
    bool analogRead(int pin, unsigned long timeMicros);
private:
    void check_timers();
    void check_pins();
    void digital_check(Pin&);
    void analog_check(Pin&);
    int callback(int msg, int arg1, int arg2);
    Message msg_queue_[MAX_MESSAGES];
    Pin pins_[MAX_PINS];
    Timer timers_[MAX_TIMERS];
    int consumer_;
    int producer_;
    EVENT_CALLBACK cb_;
};


#endif
