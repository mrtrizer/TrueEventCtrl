#include "testeventsender.h"
#include "event.h"
#include "eventcontroller.h"

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
        eventSender.sendEvent(&i,sizeof(unsigned int)); //We send current counter value
        i++;
        sleep(1);
    }
}
