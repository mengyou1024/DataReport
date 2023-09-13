// 车轮超声波探伤记录表
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
    InspectionRecordMsg {
        id: root_msg
        onDetectDateChanged: {
            var _date = Date.fromLocaleString(Qt.locale(), root_msg.detectDate, "yyyy-M-d")
            date_picker.yearText = _date.toLocaleString(Qt.locale(), "yyyy")
            date_picker.monthText = _date.toLocaleString(Qt.locale(), "M")
            date_picker.dayText = _date.toLocaleString(Qt.locale(), "d")
        }
    }
    ScrollView {
        Layout.alignment: Qt.AlignHCenter
        GridLayout {
            columns: 8
            columnSpacing: 10
            rowSpacing: 10
            FluText {
                text: qsTr("单位名称:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.companyName
                placeholderText: "请输入单位名称"
                onTextChanged: {
                    root_msg.companyName = text
                }
            }

            FluText {
                text: qsTr("设备名称:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.deviceName
                placeholderText: "请输入设备名称"
                onTextChanged: {
                    root_msg.deviceName = text
                }
            }

            FluText {
                text: qsTr("探伤日期:")
            }
            FluDatePicker {
                Layout.columnSpan: 3
                radius: 0
                normalColor: Qt.rgba(238/255,238/255,238/255)
                dividerColor: Qt.rgba(198/255,207/255,216/255)
                id: date_picker
            }

            FluText {
                text: qsTr("设备型号:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.deviceType
                placeholderText: "请输入设备型号"
                onTextChanged: {
                    root_msg.deviceType = text
                }
            }
            FluText {
                text: qsTr("设备编号:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.deviceSerial
                placeholderText: "请输入设备编号"
                onTextChanged: {
                    root_msg.deviceSerial = text
                }
            }
            FluText {
                text: qsTr("探伤标准:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.detectStandard
                Layout.columnSpan: 3
                placeholderText: "请输入探伤标准"
                onTextChanged: {
                    root_msg.detectStandard = text
                }
            }
            FluText {
                text: qsTr("探伤内容:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.detectContent
                placeholderText: "请输入探伤内容"
                onTextChanged: {
                    root_msg.detectContent = text
                }
            }
            FluText {
                text: qsTr("探伤区域:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.detectRegion
                placeholderText: "请输入探伤区域"
                onTextChanged: {
                    root_msg.detectRegion = text
                }
            }

            FluText {
                text: qsTr("探头:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.probe
                placeholderText: "探头"
                onTextChanged: {
                    root_msg.probe = text
                }
            }

            FluText {
                text: qsTr("声速:")
            }

            RowLayout {
                FluTextBox {
                    background: Rectangle {
                        border.color: Qt.rgba(191/255, 191/255,191/255)
                    }
                    text: root_msg.soundSpeed
                    validator: IntValidator {
                        bottom: 0
                    }
                    placeholderText: "0"
                    onTextChanged: {
                        root_msg.soundSpeed = text
                    }
                }
                FluText {
                    text: "m/s"
                }
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 10
        FileDialog {
            id: save_dialog
            currentFile: "file:///" + qsTr("探伤记录表")
            title: "导出表格"
            nameFilters: ["Excel file (*.xlsx *.xls)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                if (date_picker.current) {
                    root_msg.detectDate = date_picker.current.toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-M-d")
                } else {
                    root_msg.detectDate = new Date().toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-M-d")
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
            font.bold: true
            normalColor: "lightblue"
            hoverColor: Qt.lighter("lightblue", 1.2)
            background: Rectangle {
                color: parent.hovered? parent.hoverColor: parent.normalColor
            }
            onClicked: {
                save_dialog.open()
            }
        }

        FluButton {
            text: qsTr("打开文件")
            font.bold: true
            hoverColor: "#1874cd"
            normalColor: "#eeeeee"
            background: Rectangle {
                color: parent.hovered? parent.hoverColor: parent.normalColor
            }
            onClicked: {
//                flushTable()
                root_filleSelect.open()
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignCenter
        spacing: 5
        FluText {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("探伤记录")
            font.pointSize: 16
        }
        ScrollView {
            Layout.fillHeight: true
            Layout.preferredWidth: 1066
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
                model: InspectionRecordView {
                    id: root_model
                    datNum: root_msg.recordNum
                    datptr: root_msg.recordPtr
                }
                delegate: Rectangle {
                    implicitHeight: 36
                    implicitWidth: 96
                    color: Qt.rgba(238/255,238/255,238/255)
                    border.color: Qt.rgba(198/255,207/255,216/255)
                    FluText {
                        anchors.centerIn: parent
                        text: display
                    }
                }
            }
        }
    }

    function flushTable() {
        root_model.datNum = root_msg.recordNum
        root_model.datptr = root_msg.recordPtr
        root_model.resetModel()
    }

    FileSelect {
        width: 560
        height: 160
        anchors.centerIn: parent
        id: root_filleSelect
        rootDir: dataDir
        dirType: "Scan"
        onAccepted: {
            root_msg.loadFile(filePath)
            flushTable()
        }
    }

    Component.onCompleted: {
        root_msg.loadFile("inspectionRecord.bin")
        flushTable()
    }
}
