import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: "transparent"
    border.color: area.containsMouse ? "#80AAFF" : "transparent"

    property bool state: false
    property bool enabled: true
    signal toggle()

    MouseArea {
        id: area
        anchors.fill: parent
        enabled: root.enabled
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: {
            root.toggle()
        }
    }

    Image {
        anchors.centerIn: parent
        width: 14
        height: 14
        source: root.enabled ? root.state ? "qrc:/com/realdesert/ui/images/select-true.svg" : "qrc:/com/realdesert/ui/images/select-false.svg" : "qrc:/com/realdesert/ui/images/select-disabled.svg"
    }
}

