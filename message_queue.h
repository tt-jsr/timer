#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include "message_queue_impl.h"

const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;
const int VALUE_EVENT = 32003;

// Each queue slot takes 6 bytes
#define MAX_MESSAGES 16 // must be a power of two

// Each timer consumes 12 bytes
#define MAX_TIMERS   5

// Each pin consumes 18 bytes
// Set to the number of pins you will use for the
// message queue digitalRead() or analogRead() events. If you will be reading
// pins directly and not using the queue, you do not need to include
// them here.
#define MAX_PINS     4

// Set this to be the number of state
// variables you will be using
#define NUMBER_OF_VALUES    2


#define MAX_VALUES   (MAX_PINS + NUMBER_OF_VALUES)

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

    // Get the currently installed event handler.
    EVENT_CALLBACK get_event_handler();


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

    // Get and remove a message from the queue. Normally you will not
    // call this as pump_message() will do this for you
    bool get_message(int& msg, int& arg1, int& arg2);

    // Get a message without removing it from the queue
    bool peek_message(int& msg, int& arg, int& arg2);

    // Is the queue empty?
    bool empty();

    // Must be called periodically to invoke the callback
    // with posted messages. 
    // This function does the following things:
    // 1. Process all messages currently in the queue, if any.
    // 2. Invoke the callback with IDLE_EVENT. This will 
    //    always be called.
    // 3. Post any timer, digital or analog read events to the queue.
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

    // Generate VALUE_EVENT messages when this given pin
    // changes state
    // def: LOW or HIGH
    // debounce: Time in microseconds to debounce the input. 
    //           Zero for no debounce
    bool digitalRead(int id, int pin, int def, unsigned long debounceTimeMicros);

    // Generate VALUE_EVENT messages
    // timeMicros: set to 0 to post an event on every call to the message_pump(),
    // otherwise, post a read every timeMicros microseconds
    bool analogRead(int id, int pin, unsigned long timeMicros);

    /***************************************************************
     State variables
    ***************************************************************/
    // Create a value that when changed, generates a VALUE_EVENT message
    bool create_value(int id, int v);

    // Set a value. If the vaue differs from the current value
    // a VALUE_EVENT message willbe posted with the new value
    void set_value(int id, int v);

    // Get the current value
    int get_value(int id, int v);

    // Toggle a value. Treats the value as a boolean, toggles between
    // 1 and 0. A VALUE_EVENT message will be posted.
    void toggle_value(int id);

    // Enable printing to the monitor for debugging
    void setDebug();
private:
    void check_timers();
    void check_pins();
    void digital_check(Pin&);
    void analog_check(Pin&);
    int callback(int msg, int arg1, int arg2);
    Message msg_queue_[MAX_MESSAGES];
    Pin pins_[MAX_PINS];
    Timer timers_[MAX_TIMERS];
    Value values_[MAX_VALUES];
    unsigned int consumer_;
    unsigned int producer_;
    EVENT_CALLBACK cb_;
    bool debug_;
};


#endif
