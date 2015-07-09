#ifndef TESTEVENTLISTENER_H
#define TESTEVENTLISTENER_H

#include "eventsender.h"

class TestEventListener
{
public:
    TestEventListener();
    ~TestEventListener();

    //Listener method
    void testListener(Event *event);
    //Abstract listener method (experiment)
    virtual void abatractTestListener(Event *event) = 0;

    //Register the listener method. It have to be in the public section.
    //And must get the class name in second argument.
    ADD_LISTENER(testListener,TestEventListener)
    ADD_LISTENER(abatractTestListener,TestEventListener)
};

#endif // TESTEVENTLISTENER_H
