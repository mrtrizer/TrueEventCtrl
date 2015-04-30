#ifndef EVENT_SENDER_H__
#define EVENT_SENDER_H__

#include <list>

#include "event.h"

class EventController;

class EventSender
{
	friend class EventController;
public:
	EventSender(void * parrent = 0);
	~EventSender();
	void sendEvent(void * arg = 0, int argLength = 0);
	void addEventListener(void (*handler)(Event *));
	void * getParrent(){return parrent;}
	
private:
	void * parrent;
};

#define SEND(arg)sendEvent(&arg,sizeof(arg))

#define ADD_LISTENER(name,class_name) static void name ## _(Event * event)\
                       {\
                            ((class_name *)event->getListener())->name(event);\
                        } \
                        void name(Event * event)

#define ADD_SENDER(name) EventSender name

#define LISTENER(name) name ## _


#endif //EVENT_SENDER_H__
