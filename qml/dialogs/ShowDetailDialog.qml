import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.realdesert 1.0
import com.realdesert.ui 1.0

ModalDialog {
    id: dialog
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Cancel

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: "Show: " + ShowUI.show.name
                font.bold: true
                Layout.alignment: Qt.AlignVCenter
            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                text: "Extras"
                onClicked: ExtrasUI.load(ExtrasType.Show, ShowUI.show.id);
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

            Row {
                spacing: 2
                width: dialog.width

                Text {
                    text: "Genres"
                    font.bold: true
                    width: 120
                }

                PillList {
                    items: ShowUI.show.genres
                    all: ShowUI.genres
                    name: "Genre"
                    width: parent.width - 135
                    onRemoveItem: function (id) {
                        ShowUI.removeGenre(id)
                    }
                    onAddItem: function (item) {
                        ShowUI.addGenre(item);
                    }
                }
            }

            Row {
                spacing: 2
                width: dialog.width

                Text {
                    text: "Tags"
                    font.bold: true
                    width: 120
                }

                PillList {
                    items: ShowUI.show.tags
                    all: ShowUI.tags
                    name: "Tag"
                    width: parent.width - 135
                    onRemoveItem: function (id) {
                        ShowUI.removeTag(id)
                    }
                    onAddItem: function (item) {
                        ShowUI.addTag(item);
                    }
                }
            }

            DefinitionList {
                width: parent.width
                definitions: ShowUI.show.info
            }

            PeopleList {
                width: parent.width
                title: "Cast"
                people: ShowUI.show.cast
            }

            PeopleList {
                width: parent.width
                title: "Crew"
                people: ShowUI.show.crew
            }
        }
    }
}
