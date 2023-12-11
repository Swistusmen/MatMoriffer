import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: mainWindow
    title: qsTr("NetShield")
    width: 1000
    height: 600
    visible: true

    Rectangle{
        id: topBar
        color: "green"
        visible: true

        anchors.right: mainWindow.right
        anchors.left: mainWindow.left
        anchors.top: mainWindow.top
        height: 75
        width: parent.width
    }

    Rectangle{
        id: sideBar
        color: "red"
        visible: true
        height: parent.height-topBar.height
        width: 150
        anchors.top: topBar.bottom
        anchors.bottom: mainWindow.bottom
        anchors.left: mainWindow.left

        MouseArea{
            id: sideBarMouseArea
            anchors.left: sideBar.left
            anchors.top: sideBar.top
            anchors.bottom: sideBar.bottom
        }
    }

}
