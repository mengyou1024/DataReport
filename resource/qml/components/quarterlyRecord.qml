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
                text: qsTr("仪器型号:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.instrumentType
                placeholderText: "请输入仪器型号"
                onTextChanged: {
                    root_msg.instrumentType = text
                }
            }

            FluText {
                text: qsTr("打印日期:")
            }
            FluDatePicker {
                id: date_picker
                radius: 0
                normalColor: Qt.rgba(238/255,238/255,238/255)
                dividerColor: Qt.rgba(198/255,207/255,216/255)
                Component.onCompleted: {
                    var date = Date.fromLocaleString(Qt.locale("zh_CN"), root_msg.printDate, "yyyy-M-d")
                    var locale = Qt.locale("zh_CN")
                    current = date.toLocaleString(locale, "yyyy-M-dd")
                    yearText = date.toLocaleString(locale, "yyyy")
                    monthText = date.toLocaleString(locale, "M")
                    dayText = date.toLocaleString(locale, "d")
                }
            }

            FluText {
                text: qsTr("仪器编号:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.instrumentSerial
                placeholderText: "仪器编号"
                onTextChanged: {
                    root_msg.instrumentSerial = text
                }
            }

            FluText {
                text: qsTr("制造单位:")
            }
            FluTextBox {
                background: Rectangle {
                    border.color: Qt.rgba(191/255, 191/255,191/255)
                }
                text: root_msg.manufactureCompany
                placeholderText: "制造单位"
                onTextChanged: {
                    root_msg.manufactureCompany = text
                }
            }
            FluText {
                text: qsTr("制造日期:")
            }

            FluDatePicker {
                id: manufacture_date_picker
                radius: 0
                normalColor: Qt.rgba(238/255,238/255,238/255)
                dividerColor: Qt.rgba(198/255,207/255,216/255)
                Component.onCompleted: {
                    var date = Date.fromLocaleString(Qt.locale("zh_CN"), root_msg.manufactureDate, "yyyy-M-d")
                    var locale = Qt.locale("zh_CN")
                    current = date.toLocaleString(locale, "yyyy-M-dd")
                    yearText = date.toLocaleString(locale, "yyyy")
                    monthText = date.toLocaleString(locale, "M")
                    dayText = date.toLocaleString(locale, "d")
                }
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
                                Qt.locale("zh_CN"), "yyyy-M-d")
                } else {
                    root_msg.printDate = new Date().toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-M-d")
                }
                if (manufacture_date_picker.current) {
                    root_msg.manufactureDate = manufacture_date_picker.current.toLocaleString(
                                Qt.locale("zh_CN"), "yyyy-M-d")
                } else {
                    root_msg.manufactureDate = new Date().toLocaleString(
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
                model: PerformanceRecordView {
                    id: root_model
                    performanceRecordPtr: root_msg
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
        root_model.resetModel()
    }

    FileSelect {
        width: 560
        height: 160
        anchors.centerIn: parent
        id: root_filleSelect
        rootDir: dataDir
        dirType: "QuartPerformance"
        onAccepted: {
            root_msg.loadFile(filePath)
            flushTable()
        }
    }

    Component.onCompleted: {
        root_msg.loadFile("quarterlyRecord.bin")
        flushTable()
    }
}
