folder_01.source = qml/ServerMonitor
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

TARGET=servermonitor

CONFIG += link_pkgconfig meegotouch
#CONFIG += qdbus
#PKGCONFIG += libaegis-crypto
QT += declarative \
      network
DEPENDPATH += .
INCLUDEPATH += .

unix:!isEmpty(MEEGO_VERSION_MAJOR)
{
    LIBS += -L"$$_PRO_FILE_PWD_/arm/" -lssh2 -lcrypto -lz -laegis_crypto # -lcrypto -lz
    LIBS += -L /Users/kesrut/QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/lib/ -lcrypto -lz
}

SOURCES += main.cpp \
    host.cpp \
    serverlistmodel.cpp \
    sshsocket.cpp \
    update.cpp \
    updatethread.cpp \
    updateelement.cpp

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += \
    host.h \
    libssh2.h \
    serverlistmodel.h \
    sshsocket.h \
    update.h \
    updatethread.h \
    updateelement.h

RESOURCES += \
    Resources.qrc
