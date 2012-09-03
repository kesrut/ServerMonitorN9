#include "updatethread.h"
#include <openssl/evp.h>
#include <openssl/aes.h>

updatethread::updatethread()
{

}

updatethread::updatethread(QObject *parent)
{
    QObject::connect(qobject_cast<QObject*>(this), SIGNAL(update()), parent, SLOT(updt())) ;
}

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
  /* because we have padding ON, we must allocate an extra cipher block size of memory */
  int p_len = *len, f_len = 0;
  unsigned char *plaintext = (unsigned char*)malloc(p_len + AES_BLOCK_SIZE);

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

static void kbd_callback(const char *name, int name_len,
                         const char *instruction, int instruction_len,
                         int num_prompts,
                         const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
                         LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
                         void **abstract)
{
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    char *password = (char*)(*abstract);
    if (num_prompts == 1) {
        responses[0].text = strdup(password);
        responses[0].length = strlen(password);
    }
    (void)prompts;
    (void)abstract;
} /* kbd_callback */

void updatethread::run()
{

    Host *host = hosts.at(index) ;
    qint16 port = host->port() ;
    socket = new QTcpSocket() ;
    socket->connectToHost(host->hostname(), port);
    if (socket->waitForConnected())
    {
        sock = socket->socketDescriptor() ;
        host->setMessage("Connecting...");
        emit update();
#ifdef Q_WS_SIMULATOR
        QString password ;
        password = host->password() ;
#else
        RAWDATA_PTR clearData;
        size_t length = 0;
        QByteArray secret;

        secret.append(host->getSecret()) ;
        aegis_crypto_decrypt(secret.data(), secret.length(), NULL, &clearData, &length) ;
        QByteArray decrypted((char *)clearData, length);
        QString password(decrypted) ;
       // aegis_crypto_free(clearData);

#endif
        session = libssh2_session_init_ex(NULL, NULL, NULL, password.data());
        rc = libssh2_session_startup(session, sock) ;
        if (rc)
        {
            host->setMessage("Failure establishing SSH session!");
            host->setError(true);
            emit update();
            socket->close();
            delete socket ;
            deleteLater();
            return ;
        }
        else
        {
            nh = libssh2_knownhost_init(session);
            if(!nh) {
                libssh2_knownhost_free(nh) ;
                libssh2_session_free(session);
                socket->close();
                delete socket ;
                return ;
            }
            QString known_host_dir  = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "known_hosts" ;
            libssh2_knownhost_readfile(nh, known_host_dir.toAscii(),
                                          LIBSSH2_KNOWNHOST_FILE_OPENSSH);
            fingerprint = libssh2_session_hostkey(session, &len, &type);
            if(fingerprint)
               {
                    struct libssh2_knownhost *known_host;
           #if LIBSSH2_VERSION_NUM >= 0x010206
                   int check = libssh2_knownhost_checkp(nh, host->hostname().toAscii(), host->port() ,
                                                        fingerprint, len,
                                                        LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                                        LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                                        &known_host);
           #else

                   int check = libssh2_knownhost_check(nh, host->hostname().toAscii(),
                                                       fingerprint, len,
                                                       LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                                       LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                                       &known_host);
           #endif
                   if (check == LIBSSH2_KNOWNHOST_CHECK_MISMATCH)
                   {
                      host->setMessage("Host key mismatch!");
                      libssh2_session_free(session);
                      libssh2_knownhost_free(nh) ;
                      host->setError(true);
                      emit update();
                      socket->close();
                      delete socket ;
                      deleteLater();
                      return ;
                   }
             }
            else
            {
                   host->setMessage("Host key mismatch!");
                   libssh2_session_free(session);
                   libssh2_knownhost_free(nh) ;
                   host->setError(true);
                   emit update();
                   socket->close();
                   delete socket ;
                   deleteLater();
                   return ;
            }
            libssh2_knownhost_free(nh);
            char *userauthlist;
            userauthlist = libssh2_userauth_list(session, host->username().toAscii(), strlen(host->username().toAscii()));
            if (userauthlist == NULL)
            {
                host->setMessage("Misc authentication error!");
                libssh2_session_disconnect(session,"Disconnect!");
                libssh2_session_free(session);
                host->setError(true);
                emit update();
                socket->close();
                delete socket ;
                return ;
            }
            if (strstr(userauthlist, "password") != NULL) {
                    if ( strlen(password.toAscii()) != 0 ) {
                        while ((rc = libssh2_userauth_password(session, host->username().toAscii(), password.toAscii())) ==
                               LIBSSH2_ERROR_EAGAIN);
                        if (rc) {
                            host->setMessage("Authentication by password failed!");
                            host->setError(true);
                            libssh2_session_disconnect(session,"Disconnect!");
                            libssh2_session_free(session);
                            emit update();
                            socket->close();
                            delete socket ;
                            deleteLater();
                            return;
                        }
                    }
                }
                else
                if (strstr(userauthlist, "keyboard-interactive") != NULL) {
                    if ( (libssh2_userauth_keyboard_interactive_ex(session, host->username().toAscii(),strlen(host->username().toAscii()),

                                                                 &kbd_callback)) )
                    {

                        host->setMessage("Authentication by password failed!");
                        host->setError(true);
                        libssh2_session_disconnect(session,"Disconnect!");
                        libssh2_session_free(session);
                        emit update();
                        socket->close();
                        delete socket ;
                        deleteLater();
                        return;
                    }

                }
            QString v;
            v = execute("cat /proc/uptime") ;
            if (exitcode == 0)
            {
                long uptime_seconds ;
                int mili ;
                sscanf(v.toAscii(), "%ld.%d", &uptime_seconds, &mili) ;
                host->setUptime(uptime_seconds);
            }
            else
            {
                host->setUptime(0);
            }
            v = execute("cat /proc/meminfo | grep MemTotal") ;
            if (exitcode == 0)
            {
                long mem_total = 0 ;
                sscanf(v.toAscii(), "MemTotal:\t%ld", &mem_total) ;
                if (mem_total > 0)
                {
                    host->setMemTotal(mem_total);
                }
            }
            else
            {
                host->setMemTotal(0);
            }
            v = execute("cat /proc/meminfo | grep MemFree") ;
            if (exitcode == 0)
            {
                long free_total = 0 ;
                sscanf(v.toAscii(), "MemFree:\t%ld", &free_total) ;
                if (free_total > 0)
                {
                    host->setMemFree(free_total);
                }
            }
            else
            {
                host->setMemFree(0);
            }
            v = execute("cat /proc/loadavg") ;
            if (exitcode == 0)
            {
                float avg1, avg2, avg3 ;
                sscanf(v.toAscii(), "%f %f %f", &avg1, &avg2, &avg3) ;
                host->setLoadAvg1(avg1);
                host->setLoadAvg2(avg2);
                host->setLoadAvg3(avg3);
            }
            else
            {
                host->setLoadAvg1(0);
                host->setLoadAvg2(0);
                host->setLoadAvg3(0);
            }
            QTime time = QTime::currentTime() ;
            host->setMessage("Updated: " + time.toString().toAscii());
            host->setError(false) ;
            libssh2_session_disconnect(session,"ServerMonitor Disconnect!");
            libssh2_session_free(session);
            emit update();
            socket->close();
            delete socket ;
            deleteLater();
        }
    }
    else
    {
        host->setError(true);
        host->setMessage(socket->errorString());
        emit update();
        socket->close();
        delete socket ;
        deleteLater();
    }
}

void updatethread::setHosts(QList<Host*> &h)
{
    hosts = h ;
}

void updatethread::setServerIndex(int i)
{
    index = i ;
}

QString updatethread::execute(QString cmd)
{
    data.clear();
    if (session == NULL)
    {
        exitcode = 127 ;
        return QString() ;
    }
    channel = libssh2_channel_open_session(session) ;
    if( channel == NULL )
    {
        session = NULL ;
        exitcode = 127 ;
        return QString();
    }
    rc = libssh2_channel_exec(channel, cmd.toAscii()) ;
    if( rc != 0 )
    {
        exitcode = 127 ;
        session = NULL ;
        return QString();
    }
    int rc;
    do
    {
        char buffer[0x4000];
        rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );
        if( rc > 0 )
        {
            int i;
            bytecount += rc;
            data.append(buffer) ;
        }
    }
    while( rc > 0 );
    exitcode = 127;
    rc = libssh2_channel_close(channel) ;
    if( rc == 0 )
    {
        exitcode = libssh2_channel_get_exit_status( channel );
    }
    if (channel != NULL)
    {
        libssh2_channel_free(channel);
        channel = NULL;
     }
     return data ;
}
