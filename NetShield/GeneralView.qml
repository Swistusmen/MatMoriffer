import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

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
        property string consoleBackgroundColor: "#3887BE"
        property string informationTextBackgroundColor: "white"

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
                text: qsTr("Reload parameters")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
                enabled: (udpButton.wasClicked || tcpButton.wasClicked ) && (amReceiverButton.wasClicked || amSenderButton.wasClicked)

                onClicked: {
                    mainPlan.isTracking=true
                    interMessageBroker.reloadParameters()
                }

            }

            Text {
                id: tracingText
                text: qsTr("Tracking")
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: sideBarLayout.leftMarginValue
           }

            RowLayout{
                id: trackingControlLayout
                Layout.preferredWidth: sideBarLayout.width
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Button {
                        id:playButton
                        text: "Start"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                        Layout.leftMargin: sideBarLayout.leftMarginValue
                        enabled: mainPlan.isTracking
                }

                Button {
                        id: stopButton
                        text: "Stop"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                        enabled: mainPlan.isTracking
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
                }

                Button{
                    id: saveLogsButton
                    text: "Save"
                    Layout.preferredWidth: sideBar.width/2 - 3* mainPlan.sideBarMargin
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    enabled: mainPlan.isTracking
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

            ScrollView{
                id: consoleScrollView
                Layout.preferredWidth: mainBar.width * 0.95
                Layout.preferredHeight: mainBar.height * 0.8
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin:  mainBar.width * 0.025
                Layout.topMargin: mainBar.width * 0.025

                //TODO: refactor
            TextArea{
                id: consoleTextArea
                Layout.preferredWidth: mainBar.width * 0.95
                Layout.preferredHeight: mainBar.height * 0.9
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin:  mainBar.width * 0.025
                Layout.topMargin: mainBar.width * 0.025
                clip: true
                wrapMode: TextArea.Wrap
                readOnly: true
                background: Rectangle{
                    color: mainPlan.consoleBackgroundColor
                }

                Timer {
                                interval: 100
                                running: true
                                repeat: true
                                onTriggered: {
                                    consoleTextArea.text = consoleTextArea.text+getDmesgOutput();
                                }
                            }

                Component.onCompleted: {
                                consoleTextArea.text = consoleTextArea.text+getDmesgOutput();
                            }
            }
            }

            RowLayout{
                id: informationLayout
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.bottomMargin: mainBar.width * 0.025
                Layout.preferredWidth: mainColumnLayout.width

                TextArea{
                    id: packetsInformationText
                    text: qsTr("Tracking packets")
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.leftMargin:  mainBar.width * 0.025
                    Layout.preferredWidth: mainColumnLayout.width* 0.2
                    readOnly: true
                    background: Rectangle{
                        anchors.fill: parent
                        color: mainPlan.informationTextBackgroundColor
                    }
                }

                TextArea{
                    id: packetsInformationStatusText
                    text: qsTr("TCP | UDP")
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.preferredWidth: mainColumnLayout.width * 0.15

                    readOnly: true
                    background: Rectangle{
                        anchors.fill: parent
                        color: mainPlan.informationTextBackgroundColor
                    }
                }

                TextArea{
                    id: messagesInformationText
                    text: qsTr("TCP | UDP")
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.preferredWidth: mainColumnLayout.width *0.15
                    readOnly: true
                    background: Rectangle{
                        anchors.fill: parent
                        color: mainPlan.informationTextBackgroundColor
                    }
                }


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
                }
            }
        }
    }

    function getDmesgOutput() {
            return "DUPADUPADUPADUPADUPADUPA"
        }

    Connections {
      target: interMessageBroker
      function onSomeMessage(result) {
        console.log (result)
      }
    }
}
