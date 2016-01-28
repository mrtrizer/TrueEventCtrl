#ifndef TESTEVENTSENDER_H
#define TESTEVENTSENDER_H

#include <QThread>
#include "eventsender.h"

class TestEventSender : public QThread
{
public:
    TestEventSender(int n);
    ~TestEventSender();
    void run();
    EventSender eventSender;
private:
    unsigned int i;
    unsigned int n;
};

#endif // TESTEVENTSENDER_H
