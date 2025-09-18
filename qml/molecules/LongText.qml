import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    property string text: ""

    Text {
        anchors.fill: parent
        text: parent.text
        elide: Text.ElideRight
        clip: true

        ToolTip.visible: hovered
        ToolTip.text: parent.text

        property bool hovered: false


        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.hovered = true
            onExited: parent.hovered = false
        }
    }
}
