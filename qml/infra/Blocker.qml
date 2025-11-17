import QtQuick
import QtQuick.Controls
import com.realdesert 1.0

Rectangle {
    anchors.fill: parent
    color: "#80000000"
    visible: Blocker.block

    MouseArea {
        anchors.fill: parent
        enabled: true
        propagateComposedEvents: false
        preventStealing: true
        acceptedButtons: Qt.AllButtons
    }
}
