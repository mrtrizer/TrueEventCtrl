#ifndef EVENT_SENDER_H__
#define EVENT_SENDER_H__

#include <list>

#include "event.h"
#include "eventctrl.h"

class EventSender
{
    friend class EventCtrl;
public:
	EventSender(void * parrent = 0);
	~EventSender();
    void sendEvent(const void * arg = 0, int argLength = 0);
    void addEventListener(void *listener, void (*handler)(Event *), EventCtrl::ConnectionType connectionType = EventCtrl::QUEUE);
	void * getParrent(){return parrent;}
	
private:
	void * parrent;
};

#define SEND(arg)sendEvent(&arg,sizeof(arg))

#define ADD_LISTENER(name,class_name) static void name ## _(Event * event)\
                       {\
                            ((class_name *)event->getListener())->name(event);\
                        }

#define ADD_SENDER(name) EventSender name

#define LISTENER(name) name ## _


#endif //EVENT_SENDER_H__
