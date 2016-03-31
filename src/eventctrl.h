#ifndef EVENT_CONTROLLER_H__
#define EVENT_CONTROLLER_H__

#ifndef MONO_THREAD
#include <pthread.h>
#include <semaphore.h>
#endif
#include <list>
#include <queue>
#include <map>
#include "event.h"

class EventSender;

class EventCtrl
{
	friend class EventSender;
public:
    enum ConnectionType {DIRECT, QUEUE};

	static void run();
    static void runOnce();
	static void addEventListener(EventSender * sender, void(*handler)(Event *), void * listener, ConnectionType connectionType);
	static void sendEvent(const Event & event);
    static void removeEventSender(EventSender * sender);

private:
    class EventListener
    {
    public:
        EventListener(EventSender * sender, void (*handler)(Event *), void *listener = 0, ConnectionType connectionType = QUEUE):
            handler(handler),sender(sender), listener(listener), connectionType(connectionType){}
        void (*handler)(Event *);
        EventSender * sender;
        void * listener;
        ConnectionType connectionType;
    };

    typedef std::list<EventListener> EventListenerList;
    typedef std::queue<Event> EventQueue;
#ifndef MONO_THREAD
	struct ThreadComparerForMap
	{
		bool operator()(const pthread_t x, const pthread_t y)
		{
#ifdef _MSC_VER
            return x.x> y.x;
#else
            return x > y;
#endif
		}
	};

	typedef std::map<pthread_t, EventCtrl *, ThreadComparerForMap> EventCtrlMap;
#endif

    EventCtrl();
    EventCtrl(EventCtrl &){}
    ~EventCtrl();
	
	void run_();
    void runOnce_();
    void send(Event &event);
    int sendDirect(const Event &event);
    static void removeEventSenderCtrl(EventSender *sender, EventCtrl * ctrl);
    static void sendEventCtrl(const Event & event, EventCtrl * ctrl);
	
#ifndef MONO_THREAD
	sem_t queueSem;
    pthread_mutex_t queueMutex;
#endif

    EventListenerList listeners;
    EventQueue eventQueue;
#ifndef MONO_THREAD
    static EventCtrlMap ctrls;
    static EventCtrl * getEventCtrl(pthread_t id);
#else
    static EventCtrl * getEventCtrl();
#endif
    static EventCtrl * monoCtrl;
};

#endif //EVENT_CONTROLLER
