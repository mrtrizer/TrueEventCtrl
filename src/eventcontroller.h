#ifndef EVENT_CONTROLLER_H__
#define EVENT_CONTROLLER_H__

#include <pthread.h>
#include <semaphore.h>
#include <list>
#include <queue>
#include <map>

class EventSender;
class Event;

class EventController
{
	friend class EventSender;
public:
	static void run();
	static void addEventListener(EventSender * sender, void (*handler)(Event *));
	static void sendEvent(const Event & event);
    static void removeEventSender(EventSender * sender);

private:
	class EventListener
	{
	public:
		EventListener(EventSender * sender, void (*handler)(Event *)):handler(handler),sender(sender){}
		void (*handler)(Event *);
		EventSender * sender;
	};

	EventController();
	EventController(EventController &){}
    ~EventController();
	
	void run_();
	
	sem_t queueSem;
    pthread_mutex_t queueMutex;

	std::list<EventListener> listeners;
	std::queue<Event> eventQueue;
	static std::map<pthread_t, EventController *> ctrls;
	static EventController * getEventCtrl(pthread_t id);
};

#endif //EVENT_CONTROLLER
