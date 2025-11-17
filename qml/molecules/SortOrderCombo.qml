import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

ComboBox {
    id: root
    width: 24
    height: 24

    property var ascValue
    property var descValue
    property var sortOrder: null
    property string sortType: "alpha"

    property var symbols: ["↥", "-", "↧"]

    property var imageMap: ({
        "alpha": ["qrc:/com/realdesert/ui/images/sort-alpha-asc.svg", "qrc:/com/realdesert/ui/images/sort-alpha-desc.svg"],
        "numeric": ["qrc:/com/realdesert/ui/images/sort-numeric-asc.svg", "qrc:/com/realdesert/ui/images/sort-numeric-desc.svg"]
    })

    function indexFromEnum(value) {
        if (value === ascValue) return 0
        if (value === descValue) return 1
        return -1
    }

    function enumFromIndex(index) {
        if (index === 0) return ascValue
        if (index === 1) return descValue
        return sortOrder
    }

    model: symbols
    currentIndex: indexFromEnum(sortOrder)

    signal sortOrderSelected(var value)

    indicator: null
    contentItem: Image {
        anchors.centerIn: parent
        source: root.currentIndex >= 0 ? root.imageMap[root.sortType][indexFromEnum(sortOrder)] : "qrc:/com/realdesert/ui/images/sort-default.svg"
        width: 20
        height: 20
    }

    popup: Popup {
        y: root.height
        width: root.width
        modal: true

        Column {
            spacing: 2
            Repeater {
                model: [0, 1]
                delegate: ItemDelegate {
                    required property int modelData
                    width: 28
                    height: 28
                    padding: 0
                    contentItem: Image {
                        anchors.centerIn: parent
                        source: root.imageMap[root.sortType][modelData]
                        width: 24
                        height: 24
                    }
                    onClicked: {
                        root.popup.close()
                        root.sortOrderSelected(root.enumFromIndex(modelData))
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.popup.open()
    }
}
