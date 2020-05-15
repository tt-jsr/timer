#ifndef APPLICATION_H_
#define APPLICATION_H_

/*****************************************
 * Application loop
 */

struct Application
{
    virtual void loop() = 0;
    virtual void setup(bool debug) = 0;
};

#endif
