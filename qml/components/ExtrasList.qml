import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Row {
        spacing: 5
        width: parent.width - 23
        height: 24

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"
        }

        Rectangle {
            width: (parent.width * 0.80) - 15
            height: 24
            color: "lightgray"

            Text {
                anchors.centerIn: parent
                text: "Name"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            HeaderIcon {
                source: "qrc:/com/realdesert/ui/images/playback.svg"
                name: "Playback"
            }
        }

        Rectangle {
            width: parent.width * 0.1
            height: 24
            color: "lightgray"

            HeaderIcon {
                source: "qrc:/com/realdesert/ui/images/time.svg"
                name: "Runtime"
            }
        }
    }

    ListView {
        y: 29
        height: parent.height - 29
        width: parent.width

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: ScrollBar.AlwaysOn
        }
        clip: true
        model: ExtrasUI
        spacing: 5

        delegate: Rectangle {
            id: delegate
            height: 24
            width: parent ? parent.width - 21 : 0
            radius: 3
            color: ExtrasUI.selected == id ? "#FFD580" : "#FFFFFF"
            property bool editing: false

            Row {
                spacing: 2
                anchors.fill: parent

                Button {
                    width: parent.width * 0.05
                    height: parent.height

                    Image {
                        anchors.centerIn: parent
                        width: 14
                        height: 14
                        source: "qrc:/com/realdesert/ui/images/edit-field.svg"
                    }

                    onClicked: delegate.editing = !delegate.editing
                }

                Rectangle {
                    visible: !delegate.editing
                    width: (parent.width * 0.80) - 15
                    height: parent.height
                    color: "transparent"
                    border.color: nameArea.containsMouse ? "#FFD580" : "#FFFFFF"

                    MouseArea {
                        id: nameArea
                        anchors.fill: parent
                        enabled: true
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            ExtrasUI.selected = model.id
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        text: model.name
                    }
                }

                TextField {
                    visible: delegate.editing
                    width: (parent.width * 0.80) - 10
                    height: parent.height
                    text: model.name
                    focus: true
                    Keys.onReturnPressed: {
                        ExtrasUI.updateName(model.id, text);
                        onClicked: delegate.editing = false
                    }
                    Keys.onEscapePressed: {
                        onClicked: delegate.editing = false
                    }
                }

                PlaybackIcon {
                    id: playbackIcon
                    width: parent.width * 0.05
                    height: parent.height
                    played: model.played
                    playedFully: model.playedFully

                    onClicked: {
                        const point = playbackArea.mapToItem(root, 0, playbackIcon.height);
                        playbackList.open(point, model.playbackList)
                    }
                }

                // Item {
                //     width: parent.width * 0.05
                //     height: parent.height
                //
                //     MouseArea {
                //         id: playbackArea
                //         anchors.fill: parent
                //         enabled: model.played
                //         hoverEnabled: true
                //         cursorShape: Qt.PointingHandCursor
                //         onClicked: {
                //             const point = playbackArea.mapToItem(root, 0, playbackArea.height);
                //             playbackList.open(point, model.playbackList)
                //         }
                //     }
                //
                //
                //     Image {
                //         visible: model.played || model.playedFully
                //         anchors.centerIn: parent
                //         width: 14
                //         height: 14
                //         source: model.playedFully ? "qrc:/com/realdesert/ui/images/finished.svg" : "qrc:/com/realdesert/ui/images/started.svg"
                //     }
                // }

                Item {
                    width: parent.width * 0.1
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 5
                        anchors.right: parent.right
                        text: model.runtime
                    }
                }
            }
        }
    }

    PlaybackList {
        id: playbackList
    }
}
