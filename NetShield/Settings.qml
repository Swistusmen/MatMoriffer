import QtQuick 2.0
import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
        property color logColor: "lightblue"
        property color borderColor: "black"
        ListView {
                id: listView
                anchors.fill: parent
                model: ListModel {
                }

                delegate: Item {
                    width: listView.width
                    height: 50

                    Rectangle {
                        border.color: borderColor

                        width: parent.width
                        height: parent.height
                        color: logColor

                        Text {
                            anchors.centerIn: parent
                            text: model.name
                        }
                    }
                }
        }

        Component.onCompleted: {
            logger.loadLogs()
        }

        Connections{
            target:logger

            function onSentLogMessage(msg){
                    listView.model.append({name: msg});
            }
        }
}
