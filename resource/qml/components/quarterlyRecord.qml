// 车轮超声波自动探伤机季度性能检查表
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import FluentUI
import mengyou.Model

ColumnLayout {
    id: root_layout
    Layout.preferredHeight: parent.height
    Layout.preferredWidth: parent.width
    QuarterlyRecordMsg {
        id: root_msg
    }
    ScrollView {
        Layout.alignment: Qt.AlignHCenter
        GridLayout {
            columns: 6
            columnSpacing: 10
            rowSpacing: 10
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("单位名称:")
            }
            FluTextBox {
                text: root_msg.companyName
                placeholderText: "请输入单位名称"
                onTextChanged: {
                    root_msg.companyName = text
                }
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("仪器型号:")
            }
            FluTextBox {
                text: root_msg.instrumentType
                placeholderText: "请输入仪器型号"
                onTextChanged: {
                    root_msg.instrumentType = text
                }
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("打印日期:")
            }
            FluDatePicker {
                id: date_picker
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("仪器编号:")
            }
            FluTextBox {
                text: root_msg.instrumentSerial
                placeholderText: "仪器编号"
                onTextChanged: {
                    root_msg.instrumentSerial = text
                }
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("制造单位:")
            }
            FluTextBox {
                text: root_msg.manufactureCompany
                placeholderText: "制造单位"
                onTextChanged: {
                    root_msg.manufactureCompany = text
                }
            }
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("制造日期:")
            }

            FluDatePicker {
                id: manufacture_date_picker
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 10

        FileDialog {
            id: save_dialog
            currentFile: "file:///" + qsTr("季度性能检查表")
            title: "导出表格"
            nameFilters: ["Excel file (*.xlsx *.xls)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                if (date_picker.current) {
                    root_msg.printDate = date_picker.current.toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-MM-dd")
                } else {
                    root_msg.printDate = new Date().toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-MM-dd")
                }
                if (manufacture_date_picker.current) {
                    root_msg.manufactureDate = manufacture_date_picker.current.toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-MM-dd")
                } else {
                    root_msg.manufactureDate = new Date().toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-MM-dd")
                }

                if (root_msg.saveFile(String(currentFile).substring(8))) {
                    showSuccess(qsTr("保存文件成功"))
                    root_excelRunner.openExcel(String(currentFile).substring(8))
                } else {
                    showError(qsTr("保存文件失败, 请先关闭文件再导出"))
                }
            }
        }

        FluButton {
            text: qsTr("导出表")
            normalColor: "lightblue"
            hoverColor: Qt.lighter("lightblue", 1.2)
            onClicked: {
                save_dialog.open()
            }
        }

        FluButton {
            text: qsTr("刷新")
            onClicked: {
                flushTable()
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignCenter
        spacing: 5
        FluText {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("五大性能")
            font.pointSize: 16
        }
        ScrollView {
            Layout.fillHeight: true
            Layout.preferredWidth: 610
            Layout.alignment: Qt.AlignHCenter

            ScrollBar.vertical: FluScrollBar {
                parent: parent
                x: parent.mirrored ? 0 : parent.width - width
                y: parent.topPadding
                height: parent.availableHeight
            }
            ScrollBar.horizontal: FluScrollBar {
                parent: parent
                x: parent.bottomPadding
                y: parent.mirrored ? 0 : parent.height - height
                width: parent.availableWidth
            }

            TableView {
                clip: true
                columnSpacing: 4
                rowSpacing: 2
                model: PerformanceRecordView {
                    id: root_model
                    performanceRecordPtr: root_msg
                }
                delegate: Rectangle {
                    implicitHeight: 36
                    implicitWidth: 96
                    radius: 4
                    FluText {
                        anchors.centerIn: parent
                        text: display
                    }
                }
            }
        }
    }

    function flushTable() {
        root_model.resetModel()
    }

    Component.onCompleted: {
        root_msg.loadFile("quarterlyRecord.bin")
        flushTable()
    }
}
