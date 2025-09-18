import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    id: root
    property var model
    signal clicked(int id)
    height: listView.height

    ListView {
        id: listView
        width: parent.width
        height: contentHeight
        model: root.model

        delegate: Item {
            width: root.width
            height: 24

            MouseArea {
                anchors.fill: parent
                onClicked: root.clicked(model.id)
            }

            Image {
                x: 4
                y: 4
                width: 14
                height: 14
                source: model.selected ? "qrc:/com/realdesert/ui/images/select-true.svg" : "qrc:/com/realdesert/ui/images/select-false.svg"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 24
                text: model.name
            }
        }
    }
}
