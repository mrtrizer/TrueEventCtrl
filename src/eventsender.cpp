#include "eventsender.h"
#include "eventctrl.h"
#include "event.h"
#include "assert.h"

#ifdef __GXX_RTTI
#include <typeinfo>
#endif

EventSender::EventSender(void * parrent):parrent(parrent)
{
	
}

EventSender::~EventSender()
{
    EventCtrl::removeEventSender(this);
}

void EventSender::sendEvent(const void * arg, int argLength)
{
    EventCtrl::sendEvent(Event(this, arg,  argLength));
}

void EventSender::addEventListener(void * listener, void (*handler)(Event *))
{
#ifdef __GXX_RTTI
    assert(typeid(this).hash_code() == typeid(EventSender*).hash_code());
#endif

    EventCtrl::addEventListener(this, handler, listener);
}
