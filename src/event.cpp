#include <string.h>
#include "event.h"

void Event::init(EventSender * sender, const void * arg, int argLength)
{
	this->sender = sender;
	this->arg = new char[argLength];
    this->listener = 0;
	memcpy(this->arg, arg, argLength);
	this->argLength = argLength;
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
	delete []arg;
}
