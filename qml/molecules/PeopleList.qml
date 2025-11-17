import QtQuick
import QtQuick.Controls
import com.realdesert.ui 1.0

Column {
    visible: people.length > 0
    id: root
    spacing: 2

    property string title: ""
    property var people: []

    Text {
        visible: !!title
        text: title
        font.bold: true
        font.underline: true
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }

    Repeater {
        model: root.people

        delegate: Row {
            spacing: 30
            width: root.width

            Text {
                width: (parent.width / 2) - 15
                font.bold: true
                text: modelData.name
                horizontalAlignment: Text.AlignRight
            }

            Text {
                width: (parent.width / 2) - 15
                text: modelData.value
            }
        }
    }
}
