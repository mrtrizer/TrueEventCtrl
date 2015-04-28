#include <string.h>
#include "event.h"

void Event::init(EventSender * sender, void * arg, int argLength)
{
	pthread_mutex_init(&argMutex, NULL);
	pthread_mutex_lock(&argMutex); //critical section begin
	this->sender = sender;
	this->arg = new char[argLength];
	memcpy(this->arg, arg, argLength);
	this->argLength = argLength;
	pthread_mutex_unlock(&argMutex); //critical section end
}

Event::Event(EventSender * sender, void * arg, int argLength)
{
	init(sender, arg, argLength);
}

Event::Event(const Event & event)
{
	init(event.sender, event.arg, event.argLength);
}

Event::~Event()
{
	pthread_mutex_lock(&argMutex); //critical section begin
	delete []arg;
	pthread_mutex_unlock(&argMutex); //critical section end
	pthread_mutex_destroy(&argMutex);
}
