#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDebug>
#include "qmlapplicationviewer.h"
#include "libssh2.h"
#include "update.h"
#include "serverlistmodel.h"
#include <QDeclarativeContext>
#include "sshsocket.h"
#include <QObject>
#include <QGraphicsObject>
#ifndef Q_WS_SIMULATOR
#ifndef AEGIS
#define AEGIS
#include "aegis_common.h"
#include "aegis_crypto.h"
#endif
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    int rc = libssh2_init (0);
    if (rc != 0) {
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }
    aegis_crypto_init() ;
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;
    ServerListModel *model = new ServerListModel() ;
    sshsocket sock ;
    update update;
    model->setUpdt(&update);
    update.getServers(model->getServers());
    update.getModel(model);
    QDeclarativeContext *ctxt = viewer.rootContext();
    model->setContext(ctxt);
    ctxt->setContextProperty("server", model);
    ctxt->setContextProperty("socket", &sock);
    ctxt->setContextProperty("updt", &update);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/ServerMonitor/main.qml"));
    viewer.showExpanded();
    return app->exec();
}
