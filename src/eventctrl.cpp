#ifndef MONO_THREAD
#include <pthread.h>
#endif
#include <assert.h>

#include "eventctrl.h"
#include "event.h"

EventCtrl * EventCtrl::monoCtrl = 0;

#ifndef MONO_THREAD
EventCtrl::EventCtrlMap EventCtrl::ctrls;
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
    EventCtrlMap::iterator i;
	
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

void EventCtrl::addEventListener(EventSender * sender, void (*handler) (Event * event), void * listener, ConnectionType connectionType)
{
    EventCtrl * ctrl;
#ifndef MONO_THREAD
    ctrl = getEventCtrl(pthread_self());
#else
    ctrl = getEventCtrl();
#endif
    EventListenerList::iterator i;
    for (i = ctrl->listeners.begin(); i != ctrl->listeners.end(); i++)
        if ((i->sender == sender) && (i->handler == handler))
            return;
    ctrl->listeners.push_back(EventListener(sender, handler, listener, connectionType));
}

void EventCtrl::removeEventSender(EventSender * sender)
{
#ifndef MONO_THREAD
    EventCtrlMap::iterator ctrl;

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
    EventListenerList::iterator i;
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
    EventCtrlMap::iterator i;
	for (i = ctrls.begin(); i != ctrls.end(); i++)
	{
        if (i->second->sendDirect(event) == 0)
            continue;
        pthread_mutex_lock(&i->second->queueMutex);
        sendEventCtrl(event,i->second);
        pthread_mutex_unlock(&i->second->queueMutex);
		sem_post(&i->second->queueSem);
    }
#else
    getEventCtrl()->sendDirect(event);
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
    EventListenerList handlerList;
    EventListenerList::iterator i;
    for (i = listeners.begin(); i != listeners.end(); i++)
        if (i->sender == event.sender)
            handlerList.push_back(*i);
    for (i = handlerList.begin(); i != handlerList.end(); i++)
    {
        event.listener = i->listener;
        i->handler(&event);
    }
}

int EventCtrl::sendDirect(const Event & event)
{
    EventListenerList handlerList;
    EventListenerList::iterator i;
    int notDirectCounter = 0;
    for (i = listeners.begin(); i != listeners.end(); i++)
        if ((i->sender == event.sender))
        {
            if (i->connectionType == DIRECT)
                handlerList.push_back(*i);
            else
                notDirectCounter++;
        }
    if (handlerList.size() != 0)
        for (i = handlerList.begin(); i != handlerList.end(); i++)
        {
            Event eventTmp(event);
            eventTmp.listener = i->listener;
            i->handler(&eventTmp);
        }
    return notDirectCounter;
}
