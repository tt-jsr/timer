#ifndef APPLICATION_H_
#define APPLICATION_H_

/*****************************************
 * Application loop
 */

struct Application
{
    virtual void loop() = 0;
    virtual void setup() = 0;
};

#endif
