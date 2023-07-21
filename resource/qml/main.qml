import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import mengyou.DataTableModel
import mengyou.Model
import FluentUI
import "components"

FluWindow {
    width: 1136
    height: 782
    minimumWidth: 1136
    minimumHeight: 782
    visible: true
    id: wnd_main
    title: qsTr("DataReport")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            Layout.alignment: Qt.AlignHCenter
            id: bar
            width: parent.width
            contentHeight: 40
            Repeater {
                model: ["发现缺陷记录表", "探伤记录表", "日常性能检查表", "季度性能检查表"]
                TabButton {
                    width: 120
                    height: 32
                    text: modelData

                    background: Rectangle {
                        radius: 4
                        border.color: "lightblue"
                        color: parent.checked ? Qt.rgba(
                                                    0, 112 / 255, 198 / 255, 1) : Qt.rgba(
                                                    254 / 255, 254 / 255,
                                                    254 / 255, 1)
                    }

                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "white" : "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        StackLayout {
            Layout.margins: 10
            id: stack_layout
            currentIndex: bar.currentIndex
            clip: true

            DiscoveryRecord {}
            InspectionRecord {}
            DailyRecord {}
            QuarterlyRecord {}

            onCurrentIndexChanged: {
                if (bar.currentIndex !== currentIndex) {
                    bar.currentIndex = currentIndex
                }
            }
        }
    }
    Component.onCompleted:{
        console.info("发现缺陷记录表\t:", mainDiscoveryFile)
        console.info("探伤记录表\t\t:", mainInspectionFile)
        console.info("日常性能检查表\t:", mainDailyFile)
        console.info("季度性能检查表\t:", mainQuarterlyFile)
    }
    ExcelRunner {
        id: root_excelRunner
    }
}
