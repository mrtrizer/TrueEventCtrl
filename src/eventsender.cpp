#include "eventsender.h"
#include "eventctrl.h"
#include "event.h"

EventSender::EventSender(void * parrent):parrent(parrent)
{
	
}

EventSender::~EventSender()
{
    EventCtrl::removeEventSender(this);
}

void EventSender::sendEvent(void * arg, int argLength)
{
    EventCtrl::sendEvent(Event(this, arg,  argLength));
}

void EventSender::addEventListener(void * listener, void (*handler)(Event *))
{
    EventCtrl::addEventListener(this, handler, listener);
}
