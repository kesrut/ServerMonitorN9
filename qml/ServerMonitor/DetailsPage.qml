import QtQuick 1.1
import com.nokia.meego 1.0

Page
{
    property int index
    id: detailsPage
    width: parent.width
    height: parent.height
    tools: commonTools
    property string uptime
    property string memtotal
    property string memfree
    property string avg1
    property string avg2
    property string avg3
    Component.onCompleted:
    {
        uptime = "N/A"
        memtotal = "N/A"
        memfree = "N/A"
        avg1 = "N/A"
        avg2 = "N/A"
        avg3 = "N/A"
    }
    ToolBarLayout
    {
        id: commonTools
        ToolIcon
        {
            iconId: "icon-m-toolbar-back"
            onClicked:
            {
                appWindow.pageStack.pop()
            }
        }
    }
    function update()
    {
        uptime = server.getUptime(index)
        memtotal = server.getMemTotal(index)
        memfree = server.getFreeMem(index)
        avg1 = server.getAvg1(index)
        avg2 = server.getAvg2(index)
        avg3 = server.getAvg3(index)
    }
    Connections
    {
        target: server
        onLayoutChanged:
        {
            uptime = server.getUptime(index)
            memtotal = server.getMemTotal(index)
            memfree = server.getFreeMem(index)
            avg1 = server.getAvg1(index)
            avg2 = server.getAvg2(index)
            avg3 = server.getAvg3(index)
        }
    }
    Flickable
    {
        id: flickable
        anchors.fill: parent
        contentHeight: 420
        contentWidth: parent.width
    Item
    {
        anchors.top: parent.top
        anchors.topMargin: 10
        id: item1
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Uptime"
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: uptime
                font.pixelSize: 25
                color: "black"
            }
        }
    }
    Item {
        anchors.top: item1.bottom
        anchors.topMargin: 10
        id: item2
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Memory total"
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: memtotal
                font.pixelSize: 25
                color: "black"
            }
        }
    }
    Item
    {
        anchors.top: item2.bottom
        anchors.topMargin: 10
        id: item3
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Memory free"
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
        Text
        {
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: memfree
            font.pixelSize: 25
            color: "black"
            }
        }
    }
    Item
    {
        anchors.top: item3.bottom
        anchors.topMargin: 10
        id: item4
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Load 1 Min."
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: avg1
                font.pixelSize: 25
                color: "black"
            }
        }
    }
    Item
    {
        anchors.top: item4.bottom
        anchors.topMargin: 10
        id: item5
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Load 5 Min."
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: avg2
                font.pixelSize: 25
                color: "black"
            }
        }
    }
    Item
    {
        anchors.top: item5.bottom
        anchors.topMargin: 10
        id: item6
        width: parent.width
        height: 55
        Column
        {
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Load 15 Min."
                font.pixelSize: 30
                font.bold: true
                color: "gray"
            }
            Text
            {
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: avg3
                font.pixelSize: 25
                color: "black"
            }
        }
    }
    }
}
