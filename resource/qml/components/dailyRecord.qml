// 车轮超声波自动探伤机日常性能检查记录表
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
    DailyRecordMsg {
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
                text: qsTr("探伤设备:")
            }
            FluTextBox {
                id: _detectDeive
                text: root_msg.detectDeive
                placeholderText: "请输入设备名称"
                onTextChanged: {
                    root_msg.detectDeive = text
                }
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("探伤日期:")
            }
            FluDatePicker {
                id: date_picker
            }

            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("探头:")
            }
            FluTextBox {
                id: _probe
                text: root_msg.probe
                placeholderText: "请输入探头"
                onTextChanged: {
                    root_msg.probe = text
                }
            }
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("耦合方式:")
            }
            FluTextBox {
                text: root_msg.coupledMode
                placeholderText: "请输入耦合方式"
                onTextChanged: {
                    root_msg.coupledMode = text
                }
            }
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("车轮型号:")
            }
            FluTextBox {
                id: _wheelType
                text: root_msg.wheelType
                placeholderText: "请输入车轮型号"
                onTextChanged: {
                    root_msg.wheeelType = text
                }
            }
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("车轮编号:")
            }
            FluTextBox {
                id: _wheelSerial
                text: root_msg.wheelSerial
                placeholderText: "请输入车轮编号"
                onTextChanged: {
                    root_msg.wheelSerial = text
                }
            }
            FluText {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("炉号:")
            }
            FluTextBox {
                id: _heatSerial
                text: root_msg.heatSerial
                placeholderText: "请输入炉号"
                onTextChanged: {
                    root_msg.heatSerial = text
                }
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 10

        FileDialog {
            id: save_dialog
            currentFile: "file:///" + qsTr("日常性能检查表")
            title: "导出表格"
            nameFilters: ["Excel file (*.xlsx *.xls)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                if (date_picker.current) {
                    root_msg.detectDate = date_picker.current.toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-MM-dd")
                } else {
                    root_msg.detectDate = new Date().toLocaleString(
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
                console.log("root_msg.coupledMode", root_msg.coupledMode)
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignCenter
        spacing: 5

        FluText {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("缺陷网表记录")
            font.pointSize: 16
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.preferredWidth: 610

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
                model: DefectRecordView {
                    id: root_model
                    datNum: root_msg.recordNum
                    datptr: root_msg.recordPtr
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
        root_model.datNum = root_msg.defectsNum
        root_model.datptr = root_msg.defectsPtr
        root_model.resetModel()
    }

    Component.onCompleted: {
        root_msg.loadFile("dailyRecord.bin")
        flushTable()
    }
}
