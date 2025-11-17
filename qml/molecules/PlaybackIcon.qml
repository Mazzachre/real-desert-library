import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    property bool played: false
    property bool playedFully: false
    color: "transparent"
    border.color: playbackArea.containsMouse ? "#80AAFF" : "transparent"

    signal clicked();

    MouseArea {
        id: playbackArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked();
    }

    Image {
        visible: root.played || root.playedFully
        anchors.centerIn: parent
        width: 14
        height: 14
        source: root.playedFully ? "qrc:/com/realdesert/ui/images/finished.svg" : "qrc:/com/realdesert/ui/images/started.svg"
    }
}
