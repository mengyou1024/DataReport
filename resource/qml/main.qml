import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import mengyou.DataTableModel
import mengyou.Model
import mengyou.WaveView
import FluentUI
import "components"

FluWindow {
    width: 1136
    height: 782
    minimumWidth: 1136
    minimumHeight: 782
    visible: true
    id: wnd_main
    title: qsTr("数据查看")
    backgroundColor: "#FEFEFE"

    property string dataDir: "file:///" + APP_DATA_DIR

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        TabBar {
            Layout.alignment: Qt.AlignHCenter
            id: bar
            width: parent.width
            contentHeight: 40
            Repeater {
                model: ["发现缺陷记录表", "探伤记录表", "日常性能检查表", "季度性能检查表", /*"波形图"*/]
                TabButton {
                    width: 160
                    height: 32
                    text: modelData
                    background: Rectangle {
                        border.color: Qt.rgba(192 / 255, 207 / 255, 216 / 255)
                        color: parent.checked ? Qt.rgba(24 / 255, 116 / 255,
                                                        205 / 255) : Qt.rgba(
                                                    238 / 255, 238 / 255,
                                                    238 / 255)
                    }

                    contentItem: Text {
                        text: parent.text
                        font.bold: true
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
//            WaveView {}

            onCurrentIndexChanged: {
                if (bar.currentIndex !== currentIndex) {
                    bar.currentIndex = currentIndex
                }
            }
        }
    }

    ExcelRunner {
        id: root_excelRunner
    }

    Component.onCompleted: {
        console.info("发现缺陷记录表\t:", mainDiscoveryFile)
        console.info("探伤记录表\t\t:", mainInspectionFile)
        console.info("日常性能检查表\t:", mainDailyFile)
        console.info("季度性能检查表\t:", mainQuarterlyFile)
//        root_filleSelect.open()
    }
}
