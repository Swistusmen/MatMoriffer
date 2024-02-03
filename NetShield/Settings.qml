import QtQuick 2.0
import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
        ListView {
                id: listView
                anchors.fill: parent
                model: ListModel {
                    ListElement { name: "Item 1" }
                    ListElement { name: "Item 2" }
                    // Dodaj więcej elementów, jeśli chcesz
                }

                delegate: Item {
                    width: listView.width
                    height: 50

                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: index % 2 === 0 ? "lightblue" : "lightgreen"

                        Text {
                            anchors.centerIn: parent
                            text: model.name
                        }
                    }
                }
        }

        Row {
            anchors.bottom: parent.bottom
            spacing: 10
            TextInput {
                id: newItemInput
                width: 200
                text: "Enter new item"
            }

            Button {
                text: "Add Item"
                onClicked: {
                    if (newItemInput.text.trim() !== "") {
                        listView.model.append({name: newItemInput.text});
                        newItemInput.text = "";
                    }
                }
            }
        }
}
