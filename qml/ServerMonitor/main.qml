import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    property string fingerprintmessage;
    property variant indicator1
    property variant clear
    property string dialogErrorMessage
    QueryDialog
    {
        id: fingerPrintDialog
        message: fingerprintmessage
        titleText: "Fingerprint"
        acceptButtonText: "Accept"
        rejectButtonText: "Reject"
        onAccepted:
        {
            addServerPage.clear()
            indicator1.visible = false
            indicator1.running = false
            socket.writeKnowHost()
            socket.insert() ;
            appWindow.pageStack.pop() ;
        }
        onRejected:
        {
            socket.Rejected()
            appWindow.pageStack.pop() ;
        }
    }
    QueryDialog
    {
        id: fingerPrintDialog1
        message: fingerprintmessage
        titleText: "Fingerprint"
        acceptButtonText: "Accept"
        rejectButtonText: "Reject"
        onAccepted:
        {
            indicator1.visible = false
            indicator1.running = false
            socket.writeKnowHost()
            socket.edit()
            appWindow.pageStack.pop() ;
        }
        onRejected:
        {
            socket.Rejected()
            appWindow.pageStack.pop() ;
        }
    }
    QueryDialog
    {
        id: errorDialog
        titleText: "Information"
        acceptButtonText: "OK"
        message: dialogErrorMessage
    }
    Connections
    {
        target: socket
        onError:
        {
            indicator1.visible = false
            indicator1.running = false
            dialogErrorMessage = errorMessage
            errorDialog.open()
        }
        onAccept_fingerprint:
        {
            indicator1.visible = false
            indicator1.running = false
            fingerprintmessage = message
            fingerPrintDialog.open()
        }
        onInsertHost:
        {
            addServerPage.clear()
            indicator1.visible = false
            indicator1.running = false
            socket.insert()
            appWindow.pageStack.pop() ;
        }
        onAccept_fingerprint_edit:
        {
            indicator1.visible = false
            indicator1.running = false
            fingerprintmessage = message
            fingerPrintDialog1.open()
        }
        onInsertHost_edit:
        {
            indicator1.visible = false
            indicator1.running = false
            socket.edit()
            appWindow.pageStack.pop() ;
        }
    }
    MainPage
    {
        id: mainPage
    }
    initialPage: mainPage
    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon
        {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolIcon
        {
            platformIconId: "icon-m-toolbar-add"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked:
            {
                appWindow.pageStack.push(addServerPage)
            }
        }
    }
    AddServerPage
    {
        id: addServerPage
    }
    Menu
    {
        id: myMenu
        visualParent: pageStack
        MenuLayout
        {
            MenuItem
            {
                text: qsTr("Refresh")
                onClicked:
                {
                    server.refresh()
                }
            }
            MenuItem
            {
                text: qsTr("Quit")
                onClicked:
                {
                    Qt.quit()
                }
            }
        }
    }
}


