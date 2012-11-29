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
    ServerListModel() ;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role) const;
    void save();
    inline int count() const { return servers->count(); }
    Q_INVOKABLE QObject* getModel() ;
    Q_INVOKABLE void insert(QString title, QString hostname, QString username, QString password, qint32 port ) ;
    Q_INVOKABLE QString getServerTitle(qint32 index) ;
    Q_INVOKABLE QString getHostname(qint32 index) ;
    Q_INVOKABLE QString getUsername(qint32 index) ;
    Q_INVOKABLE QString getPort(qint32 index) ;
    Q_INVOKABLE QString getPassword(qint32 index) ;
    void new_updt(int index) ;
    Q_INVOKABLE void deleteServerWithIndex(qint32 index) ;
    Q_INVOKABLE QList<Host*> *getServers() ;
    int insertHost(Host *obj) ;
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
    QList <Host*> *servers ;
    update *updt ;
};

#endif // SERVERLISTMODEL_H
