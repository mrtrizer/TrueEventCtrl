#include <QDebug>
#include "event.h"
#include "eventcontroller.h"
#include "eventsender.h"

void handler(Event * event)
{
    qDebug() << "Event";
}

int main(int argc, char *argv[])
{
    QString str = "test";
    EventSender eventSender;
    EventController::addEventListener(&eventSender, handler);

    EventController::init();
    EventController::runOnce();
    EventController::sendEvent(Event(&eventSender,(void*)str.data(),str.length()));
    EventController::runOnce();
}
