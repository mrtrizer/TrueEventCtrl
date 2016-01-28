#include <QDebug>
#include "event.h"
#include "eventctrl.h"
#include "eventsender.h"
#include "testeventsender.h"
#include "testeventlistener2.h"

void handler(Event * event)
{
    qDebug() << "Event";
    qDebug() << *(int *)event->getArg();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TestEventListener2 testEventListener1(1);
    TestEventListener2 testEventListener2(2);
    //Create 100 threads for extreme test
    for (int i = 0; i < 5; i++)
    {
        TestEventSender * testEventSender = new TestEventSender(i); //Thread creating
        testEventSender->eventSender.addEventListener(&testEventListener1,testEventListener1.LISTENER(testListener),EventCtrl::DIRECT); //Add event listener
        testEventSender->eventSender.addEventListener(&testEventListener2,testEventListener2.LISTENER(testListener),EventCtrl::DIRECT); //Add event listener
        testEventSender->start(); //And start the therad
    }

    EventCtrl::run();
    return 0;
}
