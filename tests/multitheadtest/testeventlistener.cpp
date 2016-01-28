#include "testeventlistener.h"
#include <QDebug>

TestEventListener::TestEventListener(int n):n(n)
{

}

TestEventListener::~TestEventListener()
{

}

void TestEventListener::testListener(Event *event)
{
    qDebug() << "Listener" << n << "| Event" << *(int *)event->getArg();
}
