#include "testeventlistener2.h"
#include <QDebug>

TestEventListener2::TestEventListener2(int n):TestEventListener(n)
{

}

TestEventListener2::~TestEventListener2()
{

}

void TestEventListener2::abatractTestListener(Event *event)
{
    (void)event;
    qDebug() << "Abstract method";
}
