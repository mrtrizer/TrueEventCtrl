#include "eventsender.h"
#include "eventcontroller.h"
#include "event.h"

EventSender::EventSender(void * parrent):parrent(parrent)
{
	
}

EventSender::~EventSender()
{
    EventController::removeEventSender(this);
}

void EventSender::sendEvent(void * arg, int argLength)
{
	EventController::sendEvent(Event(this, arg,  argLength));
}

void EventSender::addEventListener(void (*handler)(Event *))
{
	EventController::addEventListener(this, handler);
}
