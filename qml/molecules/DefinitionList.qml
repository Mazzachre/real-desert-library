import QtQuick
import QtQuick.Controls
import com.realdesert.ui 1.0

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

            LongText {
                width: 120
                bold: true
                text: modelData.key
            }

            Text {
                text: modelData.value
                wrapMode: Text.Wrap
                width: parent.width - 135
            }
        }
    }
}
