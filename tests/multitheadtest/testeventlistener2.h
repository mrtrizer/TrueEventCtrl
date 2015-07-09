#ifndef TESTEVENTLISTENER2_H
#define TESTEVENTLISTENER2_H

#include "testeventlistener.h"

class TestEventListener2 : public TestEventListener
{
public:
    TestEventListener2();
    ~TestEventListener2();
    void abatractTestListener(Event *event);
};

#endif // TESTEVENTLISTENER2_H
