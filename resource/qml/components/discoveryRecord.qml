// 车轮超声波探伤发现缺陷记录表
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import mengyou.Model
import FluentUI

ColumnLayout {
    id: root_layout
    Layout.preferredHeight: parent.height
    Layout.preferredWidth: parent.width
    DiscoveryRecordMsg {
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
            columns: 6
            columnSpacing: 10
            rowSpacing: 10
            FluText {
                text: qsTr("单位名称:")
            }
            FluTextBox {
                text: root_msg.companyName
                placeholderText: "请输入单位名称"
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                onTextChanged: {
                    root_msg.companyName = text
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
                text: qsTr("工作频率:")
            }

            RowLayout {
                FluTextBox {
                    text: root_msg.workFreq
                    background: Rectangle {
                        border.color: Qt.rgba(191/255, 191/255,191/255)
                    }
                    validator: DoubleValidator {
                        top: 100.0
                        bottom: 0.0
                        decimals: 2
                    }
                    placeholderText: "0"
                    onTextChanged: {
                        root_msg.workFreq = text
                    }
                }
                FluText {
                    text: "MHz"
                }
            }
            FluText {
                text: qsTr("探头:")
            }

            FluTextBox {
                text: root_msg.probe
                placeholderText: "探头类型"
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                onTextChanged: {
                    root_msg.probe = text
                }
            }

            FluText {
                text: qsTr("耦合方式:")
            }
            FluTextBox {
                text: root_msg.coupledMode
                placeholderText: "耦合方式"
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                onTextChanged: {
                    root_msg.coupledMode = text
                }
            }

            FluText {
                text: qsTr("车轮型号:")
            }

            FluTextBox {
                text: root_msg.wheelType
                placeholderText: "车轮型号"
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                onTextChanged: {
                    root_msg.wheelType = text
                }
            }

            FluText {
                text: qsTr("车轮编号:")
            }

            FluTextBox {
                text: root_msg.wheelSerial
                placeholderText: "车轮编号"
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                onTextChanged: {
                    root_msg.wheelSerial = text
                }
            }
            FluText {
                text: qsTr("炉号:")
            }

            FluTextBox {
                text: root_msg.heatSerial
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                placeholderText: "炉号"
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
            currentFile: "file:///" + qsTr("发现缺陷记录表")
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

    FluFlipView {
        Layout.fillHeight: true
        Layout.preferredWidth: 720
        Layout.alignment: Qt.AlignCenter
        ColumnLayout {
            FluText {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("缺陷回波记录")
                font.pointSize: 16
                font.bold: true
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
                    model: DefectRecordView {
                        id: der_table
                        datNum: root_msg.defectsNum
                        datptr: root_msg.defectEchoRecord
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

        ColumnLayout {
            FluText {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("底波衰减记录")
                font.bold: true
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
                    model: DefectRecordView {
                        id: bwar_table
                        datNum: root_msg.defectsNum
                        datptr: root_msg.bottomWaveAttenuationRecord
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
    }

    function flushTable() {
        der_table.datptr = root_msg.defectEchoRecord
        der_table.datNum = root_msg.defectsNum
        bwar_table.datptr = root_msg.bottomWaveAttenuationRecord
        bwar_table.datNum = root_msg.defectsNum
        der_table.resetModel()
        bwar_table.resetModel()
    }

    FileSelect {
        width: 560
        height: 160
        anchors.centerIn: parent
        id: root_filleSelect
        rootDir: dataDir
        dirType: "Scan"
        onAccepted: {
            console.log("rootDir", rootDir)
            console.log("dirType", dirType)
            root_msg.loadFile(filePath)
            flushTable()
        }
    }

    Component.onCompleted: {
        root_msg.loadFile("discoveryRecord.bin")
        flushTable()
    }
}
