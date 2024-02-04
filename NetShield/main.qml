import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import "./"

Window {
    id: mainWindow
    title: qsTr("NetShield")
    width: 1000
    height: 600
    visible: true

    property int backboardMarginsValue: 10

    Rectangle{
        id: secondLayer
        width: parent.width
        height: parent.height
        color: "#52D3D8"
    }

    TabView
    {
    id: viewTab

    anchors.left: parent.left;
    anchors.top: parent.top;
    anchors.leftMargin: backboardMarginsValue
    anchors.rightMargin: backboardMarginsValue
    anchors.bottomMargin: backboardMarginsValue
    anchors.topMargin: backboardMarginsValue

    width: parent.width-2*backboardMarginsValue
    height: parent.height-2*backboardMarginsValue

    currentIndex: tabContext.tabIndex;
    Binding
    {
    target: tabContext;
    property: "tabIndex";
    value: viewTab.currentIndex;
    }

    Tab
    {
    title: "General";
        GeneralView{
            id: generalView
            anchors.fill: parent
        }
    }

    Tab
    {
    title: "Statistics View";
        Rectangle
        {
        anchors.fill: parent
        color: "yellow"
        }
    }

    Tab
    {
    title: "Security View";
        Rectangle
        {
        anchors.fill: parent
        color: "blue"
        }
    }

    Tab{
        title:"Settings"
        Settings{
            id: settingsView
            anchors.fill: parent
        }
    }
    }
}
