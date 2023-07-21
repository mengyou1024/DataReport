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
                text: qsTr("探伤日期:")
            }

            FluDatePicker {
                Layout.columnSpan: 3
                id: date_picker
            }

            FluText {
                text: qsTr("工作频率:")
                Layout.alignment: Qt.AlignCenter
            }

            RowLayout {
                FluTextBox {
                    text: root_msg.workFreq
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
                Layout.alignment: Qt.AlignCenter
            }

            FluTextBox {
                text: root_msg.probe
                placeholderText: "探头类型"
                onTextChanged: {
                    root_msg.probe = text
                }
            }

            FluText {
                text: qsTr("耦合方式:")
                Layout.alignment: Qt.AlignCenter
            }
            FluTextBox {
                text: root_msg.coupledMode
                placeholderText: "耦合方式"
                onTextChanged: {
                    root_msg.coupledMode = text
                }
            }

            FluText {
                text: qsTr("车轮型号:")
                Layout.alignment: Qt.AlignCenter
            }

            FluTextBox {
                text: root_msg.wheelType
                placeholderText: "车轮型号"
                onTextChanged: {
                    root_msg.wheelType = text
                }
            }

            FluText {
                text: qsTr("车轮编号:")
                Layout.alignment: Qt.AlignCenter
            }

            FluTextBox {
                text: root_msg.wheelSerial
                placeholderText: "车轮编号"
                onTextChanged: {
                    root_msg.wheelSerial = text
                }
            }
            FluText {
                text: qsTr("炉号:")
                Layout.alignment: Qt.AlignCenter
            }

            FluTextBox {
                text: root_msg.heatSerial
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
                    model: DefectRecordView {
                        id: der_table
                        datNum: root_msg.defectsNum
                        datptr: root_msg.defectEchoRecord
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

        ColumnLayout {
            FluText {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("底波衰减记录")
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
                    model: DefectRecordView {
                        id: bwar_table
                        datNum: root_msg.defectsNum
                        datptr: root_msg.bottomWaveAttenuationRecord
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
    }

    function flushTable() {
        der_table.datptr = root_msg.defectEchoRecord
        der_table.datNum = root_msg.defectsNum
        bwar_table.datptr = root_msg.bottomWaveAttenuationRecord
        bwar_table.datNum = root_msg.defectsNum
        der_table.resetModel()
        bwar_table.resetModel()
    }

    Component.onCompleted: {
        root_msg.loadFile("discoveryRecord.bin")
        flushTable()
    }
}
