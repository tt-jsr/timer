#ifndef TEST_H_
#define TEST_H_

/*****************************************
 * Test code
 */


struct TimerTest : public Application
{
    void setup();
    void loop();
    int timerno_;
};

struct KeypadTest : public Application
{
    void setup();
    void loop();

    int nchar;
};

#endif
