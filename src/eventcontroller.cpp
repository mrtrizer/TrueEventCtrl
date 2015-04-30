#ifndef MONO_THREAD
#include <pthread.h>
#endif
#include <assert.h>

#include "eventcontroller.h"
#include "event.h"

EventCtrl * EventCtrl::monoCtrl = 0;

#ifndef MONO_THREAD
std::map<pthread_t, EventCtrl *> EventCtrl::ctrls;
#endif

EventCtrl::EventCtrl()
{
#ifndef MONO_THREAD
    sem_init(&queueSem, 0, 0);
    pthread_mutex_init(&queueMutex, NULL);
#endif
}

EventCtrl::~EventCtrl()
{
#ifndef MONO_THREAD
    sem_destroy(&queueSem);
    pthread_mutex_destroy(&queueMutex);
#endif
}

#ifndef MONO_THREAD
EventCtrl * EventCtrl::getEventCtrl(pthread_t id)
{
    EventCtrl * ctrl;
    std::map<pthread_t,EventCtrl *>::iterator i;
	
	if ((i = ctrls.find(id)) != ctrls.end())
		ctrl = i->second;
	else
        ctrl = ctrls[id] = new EventCtrl();
		
	return ctrl;	
}

#else

EventCtrl * EventCtrl::getEventCtrl()
{
    if (monoCtrl == 0)
        monoCtrl = new EventCtrl();
    return monoCtrl;
}

#endif

void EventCtrl::addEventListener(EventSender * sender, void (*handler) (Event * event), void * listener)
{
    EventCtrl * ctrl;
#ifndef MONO_THREAD
    ctrl = getEventCtrl(pthread_self());
#else
    ctrl = getEventCtrl();
#endif
    std::list<EventListener>::iterator i;
    for (i = ctrl->listeners.begin(); i != ctrl->listeners.end(); i++)
        if ((i->sender == sender) && (i->handler == handler))
            return;
    ctrl->listeners.push_back(EventListener(sender, handler, listener));
}

void EventCtrl::removeEventSender(EventSender * sender)
{
#ifndef MONO_THREAD
    std::map<pthread_t, EventCtrl *>::iterator ctrl;

    for (ctrl = ctrls.begin(); ctrl != ctrls.end(); ctrl++)
    {
        removeEventSenderCtrl(sender,ctrl->second);
    }
#else
    removeEventSenderCtrl(sender,getEventCtrl());
#endif
}

void EventCtrl::removeEventSenderCtrl(EventSender * sender,EventCtrl * ctrl)
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

void EventCtrl::sendEvent(const Event & event)
{
#ifndef MONO_THREAD
    std::map<pthread_t, EventCtrl *>::iterator i;
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

void EventCtrl::sendEventCtrl(const Event & event, EventCtrl * ctrl)
{
    ctrl->eventQueue.push(event);
}

void EventCtrl::run()
{ 
#ifndef MONO_THREAD
    getEventCtrl(pthread_self())->run_();
#else
    getEventCtrl()->run_();
#endif
}

void EventCtrl::runOnce()
{
#ifndef MONO_THREAD
    getEventCtrl(pthread_self())->runOnce_();
#else
    getEventCtrl()->runOnce_();
#endif
}

void EventCtrl::run_()
{
    while (1)
    {
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
        send(event);

    }
}

void EventCtrl::runOnce_()
{

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

        Event event = eventQueue.front();
        eventQueue.pop();
#ifndef MONO_THREAD
        pthread_mutex_unlock(&queueMutex);
#endif
        send(event);
    }
}

void EventCtrl::send(Event & event)
{
    HandlerList handlerList;
    HandlerList::iterator i;
    for (i = listeners.begin(); i != listeners.end(); i++)
        if (i->sender == event.sender)
            handlerList.push_back(*i);
    for (i = handlerList.begin(); i != handlerList.end(); i++)
    {
        event.listener = i->listener;
        i->handler(&event);
    }
}
