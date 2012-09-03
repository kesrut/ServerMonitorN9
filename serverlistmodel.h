#ifndef SERVERLISTMODEL_H
#define SERVERLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QList>
#include "host.h"
#include <QThread>
#include <QFile>
#include <QDir>
#include <QString>
#include "sshsocket.h"
#include <QDeclarativeContext>
#include <libssh2.h>
#include <QDesktopServices>
#ifndef Q_WS_SIMULATOR
#ifndef AEGIS
#define AEGIS
#include "aegis_common.h"
#include "aegis_crypto.h"
#endif
#endif
#include "update.h"

class update ;

class ServerListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles {
            NameRole = Qt::UserRole + 1,
            MessageRole,
            ErrorRole
        };
    ServerListModel(QAbstractListModel *parent = 0) : QAbstractListModel(parent)
    {
        servers = QList<Host*>() ;
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[MessageRole] = "status" ;
        roles[ErrorRole] = "error" ;
        setRoleNames(roles);
        QFile file(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "servers.db") ;
        file.open(QFile::ReadOnly) ;
        QDataStream stream(&file) ;
        stream.setVersion(QDataStream::Qt_4_7);
        int count = 0, i=0 ;
        stream >> count ;
        for (i=0; i < count; i++)
        {
            QString title ;
            QString hostname ;
            QString username ;
            qint32 port ;
            qint32 password_id ;
            QString password ;
            QByteArray data ;
#ifdef Q_WS_SIMULATOR
            stream >> title >> hostname >> username >> port >> password ;
#else
            stream >> title >> hostname >> username >> port >> data ;
#endif
            Host *host ;
            host = new Host(title, hostname, username, port);
#ifdef Q_WS_SIMULATOR
            host->setPassword(password) ;
#else
            host->setSecret(data);
#endif
            servers.append(host);
        }
        file.close();

        emit countChanged();
    }
    void setContext(QDeclarativeContext *context) { _context = context ; }
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role) const;
    void save();
    inline int count() const { return servers.count(); }
    Q_INVOKABLE QObject* getModel() ;
    Q_INVOKABLE void insert(QString title, QString hostname, QString username, QString password, qint32 port ) ;
    Q_INVOKABLE QString getServerTitle(qint32 index) ;
    Q_INVOKABLE QString getHostname(qint32 index) ;
    Q_INVOKABLE QString getUsername(qint32 index) ;
    Q_INVOKABLE QString getPort(qint32 index) ;
    Q_INVOKABLE QString getPassword(qint32 index) ;
    Q_INVOKABLE void deleteServerWithIndex(qint32 index) ;
    Q_INVOKABLE QList<Host*> &getServers() ;
    Q_INVOKABLE int insertHost(QObject *obj) ;
    Q_INVOKABLE int editHost(QObject *obj, qint32 index) ;
    Q_INVOKABLE QString getUptime(int index) ;
    Q_INVOKABLE QString getMemTotal(int index) ;
    Q_INVOKABLE QString getFreeMem(int index) ;
    Q_INVOKABLE QString getAvg1(int index) ;
    Q_INVOKABLE QString getAvg2(int index) ;
    Q_INVOKABLE QString getAvg3(int index) ;
    Q_INVOKABLE void refresh() ;
    void setUpdt(update *upd) { updt = upd ; }
private slots:
    void credentialsStored(const quint32 id) ;
    void update_table() ;
signals:
    void countChanged();
    void layoutChanged();
    void layoutAboutToBeChanged();

private:
    QList <Host*> servers ;
    QDeclarativeContext *_context ;
    update *updt ;
};

#endif // SERVERLISTMODEL_H
