#include "serverlistmodel.h"
#include <QDebug>


int ServerListModel::rowCount(const QModelIndex & parent) const {
     return count();
 }

 QVariant ServerListModel::data(const QModelIndex & index, int role) const {
     if (index.row() < 0 || index.row() > servers.count())
         return QVariant() ;
     Host *host = servers[index.row()];
     if (role == NameRole)
     {
         return host->title() ;
     }
     if (role == MessageRole)
     {
         return host->getMessage() ;
     }
     if (role == ErrorRole)
     {
        return host->isError() ;
     }
     return QVariant();
 }

QObject* ServerListModel::getModel()
 {
    return this ;
 }

int ServerListModel::editHost(QObject *obj, qint32 index)
{
    Host *host = (Host*)obj ;
    QTimer *timer = servers.at(index)->getTimer() ;
    if (timer != NULL)
    {
        if (timer->isActive())
        {
            timer->stop() ;
            delete timer ;
        }
    }
    QThread *thread = (QThread*)host->getThread() ;
    if (thread != NULL)
    {
        if (thread->isRunning())
        {
            thread->exit();
        }
    }
    servers.replace(index, host);
    updt->setServes(servers);
    updt->start(index);
    emit countChanged();
    save() ;
    return index ;
}

void ServerListModel::refresh()
{
    int i=0 ;
    for (i=0; i < servers.count(); i++)
    {
        QTimer *timer = servers.at(i)->getTimer() ;
        if (timer != NULL)
        {
            if (timer->isActive())
            {
                timer->stop() ;
                delete timer ;
            }
        }
        Host *h = servers.at(i) ;
        QThread *thread = (QThread*)h->getThread() ;
        if (thread != NULL)
        {
            if (thread->isRunning())
            {
                thread->exit();
            }
        }
        updt->setServes(servers);
        updt->start(i);
    }
}

int ServerListModel::insertHost(QObject *obj)
{
    Host *host = (Host*)obj ;
    beginInsertRows(QModelIndex(), servers.count(), servers.count());
    servers.append(host);
    save() ;
    endInsertRows();
    //delete host ;
    int k = (servers.count() - 1) ;
    updt->setServes(servers);
    updt->start(k);
    emit countChanged();

    return (servers.count() - 1) ;
}

void ServerListModel::insert(QString title, QString hostname, QString username, QString password, qint32 port )
 {
     Host *host = new Host(title, hostname, username, port) ;
     beginInsertRows(QModelIndex(), servers.count(), servers.count());
     servers.append(host);
     endInsertRows();
     save() ;
     emit countChanged();
 }

QString ServerListModel::getServerTitle(qint32 index)
 {
    if (count() > 0)
    return servers.at(index)->title() ;
    else return "" ;
 }

QString ServerListModel::getHostname(qint32 index)
{
    if (count() > 0)
    return servers.at(index)->hostname() ;
    else return "" ;
}

QString ServerListModel::getUsername(qint32 index)
{
    if (count() > 0)
    return servers.at(index)->username() ;
    else return "" ;
}

QString ServerListModel::getPort(qint32 index)
{
    if (count() > 0)
    return QString::number(servers.at(index)->port()) ;
    else return "" ;
}

QString ServerListModel::getPassword(qint32 index)
{
#ifdef Q_WS_SIMULATOR
        QString password ;
        password = servers.at(index)->password() ;
#else
        RAWDATA_PTR clearData;
        size_t length = 0;
        QByteArray secret;
        secret.append(servers.at(index)->getSecret());
        aegis_crypto_decrypt(secret.data(), secret.length(), NULL, &clearData, &length) ;
        QByteArray decrypted((char *)clearData, length);
        aegis_crypto_free(clearData);
        QString password(decrypted) ;
#endif
        if (count() > 0)
        return password;
        else return "" ;
}

 void ServerListModel::credentialsStored(const quint32 id)
 {
    int i = servers.count() - 1 ;
    servers.at(i)->setPasswordId(id);
    save();
 }

 void ServerListModel::deleteServerWithIndex(qint32 index)
 {
    LIBSSH2_SESSION *session = libssh2_session_init() ;
    LIBSSH2_KNOWNHOSTS *nh ;
    nh = libssh2_knownhost_init(session) ;
    QString known_host_dir  = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "known_hosts" ;
    libssh2_knownhost_readfile(nh, known_host_dir.toAscii(), LIBSSH2_KNOWNHOST_FILE_OPENSSH) ;
    struct libssh2_knownhost *kh;
    struct libssh2_knownhost *kh2=NULL;
    while (!libssh2_knownhost_get(nh, &kh, kh2))
    {
        kh2 = kh ;
        if (strcmp(kh2->name, servers.at(index)->hostname().toAscii()) == 0)
        {
            libssh2_knownhost_del(nh, kh2) ;
            break ;
        }
    }
    libssh2_knownhost_writefile(nh, known_host_dir.toAscii(),LIBSSH2_KNOWNHOST_FILE_OPENSSH) ;
    libssh2_knownhost_free(nh) ;
    QTimer *timer = servers.at(index)->getTimer() ;
    if (timer != NULL)
    {
        if (timer->isActive())
        {
            timer->stop() ;
            delete timer ;
        }
    }
#ifdef Q_WS_SIMULATOR
     beginRemoveRows(QModelIndex(), index, index);
     servers.removeAt(index) ;
     endRemoveRows();
     save() ;
#else
    beginRemoveRows(QModelIndex(), index, index);
    Host *h = servers.at(index) ;
    QThread *thread = (QThread*)h->getThread() ;
    if (thread != NULL)
    {
        if (thread->isRunning())
        {
            thread->exit();
        }
    }
    delete h ;
    servers.removeAt(index) ;
    endRemoveRows();
    save() ;
#endif
 }

