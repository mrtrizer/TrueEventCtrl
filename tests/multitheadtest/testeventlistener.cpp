#include "testeventlistener.h"
#include <QDebug>

TestEventListener::TestEventListener()
{

}

TestEventListener::~TestEventListener()
{

}

void TestEventListener::testListener(Event *event)
{
    qDebug() << "Event";
    qDebug() << *(int *)event->getArg();
}
