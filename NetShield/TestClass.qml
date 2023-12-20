import QtQuick 2.0

Item {
    id: dupa
    height: 100
    width: 100
    property string  textToDisplay: "test"
    property string iconToDisplay: ""
    property var mainColor: "grey"
    property var hoveredColor: "blue"
    property var clickedColor: "green"
    property var clickedFunction:  function (){console.log("text message")}
    property bool isClicked: false
    property bool isMouseOver: false


    MouseArea{
        id: mouseArea
        height: parent.height
        width: parent.width
        anchors.fill: parent



        onClicked: function() {
                   isClicked = !isClicked;

                    mainRectangle.color = isClicked ? clickedColor : (mouseArea.containsMouse ? hoveredColor : mainColor);

                    clickedFunction();
            isClicked = !isClicked;
                }



        onHoveredChanged: {
            console.log("Change")
            mainRectangle.color = isClicked ? clickedColor : (mouseArea.containsMouse ? hoveredColor : mainColor);
        }

        onContainsMouseChanged: {
                    // Ustawianie koloru na podstawie stanu kliknięcia
                    console.log("Change")
                    mainRectangle.color = isClicked ? clickedColor : (mouseArea.containsMouse ? hoveredColor : mainColor);
                }

        Rectangle{
            id: mainRectangle
            height: parent.height
            width: parent.width
            anchors.fill: parent
            color: mouseArea.containsMouse? hoveredColor: mainColor
            Text {
                id: buttonText
                text: textToDisplay
            }
        }

    }
}
