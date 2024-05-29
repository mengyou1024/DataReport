import QtQuick
import QtQuick.Controls
import Qt.labs.folderlistmodel

ComboBox {
    implicitWidth: 120
    implicitHeight: 32
    property alias folder: folder.folder
    property alias showFiles: folder.showFiles
    property alias showDirs: folder.showDirs
    model: FolderListModel {
        showFiles: false
        id: folder
        sortReversed: true
    }
    currentIndex: 0
    textRole: "fileName"
    valueRole: "fileName"
}
