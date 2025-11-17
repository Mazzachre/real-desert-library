import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: root
    property int itemId: 0
    property var loadPath: ""
    property string subtitle: ""
    property real pendingX: 0
    property real pendingY: 0
    signal setSubtitle(int id, string subtitle)

    function open(point, id, path, current, fileName) {
        pendingX = point.x;
        pendingY = point.y;
        itemId = id
        loadPath = path
        subtitle = current
        file.title = fileName
        if (subtitle != "") {
            replaceMenu.open()
        } else {
            addMenu.open()
        }
    }

    Menu {
        id: replaceMenu

        MenuItem {
            text: root.subtitle
            enabled: false
        }

        MenuItem {
            text: "Remove"
            onTriggered: {
                root.setSubtitle(root.itemId, "")
            }
        }

        MenuItem {
            text: "Replace"
            onTriggered: {
                file.open()
            }
        }

        onAboutToShow: {
            x = Math.max(0, root.pendingX - width);
            y = Math.max(0, root.pendingY - height);
        }
    }

    Menu {
        id: addMenu

        MenuItem {
            text: "Add"
            onTriggered: {
                file.open()
            }
        }

        onAboutToShow: {
            x = Math.max(0, root.pendingX - width);
            y = Math.max(0, root.pendingY - height);
        }
    }

    FileDialog {
        id: file
        title: "Select Subtitle File"
        currentFolder: root.loadPath
        options: FileDialog.ReadOnly | FileDialog.DontUseNativeDialog
        nameFilters: ["Subtitle Files (*.srt *.idx)", "All Files (*)"]
        onAccepted: root.setSubtitle(root.itemId, selectedFile)
    }
}
