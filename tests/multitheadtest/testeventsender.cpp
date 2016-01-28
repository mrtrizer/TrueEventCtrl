#include <QDebug>

#include "testeventsender.h"
#include "event.h"
#include "eventctrl.h"

TestEventSender::TestEventSender(int n):i(0),n(n)
{

}

TestEventSender::~TestEventSender()
{

}

void TestEventSender::run()
{
    while (1)
    {
        qDebug() << "Sender" << n << "| Send event" << i;
        eventSender.SEND(i); //We send current counter value
        i++;
        sleep(1);
    }
}
