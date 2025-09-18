import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Dialog {
    id: dialog
    title: "Match Episodes to files"
    modal: true
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
                spacing: 5
                anchors.fill: parent

                Item {
                    width: parent.width * 0.40
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        text: file
                    }
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
