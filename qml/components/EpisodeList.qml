import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    id: root

    Row {
        spacing: 2
        width: parent.width - 15
        height: 24

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            HeaderText {
                display: "SE"
                name: "Season"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            HeaderText {
                display: "EP"
                name: "Episode"
            }
        }

        Rectangle {
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            HeaderIcon {
                source: "qrc:/com/realdesert/ui/images/details.svg"
                name: "Details"
            }
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

            HeaderIcon {
                source: "qrc:/com/realdesert/ui/images/favorite.svg"
                name: "Favourite"
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
            width: parent.width * 0.05
            height: 24
            color: "lightgray"

            HeaderIcon {
                source: "qrc:/com/realdesert/ui/images/subtitles.svg"
                name: "Subtitles"
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
        model: ShowUI.episodes
        spacing: 5

        delegate: Rectangle {
            height: 24
            width: parent ? parent.width - 15 : 0
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
                        cursorShape: Qt.WhatsThisCursor
                        onClicked: {
                            ShowUI.episodes.openDetails(model.id)
                        }
                    }

                    ToolTip.visible: detailsArea.containsMouse
                    ToolTip.text: "Details"

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
                        ShowUI.setFavorite(model.id, !model.favorite)
                    }
                }

                PlaybackIcon {
                    id: playbackIcon
                    width: parent.width * 0.05
                    height: parent.height
                    played: model.played
                    playedFully: model.playedFully

                    onClicked: {
                        const point = playbackIcon.mapToItem(root, 0, playbackIcon.height);
                        // playbackMenu.open(point, model.file, model.fileRuntime, [{time: "Wed May 20 03:40:13 1998", played: "0:22 of 0:45"}])
                        playbackMenu.open(point, model.file, model.fileRuntime, model.playbackList)
                    }
                }

                Rectangle {
                    width: parent.width * 0.05
                    height: parent.height
                    color: "transparent"
                    border.color: subtitleArea.containsMouse ? "#80AAFF" : "transparent"

                    MouseArea {
                        id: subtitleArea
                        anchors.fill: parent
                        enabled: playable
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            const point = subtitleArea.mapToItem(root, 0, subtitleArea.height);
                            subtitleMenu.open(point, file, path, subtitleSelected, fileName);
                        }
                    }

                    Image {
                        visible: model.subtitles
                        anchors.centerIn: parent
                        width: 14
                        height: 14
                        source: model.subtitleSelected != "" ? "qrc:/com/realdesert/ui/images/subtitles.svg" : "qrc:/com/realdesert/ui/images/subtitles-available.svg"
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
                        text: model.runtime
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

    SubtitleMenu {
        id: subtitleMenu
        onSetSubtitle: function(id, file) {
            ShowUI.setSubtitle(id, file)
        }
    }

    PlaybackMenu {
        id: playbackMenu
        onSetPlayed: function(id, played) {
            ShowUI.setPlayed(id, played)
        }
    }

    Component.onCompleted: {
        ShowUI.episode.open.connect(() => {
            episodeDetailDialog?.open();
        });
    }
}
