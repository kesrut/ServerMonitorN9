import QtQuick 1.1
import com.nokia.meego 1.0
Page {
    id: listPage
    tools: commonTools
    property int selectedIndexValue
    property string hostTitle

    AddServerPage
    {
        id: addServerPage
    }
    EditPage
    {
        id: editPage
    }
    DetailsPage
    {
       id: detailsPage
    }
    Menu
    {
        id: contextMenu1
        visualParent: listPage
        MenuLayout
        {
            MenuItem
            {
                text: "Edit"
                onClicked:
                {
                    appWindow.pageStack.push(editPage, {index: selectedIndexValue})
                    editPage.update()
                }
            }
            MenuItem
            {
                text: "Delete"
                onClicked:
                {
                    dialog1.open()
                }
            }
        }
    }
    QueryDialog
    {
        id: dialog1
        titleText: "Information"
        message: "Delete server named " + hostTitle
        acceptButtonText: "Yes"
        rejectButtonText: "Cancel"
        onAccepted:
        {
            server.deleteServerWithIndex(selectedIndexValue)
        }
        onRejected:
        {        
        }
    }
    Component
    {
        id: serverListDelegate
        Item
        {
            BorderImage
            {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -listPage.anchors.leftMargin
                anchors.rightMargin: -listPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }
            MouseArea
            {
                id: mouseArea
                anchors.fill: background
                onPressAndHold:
                {
                    selectedIndexValue = model.index
                    hostTitle = server.getServerTitle(selectedIndexValue)
                    contextMenu1.open()
                }
                onClicked:
                {
                    selectedIndexValue = model.index
                    appWindow.pageStack.push(detailsPage, {index: model.index})
                    detailsPage.update()
                }
            }
            width: parent.width
            height: 100
            id: serverListItem
            Image
            {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }
            Image
            {
                source: (error === true) ? "red-on-16.png" : "green-on16.png"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 20
            }
            Column
            {
                width: parent.width
                height:parent.height
                spacing: 0
                Text
                {
                    font.pixelSize: 40
                    text: name
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                }
                Text
                {
                    font.pixelSize: 18
                    text: status
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                }
            }
        }
    }
    ListView
    {
        id: list
        anchors.fill: parent
        model: server.getModel()
        delegate: serverListDelegate
    }
}

