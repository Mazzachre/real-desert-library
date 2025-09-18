import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Column {
        anchors.fill: parent
        spacing: 2

        Foldable {
            width: parent.width
            title: "Filter Years"
            content: Component {
                Item {
                    width: parent.width
                    height: 56

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.top: parent.top
                        anchors.topMargin: 4
                        text: "Newer Than"
                    }

                    TextField {
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        anchors.top: parent.top
                        anchors.topMargin: 2
                        width: 100
                        height: 24
                        inputMethodHints: Qt.ImhDigitsOnly
                        text: ShowsUI.filter.newerThan > 0 ? ShowsUI.filter.newerThan.toString() : ""
                        onTextChanged: {
                            ShowsUI.filter.newerThan = parseInt(text) || 0
                        }
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.top: parent.top
                        anchors.topMargin: 34
                        height: 24
                        horizontalAlignment: Text.AlignHCenter
                        text: "Older Than"
                    }

                    TextField {
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        anchors.top: parent.top
                        anchors.topMargin: 30
                        width: 100
                        inputMethodHints: Qt.ImhDigitsOnly
                        text: ShowsUI.filter.olderThan > 0 ? ShowsUI.filter.olderThan.toString() : ""
                        onTextChanged: {
                            ShowsUI.filter.olderThan = parseInt(text) || 0
                        }
                    }
                }
            }
        }

        Foldable {
            width: parent.width
            title: "Filter Genres"
            content: Component {
                SelectList {
                    width: parent.width
                    model: ShowsUI.filter.genres
                    onClicked: function(id) {
                        ShowsUI.filter.genres.toggle(id);
                    }
                }
            }
        }

        Foldable {
            width: parent.width
            title: "Filter Tags"
            content: Component {
                SelectList {
                    width: parent.width
                    model: ShowsUI.filter.tags
                    onClicked: function(id) {
                        ShowsUI.filter.tags.toggle(id);
                    }
                }
            }
        }
    }
}
