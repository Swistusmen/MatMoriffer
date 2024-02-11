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
    property var maxValue: 100

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
                    width: root.width * 0.7
                    height: 500
                    cellWidth: width / 3
                    cellHeight: height / 7
                    anchors.top: chart.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    model: gridModel

                    delegate: Rectangle {
                        width: gridView.cellWidth
                        height: gridView.cellHeight
                        clip: true
                        color: index % 2 === 0 ? "lightgray" : "white"
                        border.color: "black"
                        Text {
                            anchors.centerIn: parent
                            text: model.text
                        }
                    }
                }

                ListModel {
                    id: gridModel
                    ListElement { text: "ip" }
                    ListElement { text: "domain" }
                    ListElement { text: "count" }
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
            root.maxValue=myGod[0]*1.1
            root.values = myGod;
        }

        function onDomainForStatistics(domains){
            root.dm=domains
        }

        function onReloadStats(){
            mySeries.axisX.categories = convertToQStringList(root.categories)
            mySeries.axisX.categories= root.categories.slice(0, Math.min(10, root.categories.length))
            mySeries.insert(0, "ips", root.values)
            mySeries.axisY.min = 0
            mySeries.axisY.max = root.maxValue

            for(var i=0;i<root.dm.length;i++){
                gridModel.append({text:root.categories[i]})
                 gridModel.append({text:root.dm[i]})
                 gridModel.append({text:root.values[i].toString()})
            }
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
