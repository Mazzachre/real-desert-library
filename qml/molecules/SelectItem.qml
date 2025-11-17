import QtQuick
import QtQuick.Controls

Item {
    id: root
    property bool editable: false
    property var items: []
    property string displayRole: "name"
    property string valueRole: "id"
    property alias text: input.text
    property int currentIndex: -1
    signal accepted()

    // property bool dropdownVisible: false

    TextField {
        id: input
        anchors.fill: parent
        placeholderText: "Select or type..."
        readOnly: !root.editable

        onPressed: {
        //     console.log(input.activeFocus, root.dropdownVisible)
        //     //TODO This is what I need to make work!
        //     //Currently dropdown.opened is always false!! Why?
        //     //I would want to toggle focus between dropdown and TextField
        //     if (root.dropdownVisible) {
                // dropdown.close()
        //         input.forceActiveFocus()
        //     } else {
                dropdown.open()
        //     }
        }

        onTextEdited: {
            dropdown.close()
        }
    }

    Popup {
        id: dropdown
        width: root.width
        y: root.height
        modal: false
        focus: true
        height: Math.min(200, listView.contentHeight)

        background: Rectangle {
            color: "white"
            border.color: "gray"
            radius: 4
        }

        // MouseArea {
        //     anchors.fill: parent
        //     hoverEnabled: true
        //     onEntered: {
        //         console.log("Over popup")
        //         listView.forceActiveFocus();
        //     }
        // }

        // onOpened: {
        //     root.dropdownVisible = true
        //     listView.forceActiveFocus()
        //     console.log("opened")
        // }
        //
        // onClosed: {
        //     root.dropdownVisible = false
        //     console.log("closed")
        // }

        ScrollView {
            height: Math.min(200, listView.contentHeight)
            width: parent.width

            ListView {
                id: listView
                model: root.items
                anchors.fill: parent
                interactive: true
                clip: true

                delegate: Rectangle {
                    width: listView.width
                    height: 24
                    color: overlay.containsMouse ? "whitesmoke" : "transparent"

                    MouseArea {
                        id: overlay
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            input.text = modelData[root.displayRole]
                            root.currentIndex = index
                            dropdown.close()
                            root.accepted()
                        }

                    }

                    Text {
                        text: modelData[root.displayRole]
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        anchors.leftMargin: 8
                    }
                }
            }
        }
    }
}
