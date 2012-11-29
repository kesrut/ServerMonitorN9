import QtQuick 1.1
import com.nokia.meego 1.0

Page
{
    property int index
    tools: commonTools
    property string title
    property string hostname
    property string username
    property string port
    property string password
    property string error_message
    property bool error
    function update()
    {
        title = server.getServerTitle(index)
        hostname = server.getHostname(index)
        username = server.getUsername(index)
        password = server.getPassword(index)
        port = server.getPort(index)
    }
    BusyIndicator
    {
        visible: false
        id: indicator1
        anchors.centerIn: parent
        width: 20
        height: 20
    }
    QueryDialog
    {
        id: errorDialog
        titleText: "Information"
        acceptButtonText: "OK"
        message: error_message
    }
    id: editPage
    ToolBarLayout
    {
        id: commonTools
        visible: true
        ToolIcon
        {
            iconId: "toolbar-back"
            onClicked:
            {
                indicator1.visible = false
                indicator1.running = false
                appWindow.pageStack.pop()
            }
        }
        ToolIcon
        {
            iconId: "toolbar-done"
            onClicked:
            {
                error = false

                if (textfield1.text.length == 0)
                {
                    error_message = "Title field can't be empty"
                    error = true
                    errorDialog.open()
                }
                if (textfield2.text.length == 0 && error == false)
                {
                    error_message = "Hostname field can't be empty"
                    error = true
                    errorDialog.open()
                }
                if (textfield3.text.length == 0 && error == false)
                {
                    error_message = "Username field can't be empty"
                    error = true
                    errorDialog.open()
                }
                if (textfield4.text.length == 0 && error == false)
                {
                    error_message = "Password field can't be empty"
                    error = true
                    errorDialog.open()
                }
                if (textfield5.text.length == 0 && error == false)
                {
                    error_message = "Port field can't be empty"
                    error = true
                    errorDialog.open()
                }
                if (error == false)
                {
                    indicator1.visible = true
                    indicator1.running = true
                    socket.setIndex(index)
                    socket.EditHost(textfield1.text, textfield2.text, textfield3.text, textfield4.text, textfield5.text)
                    appWindow.indicator1 = indicator1
                }
            }
        }
    }
    Flickable
    {
        id: flickable
        anchors.topMargin: 30
        anchors.fill: parent
        contentHeight: 560
        contentWidth: parent.width
        Item
        {
            id: rootItem
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            anchors.topMargin: 20
            Item
            {
                id: item1
                width: parent.width
                height: label1.font.pixelSize + textfield1.height + 5
                Label
                {
                    id: label1
                    text: "Title"
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                }
                TextField
                {
                    anchors.top: label1.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    id: textfield1
                    text: title
                }
            }
            Item
            {
                id: item2
                anchors.top: item1.bottom
                width: parent.width
                height: label2.font.pixelSize + textfield2.height + 5
                anchors.topMargin: 20
                Label
                {
                    id: label2
                    text: "Hostname"
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                }
                TextField
                {
                    anchors.top: label2.bottom
                    id: textfield2
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    text: hostname
                    inputMethodHints: Qt.ImhNoAutoUppercase
                }
            }
            Item
            {
                id: item3
                width: parent.width
                height: label3.font.pixelSize + textfield3.height + 5
                anchors.top: item2.bottom
                anchors.topMargin: 20
                Label
                {
                    id: label3
                    text: "Username"
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                }
                TextField
                {
                    anchors.top: label3.bottom
                    id: textfield3
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    text: username
                    inputMethodHints: Qt.ImhNoAutoUppercase
                }
            }
            Item
            {
                id: item4
                width: parent.width
                height: label4.font.pixelSize + textfield4.height + 5
                anchors.top: item3.bottom
                anchors.topMargin: 20
                Label
                {
                    id: label4
                    text: "Password"
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                }
                TextField
                {
                    anchors.top: label4.bottom
                    id: textfield4
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    echoMode: TextInput.Password
                    text: password
                }
            }
            Item
            {
                id: item5
                width: parent.width
                height: label5.font.pixelSize + textfield5.height + 5
                anchors.top: item4.bottom
                anchors.topMargin: 20
                Label
                {
                    id: label5
                    text: "Port"
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                }
                TextField
                {
                    anchors.top: label5.bottom
                    id: textfield5
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    text: port
                    placeholderText: "22"
                    inputMethodHints: Qt.ImhPreferNumbers
                }
            }

        }
    }
}
