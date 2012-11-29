#include "sshsocket.h"

sshsocket::sshsocket(QObject *parent) : QObject(parent)
{
    setMode(0);
}

void sshsocket::setMode(int mode)
{
    this->mode = mode  ;
}

void sshsocket::socket_connected_slot()
{
   rc = libssh2_init (0);
   sock = socket->socketDescriptor() ;
   session = libssh2_session_init();
   libssh2_session_set_blocking(session, 0);
   while ((rc = libssh2_session_startup(session, sock)) == LIBSSH2_ERROR_EAGAIN);
   if (rc)
   {
       emit error("Failure establishing SSH session!") ;
       libssh2_session_free(session);
   }
   nh = libssh2_knownhost_init(session);
      if(!nh) {
          /* eeek, do cleanup here */
      }
      QString known_host_dir  = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "known_hosts" ;
      libssh2_knownhost_readfile(nh, known_host_dir.toAscii(),
                                 LIBSSH2_KNOWNHOST_FILE_OPENSSH);
      fingerprint = libssh2_session_hostkey(session, &len, &type);
      if(fingerprint) {
#if LIBSSH2_VERSION_NUM >= 0x010206
          /* introduced in 1.2.6 */
          struct libssh2_knownhost *known_host;
          int check = libssh2_knownhost_checkp(nh, host->hostname().toAscii(), host->port(),
                                               fingerprint, len,
                                               LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                               LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                               &known_host);
  #else
          /* 1.2.5 or older */
          int check = libssh2_knownhost_check(nh, host->hostname().toAscii(),
                                              fingerprint, len,
                                              LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                              LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                              &known_host);
  #endif

          if (check == LIBSSH2_KNOWNHOST_CHECK_NOTFOUND)
          {
              const char *finger = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
              QString fingerkey ;
              fingerkey.append("Server's SHA-1 fingerprint is ") ;
              int i=0 ;
              for (i=0; i < 20; i++)
              {
                  if (i != 19)
                  {
                      QString str ;
                      str.sprintf("%02X:", (unsigned char)finger[i]) ;
                      fingerkey.append(str) ;
                  }
                  else {
                      QString str ;
                      str.sprintf("%02X", (unsigned char)finger[i]) ;
                      fingerkey.append(str) ;
                  }
                  if (i == 10)
                  {
                      fingerkey.append("<br>") ;
                  }
              }
              if (mode == 0)
              {

                 emit accept_fingerprint(fingerkey);
              }
              else
              {
                 emit accept_fingerprint_edit(fingerkey);
              }
              setMode(0);
          }
          if (check == LIBSSH2_KNOWNHOST_CHECK_MATCH)
          {
              if (mode == 0)
              {
                emit insertHost();
              }
              else
              {
                emit insertHost_edit();
              }
              setMode(0);
          }
      }
}


void sshsocket::insert()
{
    model->insertHost(host) ;
}

void sshsocket::edit()
{
    QTimer *timer = servers->at(index)->getTimer() ;
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
    servers->replace(index, host);
    model->new_updt(index);
}

void sshsocket::setIndex(qint32 index)
{
    this->index = index ;
}

int sshsocket::getIndex()
{
    return index ;
}

void sshsocket::Rejected()
{
    libssh2_knownhost_free(nh);
    libssh2_session_free(session);
    socket->close();
    delete socket ;
}

void sshsocket::writeKnowHost()
{
    QString known_host_dir  = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "known_hosts" ;
    type = type == LIBSSH2_HOSTKEY_TYPE_RSA ? LIBSSH2_KNOWNHOST_KEY_SSHRSA : LIBSSH2_KNOWNHOST_KEY_SSHDSS;
    libssh2_knownhost_add(nh, host->hostname().toAscii(), NULL, fingerprint, len,  LIBSSH2_KNOWNHOST_TYPE_PLAIN|LIBSSH2_KNOWNHOST_KEYENC_RAW|type, NULL) ;
    libssh2_knownhost_writefile(nh, known_host_dir.toAscii(),  LIBSSH2_KNOWNHOST_FILE_OPENSSH) ;
    libssh2_knownhost_free(nh);
    libssh2_session_free(session);
    socket->close();
    delete socket ;
}

QObject *sshsocket::getHost()
{
    return qobject_cast<QObject*>(host) ;
}
void sshsocket::EditHost(QString title, QString hostname, QString username, QString password, qint32 port )
{
    setMode(1) ;
    this->AcceptHost(title, hostname, username, password, port);
}

void sshsocket::AcceptHost(QString title, QString hostname, QString username, QString password, qint32 port )
{
    host = new Host(title, hostname, username, port) ;
#ifdef Q_WS_SIMULATOR
     host->setPassword(password);
#else
     QByteArray clearData ;
     clearData.append(password) ;
     RAWDATA_PTR cipherData = NULL;
     size_t cipherLength = 0;
     aegis_crypto_result rc = aegis_crypto_encrypt(clearData.data(), clearData.length(), NULL, &cipherData, &cipherLength) ;
     if (aegis_crypto_ok != rc) {
             AEGIS_ERROR("%s: aegis_crypto_encrypt failed (%s)", __func__,
                        aegis_crypto_last_error_str());
     }
     QByteArray encrypted((char *)cipherData, cipherLength);
     host->setSecret(encrypted);
    // aegis_crypto_free(cipherData);
#endif
    socket = new QTcpSocket(this) ;
    qint16 port_value = host->port() ;
    socket->connectToHost(host->hostname(), port_value);
    connect(socket, SIGNAL(connected()), this, SLOT(socket_connected_slot())) ;
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(display_error(QAbstractSocket::SocketError))) ;
}

void sshsocket::display_error(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError: break;
        default:
        {
            emit error(socket->errorString());
            socket->close();
            delete socket ;
        } break ;
    }
}

QString sshsocket::connectErrorMessage()
{
    return socket->errorString() ;
}
