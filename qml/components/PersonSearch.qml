import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Rectangle {
    id: root
    height: 52
    signal selected(int id)

    color: "transparent"
    border.width: 0.5
    border.color: "black"
    radius: 3

    Label {
        y: 4
        x: 4
        text: "Cast & Crew"
        font.bold: true
    }

    Item {
        y: 24
        x: 4
        visible: (PersonSearch.selected === "" && PersonSearch.people.length === 0)

        TextField {
            id: input
            width: root.width - 32
            height: 24
            Keys.onReturnPressed: PersonSearch.search(input.text)
        }

        Button {
            x: root.width - 32
            width: 24
            height: 24
            text: "s"
            onClicked: PersonSearch.search(input.text)
        }
    }

    Item {
        y: 24
        x: 4
        visible: PersonSearch.selected

        LongText {
            width: root.width - 32
            height: 24
            text: PersonSearch.selected
        }

        Button {
            x: root.width - 32
            width: 24
            height: 24
            text: "c"
            onClicked: {
                PersonSearch.clear()
                root.selected(0)
            }
        }
    }

    Connections {
        target: PersonSearch

        function onPeopleUpdated() {
            if (PersonSearch.people.length > 0) {
                peopleSelector.open()
            }
        }
    }

    ModalDialog {
        id: peopleSelector
        width: 400
        height: 300
        title: "Select Person"
        closePolicy: Popup.CloseOnEscape
        standardButtons: Dialog.Cancel

        onRejected: {
            PersonSearch.clear()
            input.text = ""
        }

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
            model: PersonSearch.people
            spacing: 5

            delegate: Rectangle {
                height: 24
                width: parent ? parent.width - 24 : 0

                Item {
                    anchors.fill: parent

                    Text {
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        text: modelData.name
                    }

                    ToolTip.visible: area.containsMouse
                    ToolTip.text: modelData.originalName

                    MouseArea {
                        id: area
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            PersonSearch.selected = modelData.name
                            root.selected(modelData.id)
                            input.text = ""
                            peopleSelector.close()
                        }
                    }
                }
            }
        }
    }
}
