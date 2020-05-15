#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include "message_queue_impl.h"

const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;
const int VALUE_EVENT = 32003;
const int USER_EVENT_BASE = 32004;


// Each queue slot takes 6 bytes
#define MAX_MESSAGES 16 // must be a power of two

// Each timer consumes 12 bytes
#define MAX_TIMERS   5

// Each pin consumes 18 bytes
// Set to the number of pins you will use for the
// message queue digitalRead() or analogRead() events. Do not set this
// to the number of pins supported by your board.
// If you will be reading pins directly and not using the queue, you 
// do not need to include them here.
#define MAX_PINS     4

// Set this to be the number of state
// variables you will be using
#define NUMBER_OF_VALUES    2


#define MAX_VALUES   (MAX_PINS + NUMBER_OF_VALUES)

class MessageQueue
{
public:
    MessageQueue();

    // Override if the application would like to be able to generate
    // events. This can be useful for the override to post keypresses
    // any other events into the message_pump
    virtual void OnGenerator();

    /*************************************************************
     Message and queing
    *************************************************************/

    // Post a message to the queue
    void post_message(int msg, int arg1, int arg2);

    // Is the queue empty?
    bool empty();

    // Get and remove a message from the queue. 
    // If there is no message in the queue, check timers and pins
    // and post events, then return an IDLE_EVENT.
    //
    // This call will always return either a queued message, or IDLE_EVENT
    void get_message(int& msg, int& arg1, int& arg2);


    // Get a message from the queue. This does not check timers or pins
    // and does not return any IDLE_EVENT
    bool consume_message(int& msg, int& arg1, int& arg2);

    // Get a message without removing it from the queue
    bool peek_message(int& msg, int& arg, int& arg2);

    /****************************************************************
    Timers
    ****************************************************************/

    // Create a timer. TIMER_EVENT will be posted to the queue
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
    // a VALUE_EVENT message will be posted with the new value
    void set_value(int id, int v);

    // Get the current value
    int get_value(int id, int v);

    // Toggle a value. Treats the value as a boolean, toggles between
    // 1 and 0. A VALUE_EVENT message will be posted.
    void toggle_value(int id);

    // Enable printing to the monitor for debugging
    void setDebug(bool debug);
private:
    void check_timers();
    void check_pins();
    void digital_check(Pin&);
    void analog_check(Pin&);
    Message msg_queue_[MAX_MESSAGES];
    Pin pins_[MAX_PINS];
    Timer timers_[MAX_TIMERS];
    Value values_[MAX_VALUES];
    unsigned int consumer_;
    unsigned int producer_;
    bool debug_;
};


#endif
