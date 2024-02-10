import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

Item {
    Rectangle{
        id: mainPlan
        anchors.fill: parent
        color: "green"

        property int sideBarMargin: 5
        property bool isTracking: false
        property string clickedButtonColor: "#d0d0d0"
        property string unclickedButtonColor: "#e0e0e0"
        property string sideBarBackgroundColor:"#96EFFF"
        property string mainWindowBackgroundColor:"#7B66FF"
        property string consoleBackgroundColor: "lightblue"//"#3887BE"
        property string informationTextBackgroundColor: "white"

        property var currentTime: 0

        Rectangle{
            id: sideBar
            anchors.left: mainPlan.left
            anchors.bottom: mainPlan.bottom
            anchors.top: mainPlan.top
            height: mainPlan.height
            width: mainPlan.width/5
            color: mainPlan.sideBarBackgroundColor

        ColumnLayout{
            id: sideBarLayout
            height: sideBar.height
            Layout.preferredWidth: sideBar.width
            Layout.maximumWidth: sideBar.width
            Layout.minimumWidth: sideBar.width
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            Layout.topMargin: mainPlan.sideBarMargin
            spacing: 5

            property int leftMarginValue:10

            Text {
                id: protocolText
                text: qsTr("Protocol")
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.topMargin: sideBarLayout.leftMarginValue
                Layout.leftMargin: sideBarLayout.leftMarginValue
           }

            RowLayout{
                id: protocolLayout
                Layout.preferredWidth: sideBarLayout.width
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop


                Button{
                    id:tcpButton
                    text: "TCP"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.leftMargin: sideBarLayout.leftMarginValue
                    property bool wasClicked: false

                    background: Rectangle {
                        anchors.fill: parent
                                color: tcpButton.wasClicked? mainPlan.clickedButtonColor : mainPlan.unclickedButtonColor

                   }

                    onClicked: {
                        tcpButton.wasClicked=!tcpButton.wasClicked
                        interMessageBroker.tcpClicked()
                    }
                }

                Button{
                    id: udpButton
                    text: "UDP"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.preferredHeight: tcpButton.height
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    property bool wasClicked: false

                    background: Rectangle {
                        anchors.fill: parent
                                color: udpButton.wasClicked? mainPlan.clickedButtonColor : mainPlan.unclickedButtonColor

                   }

                    onClicked: {
                        udpButton.wasClicked=!udpButton.wasClicked
                        interMessageBroker.udpClicked();
                    }
                }
            }

            Text {
                id: whoSendsPacketsText
                text: qsTr("Track packets")
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
           }

            RowLayout{
                id: whoSendsPacketsLayout
                Layout.preferredWidth: sideBarLayout.width
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                Button{
                    id: amSenderButton
                    text: "comming"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.leftMargin: sideBarLayout.leftMarginValue
                    property bool wasClicked: false

                    background: Rectangle {
                        anchors.fill: parent
                                color: amSenderButton.wasClicked? mainPlan.clickedButtonColor : mainPlan.unclickedButtonColor

                   }

                    onClicked: {
                        amSenderButton.wasClicked=!amSenderButton.wasClicked
                    }
                }

                Button{
                    id: amReceiverButton
                    text: "leaving"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    property bool wasClicked: false

                    background: Rectangle {
                        anchors.fill: parent
                                color: amReceiverButton.wasClicked? mainPlan.clickedButtonColor : mainPlan.unclickedButtonColor

                   }

                    onClicked: {
                        amReceiverButton.wasClicked=!amReceiverButton.wasClicked
                    }
                }
            }

            Button{
                id: setupNewParameters
                text: qsTr("Reload and start")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
                enabled: (udpButton.wasClicked || tcpButton.wasClicked ) && (amReceiverButton.wasClicked || amSenderButton.wasClicked)

                onClicked: {
                    mainPlan.isTracking=true
                    interMessageBroker.reloadParameters()
                    if (timerComponent.running) {
                                        timerComponent.stop();
                                        timer.text="00:00"
                                        mainPlan.currentTime=0
                                        timerComponent.start();
                                    } else {
                                        timerComponent.start();
                                    }
                }

            }

            Button {
                    id: stopButton
                    text: "Stop tracking"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    enabled: mainPlan.isTracking
                    onClicked: {
                        interMessageBroker.stopTracing()
                        timerComponent.stop();
                        mainPlan.currentTime=0
                        timer.text="00:00"
                    }
            }

            Text {
                id: logsText
                text: qsTr("Logs")
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
           }

            RowLayout{
                id: logsControlLayout
                Layout.preferredWidth: sideBarLayout.width
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                Button{
                    id:cleanLogsButton
                    text: "Clean"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.leftMargin: sideBarLayout.leftMarginValue
                    enabled: mainPlan.isTracking

                    onClicked: {
                        listView.model=ListModel
                    }
                }

                Button{
                    id: saveLogsButton
                    text: "Save"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    enabled: mainPlan.isTracking
                    onClicked: {
                        fileDialog.open()
                    }
                }

                FileDialog {
                    id: fileDialog
                    title: "Save Logs"
                    folder: shortcuts.home
                    selectMultiple: false
                    selectExisting: false

                    onAccepted: {
                        interMessageBroke.saveLogs(fileDialog.fileUrls[0].toString())
                    }
                }

            }

            Button{
                id:showAllLogsButton
                text: "Show all logs"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                enabled: mainPlan.isTracking
                onClicked: {
                    interMessageBroker.showAllLogs()
                }
            }

            Text{
                id: filterText
                text: qsTr("Show messages with IP")
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
            }

            RowLayout{
                id: filteringLayout
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                Rectangle{
                    border.width: 1
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.preferredHeight: filterAddressButton.height
                    Layout.leftMargin: sideBarLayout.leftMarginValue

                TextInput{
                    id: filterAddressInputText
                    text: qsTr("127.0.0.1")
                    clip: true
                    anchors.fill: parent
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter

                }

                }
                Button{
                    id: filterAddressButton
                    text: qsTr("Filter")
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    enabled: mainPlan.isTracking
                    onClicked: {
                        listView.model=ListModel
                        interMessageBroker.filterIpAddress(filterAddressInputText.text)
                    }
                }
            }


          }
        }
    }

    Rectangle{
        id: mainBar
        anchors.top:mainPlan.top
        anchors.bottom: mainPlan.bottom
        anchors.right: mainPlan.right
        width: mainPlan.width/5*4
        height: mainPlan.height
        color: mainPlan.mainWindowBackgroundColor

        ColumnLayout{
            id: mainColumnLayout
            height: mainBar.height
            Layout.preferredWidth: mainBar.width
            Layout.maximumWidth: mainBar.width
            Layout.minimumWidth: mainBar.width
            Layout.rightMargin: mainBar.width * 0.025
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

            ListView {
                    id: listView
                    Layout.preferredWidth: mainBar.width * 0.95
                    Layout.preferredHeight: mainBar.height * 0.8
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.leftMargin:  mainBar.width * 0.025
                    Layout.topMargin: mainBar.width * 0.025
                    model: ListModel {
                    }

                    delegate: Item {
                        width: listView.width
                        height: 50

                        Rectangle {
                            border.color: "black"
                            border.width: 1

                            width: parent.width
                            height: parent.height
                            color: mainPlan.consoleBackgroundColor

                            Text {
                                anchors.centerIn: parent
                                text: model.name
                            }
                        }
                    }
            }

            RowLayout{
                id: informationLayout
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.bottomMargin: mainBar.width * 0.025
                Layout.preferredWidth: mainColumnLayout.width

                TextArea{
                    id: timer
                    text:qsTr("00:00")
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop
                    Layout.preferredWidth: mainColumnLayout.width *0.15
                    Layout.leftMargin:  mainBar.width * 0.3
                    readOnly: true
                    background: Rectangle{
                        anchors.fill: parent
                        color: mainPlan.informationTextBackgroundColor
                    }

                    Timer {
                        id: timerComponent
                        interval: 1000
                        running: false
                        repeat: true
                        onTriggered: {
                            updateTime();
                        }
                    }
                }
            }
        }
    }

    function updateTime() {
                    mainPlan.currentTime += 1000;
                    var seconds = Math.floor(mainPlan.currentTime / 1000);
                    var minutes = Math.floor(seconds / 60);
                    seconds = seconds % 60;

                    var formattedTime = padZero(minutes) + ":" + padZero(seconds);
                    timer.text = formattedTime;
                }

    function padZero(number) {
        return number < 10 ? "0" + number : number;
    }

    Connections {
      target: interMessageBroker
      function onSomeMessage(result) {
        console.log (result)
      }


    }


    Connections{
        target: interMessageBroker
        function onMessageFromDriverSocket(mmessage){
            listView.model.append(message);
        }
    }

    Component.onCompleted: {
        tcpButton.wasClicked=interMessageBroker.tcpStatus()
        udpButton.wasClicked=interMessageBroker.udpStatus()
    }
}
