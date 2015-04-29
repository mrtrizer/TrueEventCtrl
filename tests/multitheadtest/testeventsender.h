#ifndef TESTEVENTSENDER_H
#define TESTEVENTSENDER_H

#include <QThread>
#include "eventsender.h"

class TestEventSender : public QThread
{
public:
    TestEventSender();
    ~TestEventSender();
    void run();
    EventSender eventSender;
    unsigned int i;
};

#endif // TESTEVENTSENDER_H
