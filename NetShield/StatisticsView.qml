import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Layouts 1.15
import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

Item {
    id: root
    property var categories
    property var values
    property var dm

    ColumnLayout {
        id: mainLayout
        width: root.width
        height: root.height
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

        ScrollView {
            id: mainScrollView
            anchors.fill: parent
            width: root.width
            height: chart.height+gridView.height+ loadData.height
            enabled: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            clip:true

            Item{
                id: l
                height:chart.height+gridView.height+ loadData.height
                implicitHeight: height
                width: parent.width

                    Button{
                        id: loadData
                        text:"Load Statistics"
                        width: root.width*0.2
                        anchors.right: parent.right
                        height: root.height*0.1
                        onClicked: {
                            interMessageBroker.loadDataForStatistics()
                        }
                    }

                ChartView {
                    id: chart
                    title: "Bar series"
                    width: root.width
                    height: root.height * 0.8
                    anchors.top: loadData.bottom
                    legend.alignment: Qt.AlignBottom
                    antialiasing: true

                    BarSeries {
                        id: mySeries
                        axisX: BarCategoryAxis {}
                    }
                }

                GridView {
                    id: gridView
                    width: root.width * 0.7 // Ustaw szerokość tablicy
                    height: 500
                    cellWidth: width / 3 // Szerokość komórki
                    cellHeight: height / 7 // Wysokość komórki
                    anchors.top: chart.bottom // Umieść siatkę pod wykresem
                    anchors.horizontalCenter: parent.horizontalCenter // Wyśrodkuj siatkę poziomo
                    model: 21 // Liczba komórek (3 kolumny * 7 wierszy = 21)

                    delegate: Rectangle {
                        width: gridView.cellWidth
                        height: gridView.cellHeight
                        color: index % 2 === 0 ? "lightgray" : "white" // Ustawienie koloru tła na przemian
                        border.color: "black"
                        Text {
                            anchors.centerIn: parent
                            text: (index + 1).toString() // Numerowanie komórek od 1
                        }
                    }
                }
            }

        }
    }

    Connections{
        target: interMessageBroker



        function onIPForStatistics(ips){
            root.categories=ips
        }

        function onCountForStatistics(counts){
            var myGod = counts.slice()

            root.values = myGod;
        }

        function onDomainForStatistics(domains){
            root.dm=domains
        }

        function onReloadStats(){
            mySeries.axisX.categories = convertToQStringList(root.categories)
            mySeries.insert(0, "ips", root.values)
            mySeries.axisY.min = 0
            mySeries.axisY.max = 100
        }

    }

    function convertToQStringList(vector) {
        var stringList = [];
        for (var i = 0; i < vector.length; ++i) {
            stringList.push(vector[i]);
        }
        return stringList;
    }
}
