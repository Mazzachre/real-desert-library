import QtQuick
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent
    property string source
    property string name

    Image {
        anchors.centerIn: parent
        width: 14
        height: 14
        source: root.source
    }

    ToolTip.visible: area.containsMouse
    ToolTip.text: name

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true
    }
}
