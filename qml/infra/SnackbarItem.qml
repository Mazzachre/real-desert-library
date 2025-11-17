import QtQuick
import QtQuick.Controls

Item {
    id: root
    property alias text: label.text
    signal closed()

    width: 400
    height: 30

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#c0ff4500"
        radius: 4

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            Text {
                id: label
                text: root.text
                color: "white"
                wrapMode: Text.Wrap
                width: parent.width - 50
                height: parent.height
            }

            Button {
                width: 30
                height: parent.height
                text: "Close"
                onClicked: root.closed()
            }
        }
    }

    Timer {
        interval: 6000
        running: true
        onTriggered: {
            root.closed()
        }
    }
}

