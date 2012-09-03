#ifndef UPDATEELEMENT_H
#define UPDATEELEMENT_H

#include <QObject>
#include <QTimer>
#include "libssh2.h"
#include <QDateTime>
#include <QList>
#include "host.h"
#include "updatethread.h"

class UpdateElement : public QObject
{
    Q_OBJECT
public:

    UpdateElement(QObject *parent, int v, QList <Host*> h) : QObject(parent)
    {
        index = v ;
        hosts = h ;
        connect(this, SIGNAL(update_info()), parent, SLOT(update_info())) ;
    }
    void start(Host *host) ;
private slots:
    void update_timer() ;
    void updt() ;
signals:
    void update_info() ;
private:
    Host *h ;
    int index;
    QList <Host*> hosts ;
};

#endif // UPDATEELEMENT_H
