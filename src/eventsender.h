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

#endif //EVENT_SENDER_H__
