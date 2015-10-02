#include <string.h>
#include "event.h"

void Event::init(EventSender * sender, const void * arg, int argLength)
{
#ifndef MONO_THREAD
	pthread_mutex_init(&argMutex, NULL);
	pthread_mutex_lock(&argMutex); //critical section begin
#endif
	this->sender = sender;
	this->arg = new char[argLength];
    this->listener = 0;
	memcpy(this->arg, arg, argLength);
	this->argLength = argLength;
#ifndef MONO_THREAD
	pthread_mutex_unlock(&argMutex); //critical section end
#endif
}

Event::Event(EventSender * sender, const void * arg, int argLength)
{
	init(sender, arg, argLength);
}

Event::Event(const Event & event)
{
	init(event.sender, event.arg, event.argLength);
}

Event::~Event()
{
#ifndef MONO_THREAD
	pthread_mutex_lock(&argMutex); //critical section begin
#endif
	delete []arg;
#ifndef MONO_THREAD
	pthread_mutex_unlock(&argMutex); //critical section end
	pthread_mutex_destroy(&argMutex);
#endif
}
