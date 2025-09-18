import QtQuick 6.4
import QtQuick.Layouts 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Window {

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
                    onClicked: console.log("Settings")
                }
            }
        }

        Loader {
            objectName: "mainLoader"
            anchors.fill: parent
            anchors.margins: 2
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

    //TODO Handle busy

    //TODO Handle error popup
}
