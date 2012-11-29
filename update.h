#ifndef UPDATE_H
#define UPDATE_H
#include <QObject>
#include <QList>
#include "host.h"
#include <qdebug.h>
#include <QString>
#include "serverlistmodel.h"
#include "updateelement.h"

class ServerListModel ;

class update : public QObject
{
    Q_OBJECT
public:
    update (QObject *parent = 0 ) : QObject(parent)
    {
    }
    void getServers(QList <Host*> *h) ;
    void getModel(ServerListModel *m) ;
    void setServers(QList <Host*> *h)
    {
        hosts = h ;
    }
    Q_INVOKABLE void start(int i) ;
private slots:
    void update_info() ;
signals:
    void update_tbl() ;
private:
    QList <Host*> *hosts ;
    ServerListModel *model ;
};

#endif // UPDATE_H
