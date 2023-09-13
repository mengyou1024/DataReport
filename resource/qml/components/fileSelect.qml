import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import "."
import QtQuick.Templates as T

Dialog {
    property string rootDir: ""
    property string dirType: ""
    property string filePath: ""
    modal: true
    closePolicy: Popup.CloseOnEscape
    T.Overlay.modal: Rectangle {
        color: "#90909090"
    }

    background: Rectangle {
        color: "#FFFFFF"
        BorderImage {
            source: "qrc:/resource/img/bkimage.png"
            anchors.fill: parent
            anchors.margins: 10
            border.left: 5
            border.top: 5
            border.right: 5
            border.bottom: 5
        }
    }

    Rectangle {
        x: 12
        width: 60
        height: 20
        color: "#FFFFFF"
        Text {
            text: qsTr("数据列表")
            font.pixelSize: 14
        }
    }

    Rectangle {
        //        color: "red"
        anchors.fill: parent
        anchors.margins: 20
        ColumnLayout {
            GridLayout {
                columns: 3
                Text {
                    text: qsTr("年/月")
                    font.pixelSize: 14
                }
                Text {
                    text: qsTr("日期")
                    font.pixelSize: 14
                }
                Text {
                    text: qsTr("时间")
                    font.pixelSize: 14
                }
                FolderComboBox {
                    id: folder_yearMonth
                    folder: rootDir + "/" + dirType
                    onFolderChanged: {
                        console.log("folder_yearMonth.folder", folder_yearMonth.folder)
                    }
                }
                FolderComboBox {
                    id: folder_day
                    folder: folder_yearMonth.folder + "/" + folder_yearMonth.currentText
                    onFolderChanged: {
                        console.log("folder_day.folder", folder_day.folder)
                    }
                }
                FolderComboBox {
                    implicitWidth: 240
                    showFiles: true
                    showDirs: false
                    currentIndex: 0
                    id: folder_time
                    folder: folder_day.folder + "/" + folder_day.currentText
                    onFolderChanged: {
                        console.log("folder_time.folder", folder_time.folder)
                    }
                }
            }
            RowLayout {
                Layout.topMargin: 20
                Layout.alignment: Qt.AlignCenter

                FluButton {
                    text: qsTr("确定")
                    font.bold: true
                    hoverColor: "#1874cd"
                    normalColor: "#eeeeee"
                    background: Rectangle {
                        color: parent.hovered ? parent.hoverColor : parent.normalColor
                    }
                    onClicked: {
                        filePath = String(folder_time.folder + "/" + folder_time.currentText).substring(8)
                        accept()
                    }
                }
                FluButton {
                    text: qsTr("取消")
                    font.bold: true
                    hoverColor: "#1874cd"
                    normalColor: "#eeeeee"
                    background: Rectangle {
                        color: parent.hovered ? parent.hoverColor : parent.normalColor
                    }
                    onClicked: {
                        reject()
                    }
                }
            }
        }
    }
}
