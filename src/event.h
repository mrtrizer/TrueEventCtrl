#ifndef EVENT_H__
#define EVENT_H__

#ifndef MONO_THREAD
#include <pthread.h>
#endif

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
    void * getListener(){return listener;}
	
private:
	void init(EventSender * sender, void * arg, int argLength);
	EventSender * sender;
	char * arg;
	int argLength;
    void * listener;
#ifndef MONO_THREAD
	pthread_mutex_t argMutex;
#endif
};

#endif //EVENT_H__
