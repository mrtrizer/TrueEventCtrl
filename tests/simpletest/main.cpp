#include <QDebug>
#include "event.h"
#include "eventcontroller.h"
#include "eventsender.h"

void handler(Event * event)
{
    qDebug() << "Event";
    qDebug() << *(QString *)event->getArg();
}

//Simple single-thread test.
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    QString str = "test"; //Example string argument
    EventSender eventSender; //Event sender
    eventSender.addEventListener(0,handler); //Adding event listener

    //You can use two form for event sending.
    eventSender.sendEvent((void*)&str,sizeof(str));
    eventSender.SEND(str); //Event emiting
    EventCtrl::runOnce(); //Runing single queue checking.
    return 0;
}
