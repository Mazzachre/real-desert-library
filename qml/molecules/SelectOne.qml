import QtQuick
import QtQuick.Controls

Item {
    id: root

    property var model
    property var selected: undefined
    property string title: "Select One"

    signal clicked(var item)

    Button {
        id: button
        text: selected ? getName(selected) : "-- Select one --"
        anchors.fill: parent
        onClicked: dialog.open()
    }

    Dialog {
        id: dialog
        parent: Overlay.overlay
        x: button.mapToItem(Overlay.overlay, 0, 0).x
        width: button.width
        y: 0
        height: parent.height
        title: root.title
        modal: true
        standardButtons: Dialog.Cancel

        contentItem: ScrollView {
            focus: true
            clip: true

            ListView {
                id: listview
                model: root.model
                width: dialog.width
                clip: true
flickableDirection: Flickable.VerticalFlick
boundsBehavior: Flickable.StopAtBounds
ScrollBar.vertical: ScrollBar {
    policy: ScrollBar.AlwaysOn
}

                delegate: Item {
                    width: dialog.width
                    height: 20

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.clicked(modelData.id)
                            dialog.close()
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        font.bold: modelData.id == root.selected
                        text: modelData.name
                    }
                }
            }
        }
    }

    function getName(id) {
        if (!model || model.length === 0)
            return "-- Unknown --";

        for (let i = 0; i < model.length; i++) {
            if (model[i].id === id) {
                return model[i].name;
            }
        }

        return "-- Unknown --";
    }
}

