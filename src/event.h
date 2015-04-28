#ifndef EVENT_H__
#define EVENT_H__

#include <pthread.h>

class EventSender;

class Event
{
	friend class EventController;
public:
	Event(EventSender * sender, void * arg, int argLength);
	Event(const Event & event);
	~Event();
	void * getArg(){return arg;}
	int getArgLength(){return argLength;}
	EventSender * getSender() {return sender;}
	
private:
	void init(EventSender * sender, void * arg, int argLength);
	EventSender * sender;
	char * arg;
	int argLength;
	pthread_mutex_t argMutex;
};

#endif //EVENT_H__
