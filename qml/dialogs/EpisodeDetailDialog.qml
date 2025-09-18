import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Dialog {
    id: dialog
    title: "Episode: " + ShowUI.details.name
    modal: true
    standardButtons: Dialog.Ok

    contentItem: ScrollView {
        clip: true
        width: dialog.width
        height: dialog.height

        Column {
            width: dialog.width - 15
            spacing: 10

            Row {
                Text {
                    font.bold: true
                    width: 120
                    text: "Favorite"
                }

                CheckField {
                    width: 24
                    height: 24
                    state: ShowUI.details.favorite
                    onToggle: {
                        ShowUI.details.setFavorite()
                    }
                }
            }

            DefinitionList {
                width: parent.width
                definitions: ShowUI.details.episode
            }

            DefinitionList {
                width: parent.width
                title: "File"
                definitions: ShowUI.details.file
            }

            DefinitionList {
                width: parent.width
                title: "Video"
                definitions: ShowUI.details.video
            }

            DefinitionList {
                width: parent.width
                title: "Audio"
                definitions: ShowUI.details.audio
            }

            DefinitionList {
                width: parent.width
                title: "Subtitle"
                definitions: ShowUI.details.subtitle
            }
        }
    }
}
