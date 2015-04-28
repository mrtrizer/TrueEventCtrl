#include <pthread.h>

#include "eventcontroller.h"
#include "event.h"

std::map<pthread_t, EventController *> EventController::ctrls;

EventController::EventController()
{
	sem_init(&queueSem, 0, 0);
    pthread_mutex_init(&queueMutex, NULL);
}

EventController::~EventController()
{
    sem_destroy(&queueSem);
    pthread_mutex_destroy(&queueMutex);
}

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

void EventController::addEventListener(EventSender * sender, void (*handler) (Event * event))
{
	EventController * ctrl = getEventCtrl(pthread_self());
	std::list<EventListener>::iterator i;
	for (i = ctrl->listeners.begin(); i != ctrl->listeners.end(); i++)
        if ((i->sender == sender) && (i->handler == handler))
			return;
	ctrl->listeners.push_back(EventListener(sender, handler));
}

void EventController::removeEventSender(EventSender * sender)
{
    std::map<pthread_t, EventController *>::iterator ctrl;
    std::list<EventListener>::iterator i;
    for (ctrl = ctrls.begin(); ctrl != ctrls.end(); ctrl++)
    {
        i = ctrl->second->listeners.begin();
        while (i != ctrl->second->listeners.end())
        {
            if (i->sender == sender)
                i = ctrl->second->listeners.erase(i);
            else
                i++;
        }
    }
}

void EventController::sendEvent(const Event & event)
{
	std::map<pthread_t, EventController *>::iterator i;
	for (i = ctrls.begin(); i != ctrls.end(); i++)
	{
        pthread_mutex_lock(&i->second->queueMutex);
		i->second->eventQueue.push(event);
        pthread_mutex_unlock(&i->second->queueMutex);
		sem_post(&i->second->queueSem);
	}
}

void EventController::run()
{
	EventController * ctrl = getEventCtrl(pthread_self());
	ctrl->run_();
}

void EventController::run_()
{
    std::list<EventListener>::iterator i;
	while (1)
	{
        std::list<EventListener> handlerList;
		sem_wait(&queueSem);
        if (eventQueue.size() == 0)
            continue;
        pthread_mutex_lock(&queueMutex);
        Event event = eventQueue.front();
        eventQueue.pop();
        pthread_mutex_unlock(&queueMutex);
        for (i = listeners.begin(); i != listeners.end(); i++)
            if (i->sender == event.sender)
                handlerList.push_back(*i);
        for (i = handlerList.begin(); i != handlerList.end(); i++)
            i->handler(&event);

	}
}
