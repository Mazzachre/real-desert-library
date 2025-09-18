import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Row {
        spacing: 2
        width: parent.width - 25
        height: 24

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            Text {
                anchors.centerIn: parent
                text: "SE"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            Text {
                anchors.centerIn: parent
                text: "EP"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"
        }

        Rectangle {
            width: (parent.width * 0.6) - 14
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

            Image {
                anchors.centerIn: parent
                width: 14
                height: 14
                source: "qrc:/com/realdesert/ui/images/favorite.svg"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"
        }

        Rectangle {
            width: parent.width * 0.1
            height: 24
            color: "lightgray"

            Image {
                anchors.centerIn: parent
                width: 14
                height: 14
                source: "qrc:/com/realdesert/ui/images/time.svg"
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
        model: ShowUI.episodes
        spacing: 5

        delegate: Rectangle {
            height: 24
            width: parent ? parent.width - 24 : 0
            radius: 3
            color: ShowUI.episodes.selected == id ? "#FFD580" : "#FFFFFF"

            Row {
                spacing: 2
                anchors.fill: parent

                Item {
                    width: parent.width * 0.05
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 2
                        anchors.right: parent.right
                        color: playable ? "black" : "grey"
                        text: season
                    }
                }

                Item {
                    width: parent.width * 0.05
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 2
                        anchors.right: parent.right
                        color: playable ? "black" : "grey"
                        text: episode
                    }
                }

                Rectangle {
                    width: parent.width * 0.05
                    height: parent.height
                    border.color: detailsArea.containsMouse ? "#80AAFF" : "#FFFFFF"

                    MouseArea {
                        id: detailsArea
                        anchors.fill: parent
                        enabled: true
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            ShowUI.episodes.openDetails(model.id)
                        }
                    }

                    Image {
                        anchors.centerIn: parent
                        width: 14
                        height: 14
                        source: "qrc:/com/realdesert/ui/images/details.svg"
                    }
                }

                Rectangle {
                    width: (parent.width * 0.6) - 14
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
                            ShowUI.episodes.selected = id
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        color: playable ? "black" : "grey"
                        text: name
                    }
                }

                CheckField {
                    width: parent.width * 0.05
                    height: parent.height
                    state: model.favorite

                    onToggle: {
                        ShowUI.episodes.toggleFavorite(model.id)
                    }
                }

                Item {
                    width: parent.width * 0.05
                    height: parent.height
                }

                Item {
                    width: parent.width * 0.05
                    height: parent.height

                    Image {
                        visible: model.subtitles
                        anchors.centerIn: parent
                        width: 14
                        height: 14
                        source: "qrc:/com/realdesert/ui/images/subtitles.svg"
                    }
                }

                Item {
                    width: parent.width * 0.1
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 2
                        anchors.right: parent.right
                        color: playable ? "black" : "grey"
                        text: runtime
                    }
                }
            }
        }
    }

    EpisodeDetailDialog {
        id: episodeDetailDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10
    }

    Component.onCompleted: {
        ShowUI.details.open.connect(() => {
            episodeDetailDialog?.open();
        });
    }
}
