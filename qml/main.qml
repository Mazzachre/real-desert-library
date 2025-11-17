import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

ApplicationWindow {

    Page {
        anchors.fill: parent

        header: ToolBar {
            RowLayout {
                anchors.fill: parent

                TabBar {
                    Layout.fillWidth: true
                    currentIndex: ModeHandler.modeTabIndex()

                    onCurrentIndexChanged: ModeHandler.setModeByIndex(currentIndex)

                    TabButton { text: "TV Shows" }
                    TabButton { text: "Movies" }
                }

                ToolButton {
                    icon.source: "qrc:/com/realdesert/ui/images/config.svg"
                    icon.width: 18
                    icon.height: 18
                    onClicked: console.log("Settings");
                }
            }
        }

        Loader {
            objectName: "mainLoader"
            anchors.fill: parent
            anchors.margins: 4
            anchors.bottomMargin: 10
        }
    }

    DropArea {
        anchors.fill: parent

        onEntered: function(drag) {
            drag.accepted = drag.hasUrls && FileHandler.acceptDrag(drag.urls)
        }
        onDropped: function(drag) {
            FileHandler.filesDropped(drag.urls)
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#80000000"
        visible: BusyIndication.busy
        z: 9999

        ProgressBar {
            anchors.centerIn: parent
            indeterminate: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: true
            hoverEnabled: true
            preventStealing: true
            propagateComposedEvents: false
            acceptedButtons: Qt.AllButtons
        }

        Behavior on visible {
            NumberAnimation { duration: 200 }
        }
    }

    Overlay.overlay.children: [
        SnackbarList {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            z: 9999
        }
    ]

    Blocker {}
}
