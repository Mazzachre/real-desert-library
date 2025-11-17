import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Button {
        x: 0
        y: 0
        width: (parent.width * 0.3) - 5
        height: 24
        text: "Back to show list"
        onClicked: ModeHandler.mode = ModeHandler.Shows
    }

    EpisodeFilter {
        x: 0
        y: 29
        width: (parent.width * 0.3) - 5
        height: parent.height - 29
    }

    Row {
        x: parent.width * 0.3
        y: 0
        width: parent.width * 0.7
        height: 24
        spacing: 4

        Button {
            width: 48
            height: 24

            Image {
                width: 14
                height: 14
                anchors.centerIn: parent
                source: "qrc:/com/realdesert/ui/images/play-one.svg"
            }

            ToolTip.visible: hovered
            ToolTip.text: "Play one file"

            onClicked: {
                ShowUI.episodes.play()
            }
        }

        Button {
            width: 48
            height: 24

            Image {
                width: 14
                height: 14
                anchors.centerIn: parent
                source: "qrc:/com/realdesert/ui/images/play-all.svg"
            }

            ToolTip.visible: hovered
            ToolTip.text: "Play all files"

            onClicked: {
                ShowUI.episodes.play(true)
            }
        }

        Button {
            width: 24
            height: 24

            Image {
                width: 14
                height: 14
                anchors.centerIn: parent
                source: "qrc:/com/realdesert/ui/images/details.svg"
            }

            ToolTip.visible: hovered
            ToolTip.text: "Show Details"

            onClicked: {
                showDetailDialog.open()
            }
        }

        //TODO Set as text... If name != originalName set a ? or similar and add a tooltip...
        Text {
            text: ShowUI.show.name
        }
    }

    EpisodeList {
        x: parent.width * 0.3
        y: 29
        width: parent.width * 0.7
        height: parent.height - 29
    }

    EpisodeMatcherDialog {
        id: episodeMatcherDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10

        Component.onCompleted: {
            Qt.callLater(() => {
                ShowUI.matcher.requestOpenDialog.connect(() => {
                    episodeMatcherDialog.open();
                });
            });
        }

        onRejected: {
            ShowUI.matcher.clear();
        }
    }

    ShowDetailDialog {
        id: showDetailDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10
    }
}
