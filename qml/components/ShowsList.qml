import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Row {
        spacing: 2
        width: parent.width - 23
        height: 24

        Rectangle {
            width: (parent.width * 0.6) - 38
            height: 24
            color: "lightgray"

            ComboBox {
                id: nameSelector
                anchors.fill: parent
                model: ["Name", "Original Name"]
                onCurrentIndexChanged: {
                    ShowsUI.searchResults.useOriginalName = (currentIndex === 1)
                }
            }
        }

        SortOrderCombo {
            ascValue: SortOrder.TitleAsc
            descValue: SortOrder.TitleDesc
            sortType: "alpha"
            sortOrder: ShowsUI.order
            onSortOrderSelected: function(value) {
                ShowsUI.order = value
            }
        }

        Rectangle {
            width: (parent.width * 0.2) - 5
            height: 24
            color: "lightgray"

            Text {
                anchors.centerIn: parent
                text: "Release Year"
            }
        }

        SortOrderCombo {
            ascValue: SortOrder.ReleaseAsc
            descValue: SortOrder.ReleaseDesc
            sortType: "numeric"
            sortOrder: ShowsUI.order
            onSortOrderSelected: function(value) {
                ShowsUI.order = value
            }
        }

        Rectangle {
            width: (parent.width * 0.2) - 5
            height: 24
            color: "lightgray"

            Text {
                anchors.centerIn: parent
                text: "Episodes"
            }
        }

    }

    ListView {
        y: 29
        height: parent.height - 29
        width: parent.width

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: ScrollBar.AlwaysOn
        }
        clip: true
        model: ShowsUI.list
        spacing: 5

        delegate: Rectangle {
            height: 24
            width: parent ? parent.width - 15 : 0
            radius: 3
            color: (showMouseHandler.containsMouse) ? "#FFD580" : "#FFFFFF"

            MouseArea {
                id: showMouseHandler
                anchors.fill: parent
                enabled: true
                hoverEnabled: true
                onClicked: {
                    ShowUI.load(id);
                }
            }

            Row {
                spacing: 5
                anchors.fill: parent

                Item {
                    width: parent.width * 0.65
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        text: ShowsUI.searchResults.useOriginalName ? originalName : name
                    }
                }

                Item {
                    width: parent.width * 0.15
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 15
                        anchors.right: parent.right
                        text: releaseYear
                    }
                }

                Item {
                    width: parent.width * 0.20
                    height: parent.height

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 20
                        anchors.right: parent.right
                        text: episodeCount + " (" + officialEpisodeCount + ")"
                    }
                }
            }
        }
    }
}
