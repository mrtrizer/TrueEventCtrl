#include <QDebug>
#include "event.h"
#include "eventcontroller.h"
#include "eventsender.h"
#include "testeventsender.h"

void handler(Event * event)
{
    qDebug() << "Event";
    qDebug() << *(int *)event->getArg();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    //Create 100 threads for extreme test
    for (int i = 0; i < 100; i++)
    {
        TestEventSender * testEventSender = new TestEventSender; //Thread creating
        EventController::addEventListener(&testEventSender->eventSender, handler); //Add event listener
        testEventSender->start(); //And start the therad
    }

    EventController::run();
    return 0;
}
