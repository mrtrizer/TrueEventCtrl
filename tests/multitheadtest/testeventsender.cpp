#include "testeventsender.h"
#include "event.h"
#include "eventctrl.h"

TestEventSender::TestEventSender():i(0)
{

}

TestEventSender::~TestEventSender()
{

}

void TestEventSender::run()
{
    while (1)
    {
        eventSender.SEND(i); //We send current counter value
        i++;
        sleep(1);
    }
}
