import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    property alias title: header.text
    property bool folded: true
    property Component content: null

    color: "transparent"
    border.width: 0.5
    border.color: "black"
    radius: 3
    height: 24 + (folded ? 0 : contentLoader.implicitHeight + 2)

    Item {
        width: parent.width
        height: 24

        Image {
            x: 2
            y: 2
            width: 20
            height: 20
            source: root.folded ? "qrc:/com/realdesert/ui/images/acc-closed.svg" : "qrc:/com/realdesert/ui/images/acc-open.svg"
        }

        Text {
            id: header
            anchors.verticalCenter: parent.verticalCenter
            x: 26
            text: "Untitled"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.folded = !root.folded
            cursorShape: Qt.PointingHandCursor
        }
    }

    Loader {
        id: contentLoader
        width: parent.width - 5
        x: 2
        y: 24
        sourceComponent: root.content
        visible: !root.folded
    }
}
