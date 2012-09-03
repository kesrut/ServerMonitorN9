#ifndef SSHSOCKET_H
#define SSHSOCKET_H

#include <QObject>
#include "host.h"
#include <QTcpSocket>
#include <QList>
#include <libssh2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <QDir>
#include <QDesktopServices>
#ifndef Q_WS_SIMULATOR
#ifndef AEGIS
#define AEGIS
#include "aegis_common.h"
#include "aegis_crypto.h"
#endif
#endif

class sshsocket : public QObject
{
    Q_OBJECT
public:
    explicit sshsocket(QObject *parent = 0) ;
    Q_INVOKABLE void AcceptHost(QString title, QString hostname, QString username, QString password, qint32 port ) ;
    Q_INVOKABLE void EditHost(QString title, QString hostname, QString username, QString password, qint32 port ) ;
    Q_INVOKABLE QString connectErrorMessage() ;
    Q_INVOKABLE void Rejected() ;
    Q_INVOKABLE void writeKnowHost() ;
    Q_INVOKABLE QObject* getHost() ;
    Q_INVOKABLE void setMode(int mode) ;
    Q_INVOKABLE void setIndex(qint32 index) ;
    Q_INVOKABLE int getIndex() ;
private slots:
    void socket_connected_slot() ;
    void display_error(QAbstractSocket::SocketError socketError) ;
signals:
    void socketConnected() ;
    void errorConnecting() ;
    void error(QString errorMessage) ;
    void accept_fingerprint(QString message) ;
    void insertHost() ;
    void accept_fingerprint_edit(QString message) ;
    void insertHost_edit() ;

private:
    QTcpSocket *socket ;
    Host *host ;
    QString *connectError ;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int sock ;
    int rc ;
    LIBSSH2_KNOWNHOSTS *nh;
    const char *fingerprint;
    size_t len;
    int type;
    int mode ;
    int index ;
};

#endif // SSHSOCKET_H
