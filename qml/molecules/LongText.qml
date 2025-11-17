import QtQuick
import QtQuick.Controls

Item {
    id: root
    property string text: ""
    property bool bold: false;
    readonly property bool containsMouse: area.containsMouse
    height: text.contentHeight

    Text {
        id: text
        anchors.fill: parent
        text: root.text
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        clip: true
        font.bold: root.bold

        ToolTip.visible: area.containsMouse
        ToolTip.text: root.text

        MouseArea {
            id: area
            anchors.fill: parent
            hoverEnabled: true
        }
    }
}
