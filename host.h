#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDataStream>
#include <QTcpSocket>

class Host : public QObject
{
    Q_OBJECT
public:
    Host() {}
    Host(const Host&, QObject *parent = 0) {}
    Host(const QString &title, const QString &hostname, const QString &username, qint32 port)
    {
        _title = QString(title);
        _hostname = QString(hostname) ;
        _username = QString(username) ;
        _port = port ;
        element = NULL ;
        _timer = NULL ;
        thread = NULL ;
    }
    void setPasswordId(qint32 id) { _password_id = id ; }
    qint32 password_id() const { return _password_id ; }
    void setTitle(const QString &title) { _title = QString(title) ; }
    QString title() const { return _title ; }
    void setHostName(const QString &hostname) { _hostname = hostname ; }
    QString hostname() const { return _hostname ; }
    void setUserName(const QString &username) { _username = username ; }
    QString username() const { return _username ; }
    void setPort(qint32 port) { _port = port ; }
    qint32 port() const { return _port ;}
    QString password() const { return _password; }
    void setPassword(const QString &password) { _password = password ; }
    bool isError() { return error ; }
    void setError(bool v) { error = v; }
    QString getMessage() const { return message ; }
    void setMessage(const QString & msg) { message = msg ; }
    QTimer *getTimer() { return _timer; }
    void setTimer(QTimer *t) { _timer = t ; }
    void setSecret(QByteArray &chiper) { chipedPassword = chiper ; }
    QByteArray getSecret() { return chipedPassword ; }
    // Parameters
    void setUptime(long upt) { uptime = upt; }
    long getUptime() { return uptime; }
    void setMemTotal(long total) { mem_total = total ; }
    long getMemTotal() { return mem_total; }
    void setMemFree(long free) { mem_free = free ; }
    long getMemFree() { return mem_free; }
    void setLoadAvg1(float avg) { loadavg1 = avg ; }
    float getLoadAvg1() { return loadavg1 ; }
    void setLoadAvg2(float avg) { loadavg2 = avg ; }
    float getLoadAvg2() { return loadavg2 ; }
    void setLoadAvg3(float avg) { loadavg3 = avg ; }
    float getLoadAvg3() { return loadavg3 ; }
    void *getUpdateElement() { return element ; }
    void setUpdateElement(void *elem) { element = elem ; }
    void *getThread() { return thread ; }
    void setThread(void *t) { thread = t ; }
private:
    QString _title ;
    QString _hostname ;
    QString _username ;
    qint32 _port ;
    qint32 _interval ;
    QTimer *_timer ;
    qint32 _password_id ;
    bool error ;
    QString message ;
    QByteArray chipedPassword ;
    QString _password ;
    long uptime ;
    long mem_total ;
    long mem_free ;
    float loadavg1 ;
    float loadavg2 ;
    float loadavg3 ;
    void *element ;
    void *thread ;
};

#endif // HOST_H
