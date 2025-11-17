import QtQuick
import QtQuick.Controls

Item {
    id: root
    property int fileId: 0
    property int fileRuntime: 0
    property real pendingX: 0
    property real pendingY: 0
    property var playbacks: []
    signal setPlayed(int fileId, int played)

    function open(point, id, runtime, list) {
        pendingX = point.x
        pendingY = point.y
        playbacks = list
        fileId = id
        fileRuntime = runtime
        popup.open()
    }

    Popup {
        id: popup
        width: 250
        height: listView.contentHeight + 42
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Button {
            text: "Mark as played"
            width: popup.width - 14
            anchors.horizontalCenter: parent.horizontalCenter
            height: 24
            x: 2
            onClicked: {
                root.setPlayed(root.fileId, root.fileRuntime * 60)
                popup.close()
            }
        }

        Column {
            spacing: 2
            y: 30
            x: 2

            ListView {
                id: listView
                model: root.playbacks

                delegate: Rectangle {
                    id: playbackItem
                    height: 24
                    width: popup.width - 14
                    color: "white"

                    Row {
                        spacing: 2

                        Text {
                            text: modelData.time
                            width: playbackItem.width * 0.6
                            height: 24
                            leftPadding: 3
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            text: modelData.played
                            width: playbackItem.width * 0.4
                            height: 24
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight
                            rightPadding: 6
                        }
                    }
                }
            }
        }

        onAboutToShow: {
            x = Math.max(0, root.pendingX - width);
            y = Math.max(0, root.pendingY - height);
        }
    }
}

