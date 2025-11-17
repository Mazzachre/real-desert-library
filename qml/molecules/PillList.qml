import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    id: root
    property var items: []
    property var all: []
    property string name: ""
    signal removeItem(int id)
    signal addItem(var item)

    width: parent ? parent.width : 300
    height: flow.implicitHeight + 6

    Rectangle {
        anchors.fill: parent
        radius: 3
        border.color: "lightsteelblue"
        border.width: 0.5

        Flow {
            id: flow
            width: root.width
            spacing: 8
            anchors.fill: parent
            anchors.margins: 3

            Repeater {
                model: root.items

                delegate: Rectangle {
                    id: pill
                    color: "#ddd"
                    radius: 10
                    height: 18
                    width: textItem.width + deleteButton.width + 6

                    Row {
                        Text {
                            id: textItem
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                            text: modelData.name
                            padding: 6
                        }

                        ToolButton {
                            id: deleteButton
                            width: 18
                            height: 18
                            onClicked: root.removeItem(modelData.id)
                            icon.source: "qrc:/com/realdesert/ui/images/cross.svg"
                        }
                    }
                }
            }

            RoundButton {
                width: 18
                height: 18
                onClicked: selector.open()
                icon.source: "qrc:/com/realdesert/ui/images/plus.svg"
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: "Add new " + root.name
            }
        }
    }

    Popup {
        id: selector
        width: root.width
        property int itemWidth: ((width/4) - 10)

        background: Rectangle {
            color: "white"
            border.color: "gray"
            radius: 4
        }

        ScrollView {
            anchors.fill: parent

            Flow {
                spacing: 8
                width: selector.width

                TextField {
                    placeholderText: "Add new..."
                    width: selector.itemWidth
                    height: 24

                    Keys.onReturnPressed: {
                        selector.close()
                        root.addItem({name: text})
                        text = ""
                    }
                }

                Repeater {
                    model: root.all

                    delegate: Rectangle {
                        width: selector.itemWidth
                        height: 24
                        color: selectorArea.containsMouse ? "whitesmoke" : "transparent"

                        MouseArea {
                            id: selectorArea
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: {
                                selector.close()
                                root.addItem({id: modelData.id, name: modelData.name})
                            }
                        }

                        Text {
                            text: modelData.name
                            anchors.fill: parent
                            anchors.leftMargin: 5
                            verticalAlignment: Text.AlignVCenter
                            clip: true
                            elide: Text.ElideRight
                            ToolTip.visible: selectorArea.containsMouse
                            ToolTip.text: modelData.name
                        }
                    }
                }
            }
        }
    }
}
