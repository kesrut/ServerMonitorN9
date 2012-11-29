#include "updateelement.h"

void UpdateElement::start(Host *host)
{
    h = host ;
    updatethread *thread = new updatethread(this) ;
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
    QTimer *timer = host->getTimer() ;
    timer = new QTimer() ;
    host->setTimer(timer);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_timer())) ;
    timer->start(120000);
}

void UpdateElement::update_timer()
{
    updatethread *thread = new updatethread() ;
    h->setThread(thread);
    thread->setHosts(hosts);
    thread->setServerIndex(index) ;
    thread->start();
}

void UpdateElement::updt()
{
    emit update_info();
}