void ServerListModel::save()
{
     QFile file(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "servers.db") ;
     file.open(QFile::WriteOnly) ;
     QDataStream stream(&file) ;
     stream.setVersion(QDataStream::Qt_4_7);
     int i=0 ;
     stream << servers.count() ;
     for (i=0; i < servers.count(); i++)
     {
        Host *host = servers.at(i) ;
#ifdef Q_WS_SIMULATOR
        stream << host->title() << host->hostname() << host->username() << host->port() << host->password() ;
#else
        stream << host->title() << host->hostname() << host->username() << host->port() << host->getSecret() ;
#endif
     }
     file.close();
}

QList<Host*> & ServerListModel::getServers()
{
    return servers;
}

void ServerListModel::update_table()
{
    emit layoutChanged();
}

QString ServerListModel::getUptime(int index)
{
   Host *host = servers.at(index) ;
   long value = host->getUptime() ;
   if (host->isError())
   {
       return "N/A" ;
   }
   if (value != 0)
   {
       long days = value / 86400 ;
       value -= (days * 86400) ;
       int hours = value / 3600 ;
       value -= (hours * 3600) ;
       int min = value / 60 ;
       QString up ;
       QString tmp ;
       if (days < 1) up.append("") ;
       else up.append(tmp.sprintf("%dd ", days )) ;
       tmp.clear();
       if (hours < 1) up.append("") ;
       else up.append(tmp.sprintf("%dh ", hours)) ;
       tmp.clear();
       if (min < 1) up.append("") ;
       else up.append(tmp.sprintf("%dm", min)) ;
       tmp.clear();
       if (days < 1 && hours < 1 && min < 1) up.append(tmp.sprintf("%ds", value)) ;
       return up ;
   }
   else
   {
       return "N/A" ;
   }
}

QString ServerListModel::getMemTotal(int index)
{
    Host *host = servers.at(index) ;
    long value = host->getMemTotal() ;
    if (host->isError())
    {
        return "N/A" ;
    }
    if (value != 0)
    {
        float mem = value / 1024.0 ;
        if (mem > 1024)
        {
            QString total ;
            QString tmp ;
            mem = mem / 1024.0 ;
            total.append(tmp.sprintf("%.2f GB", mem)) ;
            return total ;
        }
        else
        {
            QString total ;
            QString tmp ;
            total.append(tmp.sprintf("%0.0f MB", mem)) ;
            return total ;
        }
    }
    else
    {
        return "N/A" ;
    }
}

QString ServerListModel::getFreeMem(int index)
{
    Host *host = servers.at(index) ;
    long value = host->getMemFree() ;
    if (host->isError())
    {
        return "N/A" ;
    }
    if (value != 0)
    {
        float mem = value / 1024.0 ;
        if (mem > 1024)
        {
            QString total ;
            QString tmp ;
            mem = mem / 1024.0 ;
            total.append(tmp.sprintf("%.2f GB", mem)) ;
            return total ;
        }
        else
        {
            QString total ;
            QString tmp ;
            total.append(tmp.sprintf("%0.0f MB", mem)) ;
            return total ;
        }
    }
    else
    {
        return "N/A" ;
    }
}

QString ServerListModel::getAvg1(int index)
{
    Host *host = servers.at(index) ;
    if (host->isError())
    {
        return "N/A" ;
    }
    float value = host->getLoadAvg1();
    QString total ;
    QString tmp ;
    total.append(tmp.sprintf("%.2f", value)) ;
    return total ;
}

QString ServerListModel::getAvg2(int index)
{
    Host *host = servers.at(index) ;
    if (host->isError())
    {
        return "N/A" ;
    }
    float value = host->getLoadAvg2();
    QString total ;
    QString tmp ;
    total.append(tmp.sprintf("%.2f", value)) ;
    return total ;
}

QString ServerListModel::getAvg3(int index)
{
    Host *host = servers.at(index) ;
    if (host->isError())
    {
        return "N/A" ;
    }
    float value = host->getLoadAvg1();
    QString total ;
    QString tmp ;
    total.append(tmp.sprintf("%.2f", value)) ;
    return total ;
}
