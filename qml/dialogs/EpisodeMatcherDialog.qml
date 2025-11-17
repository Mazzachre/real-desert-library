import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

ModalDialog {
    id: dialog
    title: "Match Episodes: " + ShowUI.show.name
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Save | Dialog.Cancel

    onRejected: ShowUI.matcher.clear()
    onAccepted: ShowUI.matcher.save()

    ListView {
        height: parent.height
        width: parent.width

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: ScrollBar.AlwaysOn
        }
        clip: true
        model: ShowUI.matcher
        spacing: 5

        delegate: Rectangle {
            height: 24
            width: parent ? parent.width - 24 : 0

            Row {
                spacing: 2
                anchors.fill: parent

                Button {
                    width: parent.width * 0.05
                    height: parent.height
                    text: "-"
                    onClicked: ShowUI.matcher.removeFile(id)
                }

                LongText {
                    width: parent.width * 0.35
                    height: parent.height
                    text: file
                }

                Item {
                    width: parent.width * 0.10
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        text: runtime
                    }
                }

                SelectOne {
                    width: parent.width * 0.45
                    height: parent.height

                    model: ShowUI.matcher.episodes
                    selected: match

                    onClicked: function(item) {
                        ShowUI.matcher.selected(id, item);
                    }
                }
            }
        }
    }
}
