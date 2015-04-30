#ifndef TESTEVENTLISTENER_H
#define TESTEVENTLISTENER_H

#include "eventsender.h"

class TestEventListener
{
public:
    TestEventListener();
    ~TestEventListener();

    ADD_LISTENER(testListener,TestEventListener);

};

#endif // TESTEVENTLISTENER_H
