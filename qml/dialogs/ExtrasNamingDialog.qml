import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

ModalDialog {
    id: dialog
    title: "Naming extras"
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Save | Dialog.Cancel

    onRejected: ExtrasUI.add.clear()
    onAccepted: ExtrasUI.add.save()

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
        model: ExtrasUI.add
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
                    onClicked: ExtrasUI.add.removeFile(id)
                }

               LongText {
                    width: parent.width * 0.35
                    height: parent.height
                    text: model.file
                }

               Item {
                    width: parent.width * 0.10
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        text: model.runtime
                    }
                }

                TextField {
                    width: parent.width * 0.45
                    text: model.name
                    onTextChanged: {
                        if (text !== model.name) {
                            ExtrasUI.add.updateName(model.id, text)
                        }
                    }
                }
            }
        }
    }
}
