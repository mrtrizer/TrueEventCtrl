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
    TestEventListener2 testEventListener;
    //Create 100 threads for extreme test
    for (int i = 0; i < 100; i++)
    {
        TestEventSender * testEventSender = new TestEventSender; //Thread creating
        testEventSender->eventSender.addEventListener(&testEventListener,testEventListener.LISTENER(testListener)); //Add event listener
        testEventSender->eventSender.addEventListener(&testEventListener,testEventListener.LISTENER(abatractTestListener)); //Add event listener
        testEventSender->start(); //And start the therad
    }

    EventCtrl::run();
    return 0;
}
