import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.realdesert 1.0
import com.realdesert.ui 1.0

ModalDialog {
    id: dialog
    standardButtons: Dialog.Close

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: "Episode: " + ShowUI.episode.name
                font.bold: true
                Layout.alignment: Qt.AlignVCenter
            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                text: "Extras"
                onClicked: ExtrasUI.load(ExtrasType.Episode, ShowUI.episode.id);
            }

            ToolSeparator {
                orientation: Qt.Vertical
            }

            Label {
                text: "Favorite"
            }

            CheckField {
                width: 24
                height: 24
                state: ShowUI.episode.favorite
                onToggle: {
                    ShowUI.setFavorite(ShowUI.episode.id, !ShowUI.episode.favorite)
                }
            }
        }
    }

    contentItem: ScrollView {
        clip: true
        width: dialog.width
        height: dialog.height

        Column {
            width: dialog.width - 15
            spacing: 10

            DefinitionList {
                width: parent.width
                definitions: ShowUI.episode.episode
            }

            PeopleList {
                width: parent.width
                title: "Guest Stars"
                people: ShowUI.episode.cast
            }

            PeopleList {
                width: parent.width
                title: "Crew"
                people: ShowUI.episode.crew
            }

            DefinitionList {
                width: parent.width
                title: "File"
                definitions: ShowUI.episode.file
            }

            DefinitionList {
                width: parent.width
                title: "Video"
                definitions: ShowUI.episode.video
            }

            DefinitionList {
                width: parent.width
                title: "Audio"
                definitions: ShowUI.episode.audio
            }

            DefinitionList {
                width: parent.width
                title: "Subtitle"
                definitions: ShowUI.episode.subtitle
            }
        }
    }
}
