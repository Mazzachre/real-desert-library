import QtQuick 6.4
import QtQuick.Controls 6.4

Column {
    visible: definitions.length > 0
    id: root
    spacing: 2

    property string title: ""
    property var definitions: []

    Text {
        visible: !!title
        text: title
        font.bold: true
        font.underline: true
        width: parent.width
    }

    Repeater {
        model: root.definitions

        delegate: Row {
            spacing: 2
            width: root.width

            Text {
                text: modelData.key
                font.bold: true
                width: 120
            }

            Text {
                text: modelData.value
                wrapMode: Text.Wrap
                width: parent.width - 125
            }
        }
    }
}
