#include "updateelement.h"


void UpdateElement::start(Host *host)
{
    updatethread *thread = new updatethread(this) ;
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
    QTimer *timer = host->getTimer() ;
    timer = new QTimer(this) ;
    host->setTimer(timer);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_timer())) ;
    timer->start(120000);
    h = host ;
}

void UpdateElement::update_timer()
{
    updatethread *thread = new updatethread(this) ;
    h->setThread(thread);
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
}

void UpdateElement::updt()
{
    emit update_info();
}
