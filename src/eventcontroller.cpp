#ifndef MONO_THREAD
#include <pthread.h>
#endif
#include <assert.h>

#include "eventcontroller.h"
#include "event.h"

EventController * EventController::monoCtrl = 0;

#ifndef MONO_THREAD
std::map<pthread_t, EventController *> EventController::ctrls;
#endif

EventController::EventController()
{
#ifndef MONO_THREAD
    sem_init(&queueSem, 0, 0);
    pthread_mutex_init(&queueMutex, NULL);
#endif
}

EventController::~EventController()
{
#ifndef MONO_THREAD
    sem_destroy(&queueSem);
    pthread_mutex_destroy(&queueMutex);
#endif
}

#ifndef MONO_THREAD
EventController * EventController::getEventCtrl(pthread_t id)
{
	EventController * ctrl;
	std::map<pthread_t,EventController *>::iterator i;
	
	if ((i = ctrls.find(id)) != ctrls.end())
		ctrl = i->second;
	else
		ctrl = ctrls[id] = new EventController();
		
	return ctrl;	
}

#else

EventController * EventController::getEventCtrl()
{
    if (monoCtrl == 0)
        monoCtrl = new EventController();
    return monoCtrl;
}

#endif

void EventController::addEventListener(EventSender * sender, void (*handler) (Event * event))
{
    EventController * ctrl;
#ifndef MONO_THREAD
    ctrl = getEventCtrl(pthread_self());
#else
    ctrl = getEventCtrl();
#endif
	std::list<EventListener>::iterator i;
	for (i = ctrl->listeners.begin(); i != ctrl->listeners.end(); i++)
        if ((i->sender == sender) && (i->handler == handler))
			return;
	ctrl->listeners.push_back(EventListener(sender, handler));
}

void EventController::removeEventSender(EventSender * sender)
{
#ifndef MONO_THREAD
    std::map<pthread_t, EventController *>::iterator ctrl;

    for (ctrl = ctrls.begin(); ctrl != ctrls.end(); ctrl++)
    {
        removeEventSenderCtrl(sender,ctrl->second);
    }
#else
    removeEventSenderCtrl(sender,getEventCtrl());
#endif
}

void EventController::removeEventSenderCtrl(EventSender * sender,EventController * ctrl)
{
    std::list<EventListener>::iterator i;
    i = ctrl->listeners.begin();
    while (i != ctrl->listeners.end())
    {
        if (i->sender == sender)
            i = ctrl->listeners.erase(i);
        else
            i++;
    }
}

void EventController::sendEvent(const Event & event)
{
#ifndef MONO_THREAD
	std::map<pthread_t, EventController *>::iterator i;
	for (i = ctrls.begin(); i != ctrls.end(); i++)
	{

        pthread_mutex_lock(&i->second->queueMutex);
        sendEventCtrl(event,i->second);
        pthread_mutex_unlock(&i->second->queueMutex);
		sem_post(&i->second->queueSem);
    }
#else
    sendEventCtrl(event,getEventCtrl());
#endif
}

void EventController::sendEventCtrl(const Event & event, EventController * ctrl)
{
    ctrl->eventQueue.push(event);
}

void EventController::run()
{ 
#ifndef MONO_THREAD
    getEventCtrl(pthread_self())->run_();
#else
    getEventCtrl()->run_();
#endif
}

void EventController::runOnce()
{
#ifndef MONO_THREAD
    getEventCtrl(pthread_self())->runOnce_();
#else
    getEventCtrl()->runOnce_();
#endif
}

void EventController::run_()
{
    std::list<EventListener>::iterator i;
    while (1)
    {
        std::list<EventListener> handlerList;
#ifndef MONO_THREAD
        sem_wait(&queueSem);
#endif
        if (eventQueue.size() == 0)
            continue;
#ifndef MONO_THREAD
        pthread_mutex_lock(&queueMutex);
#endif
        Event event = eventQueue.front();
        eventQueue.pop();
#ifndef MONO_THREAD
        pthread_mutex_unlock(&queueMutex);
#endif
        for (i = listeners.begin(); i != listeners.end(); i++)
            if (i->sender == event.sender)
                handlerList.push_back(*i);
        for (i = handlerList.begin(); i != handlerList.end(); i++)
        {
            event.listener = i->listener;
            i->handler(&event);
        }

    }
}

void EventController::runOnce_()
{
    std::list<EventListener>::iterator i;
#ifndef MONO_THREAD
    sem_wait(&queueSem);
#endif
    if (eventQueue.size() == 0)
        return;
#ifndef MONO_THREAD
    pthread_mutex_lock(&queueMutex);
#endif
    while (eventQueue.size() > 0)
    {
        std::list<EventListener> handlerList;
        Event event = eventQueue.front();
        eventQueue.pop();
#ifndef MONO_THREAD
        pthread_mutex_unlock(&queueMutex);
#endif
        for (i = listeners.begin(); i != listeners.end(); i++)
            if (i->sender == event.sender)
                handlerList.push_back(*i);
        for (i = handlerList.begin(); i != handlerList.end(); i++)
        {
            event.listener = i->listener;
            i->handler(&event);
        }
    }
}
