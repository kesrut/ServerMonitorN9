#include "updateelement.h"

void UpdateElement::start(Host *host)
{
    h = host ;
    thread  = new updatethread(this) ;
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
    QTimer *timer = host->getTimer() ;
    //if (timer != NULL) timer->stop(); delete timer ;
    timer = new QTimer() ;
    h->setTimer(timer);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_timer())) ;
    timer->start(50000);
}

void UpdateElement::stopThread()
{
    thread->end();
}

void UpdateElement::update_timer()
{
    thread = new updatethread(this) ;
    h->setThread(thread);
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
}

void UpdateElement::updt()
{
    emit update_info();
}
