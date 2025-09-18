import QtQuick 6.4
import QtQuick.Controls 6.4
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

    Item {
        x: parent.width * 0.3
        y: 0
        width: parent.width * 0.7
        height: 24

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 5
            anchors.left: parent.left
            text: ShowUI.name
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
            ShowUI.matcher.requestOpenDialog.connect(function() {
                episodeMatcherDialog.open();
            });
        }

        onRejected: {
            ShowUI.matcher.clear();
        }
    }
}
