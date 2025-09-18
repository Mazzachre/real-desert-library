import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    id: root
    color: "#FFFFFF"
    border.color: area.containsMouse ? "#80AAFF" : "#FFFFFF"

    property bool state
    signal toggle()

    MouseArea {
        id: area
        anchors.fill: parent
        enabled: true
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            root.toggle()
        }
    }

    Image {
        anchors.centerIn: parent
        width: 14
        height: 14
        source: root.state ? "qrc:/com/realdesert/ui/images/select-true.svg" : "qrc:/com/realdesert/ui/images/select-false.svg"
    }
}

