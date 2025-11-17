import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

ListView {
    id: snackbarList
    width: 400
    height: contentHeight
    spacing: 6
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    model: Warnings
    interactive: false
    clip: true

    delegate: SnackbarItem {
        id: snackbarItem
        text: model.text
        onClosed: Warnings.close(index)
    }
}

