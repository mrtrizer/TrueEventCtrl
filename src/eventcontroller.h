#ifndef EVENT_CONTROLLER_H__
#define EVENT_CONTROLLER_H__

#ifndef MONO_THREAD
#include <pthread.h>
#include <semaphore.h>
#endif
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
    static void runOnce();
	static void addEventListener(EventSender * sender, void (*handler)(Event *));
	static void sendEvent(const Event & event);
    static void removeEventSender(EventSender * sender);

private:
    class EventListener
    {
    public:
        EventListener(EventSender * sender, void (*handler)(Event *), void *listener = 0):
            handler(handler),sender(sender), listener(listener){}
        void (*handler)(Event *);
        EventSender * sender;
        void * listener;
    };

	EventController();
	EventController(EventController &){}
    ~EventController();
	
	void run_();
    void runOnce_();
    static void removeEventSenderCtrl(EventSender *sender, EventController * ctrl);
    static void sendEventCtrl(const Event & event, EventController * ctrl);
	
#ifndef MONO_THREAD
	sem_t queueSem;
    pthread_mutex_t queueMutex;
#endif

	std::list<EventListener> listeners;
	std::queue<Event> eventQueue;
#ifndef MONO_THREAD
	static std::map<pthread_t, EventController *> ctrls;
	static EventController * getEventCtrl(pthread_t id);
#else
    static EventController * getEventCtrl();
#endif
    static EventController * monoCtrl;
};

#endif //EVENT_CONTROLLER
