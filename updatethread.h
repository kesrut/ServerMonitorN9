#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>
#include <QObject>
#include "libssh2.h"
#include <QList>
#include <qtcpsocket.h>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QTime>
#include "host.h"
#include "updateelement.h"
#include <QDesktopServices>

#ifndef Q_WS_SIMULATOR

#ifndef AEGIS
#define AEGIS
#include "aegis_common.h"
#include "aegis_crypto.h"
#endif

#endif


class updatethread : public QThread
{
    Q_OBJECT
protected:
    void run() ;
public:
    updatethread();
    updatethread(QObject *parent) ;
    void setHosts(QList<Host*> &h) ;
    void setServerIndex(int i) ;
    QString execute(QString cmd) ;
signals:
    void update() ;
private:
    QList <Host*> hosts;
    int index ;
    QTcpSocket *socket ;
    int sock ;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    LIBSSH2_KNOWNHOSTS *nh;
    const char *fingerprint;
    int rc ;
    size_t len;
    int type;
    int exitcode;
    int bytecount;
    QString data ;
};

#endif // UPDATETHREAD_H
