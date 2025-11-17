import QtQuick
import QtQuick.Controls
Item {
    id: root
    anchors.fill: parent
    property string display
    property string name

    Text {
        id: text;
        anchors.centerIn: parent
        clip: true
        text: root.display
    }

    ToolTip.visible: area.containsMouse
    ToolTip.text: name

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true
    }
}

