import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0

Dialog {
    id: dialog
    title: "Search for TV Show"
    modal: true
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Cancel

    Column {
        spacing: 5
        padding: 2
        anchors.fill: parent

        Row {
            spacing: 5
            width: parent.width

            TextField {
                id: searchField
                width: parent.width - searchButton.width - (ShowsUI.searchResults.total !== 0 ? 110 : 25)
                placeholderText: "Enter TV show name"
                onAccepted: ShowsUI.searchResults.search(searchField.text)
            }

            Button {
                visible: ShowsUI.searchResults.total !== 0
                enabled: ShowsUI.searchResults.page > 1
                height: 24
                width: 20
                text: "<"
                onClicked: ShowsUI.searchResults.search(searchField.text, ShowsUI.searchResults.page-1)
            }

            Text {
                visible: ShowsUI.searchResults.total !== 0
                height: 24
                width: 31
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: ShowsUI.searchResults.page + " of " + ShowsUI.searchResults.total
            }

            Button {
                visible: ShowsUI.searchResults.total !== 0
                enabled: ShowsUI.searchResults.page < ShowsUI.searchResults.total
                height: 24
                width: 20
                text: ">"
                onClicked: ShowsUI.searchResults.search(searchField.text, ShowsUI.searchResults.page+1)
            }

            Button {
                id: searchButton
                text: "Search"
                onClicked: ShowsUI.searchResults.search(searchField.text)
            }
        }

        Row {
            spacing: 2
            width: parent.width - 25

            Rectangle {
                width: parent.width * 0.05
                height: 24
                color: "lightgray"
            }

            Rectangle {
                width: parent.width * 0.65
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

            Rectangle {
                width: parent.width * 0.15
                height: 24
                color: "lightgray"

                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Language"
                }
            }

            Rectangle {
                width: parent.width * 0.15
                height: 24
                color: "lightgray"

                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Release Year"
                }
            }
        }

        ListView {
            width: parent.width - 10
            height: parent.height - 70

            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
                active: ScrollBar.AlwaysOn
            }
            clip: true
            model: ShowsUI.searchResults
            spacing: 5

            delegate: Rectangle {
                height: 24
                width: dialog.width - 24
                radius: 3
                color: (resultMouseHandler.containsMouse || infoIconHandler.containsMouse) ? "#FFD580" : "#FFFFFF"

                MouseArea {
                    id: resultMouseHandler
                    anchors.fill: parent
                    enabled: true
                    hoverEnabled: true
                    onClicked: {
                        dialog.close();
                        searchField.text = "";
                        ShowsUI.addShow(id);
                    }
                }

                Row {
                    spacing: 5
                    anchors.fill: parent

                    Item {
                        width: parent.width * 0.05
                        height: parent.height

                        MouseArea {
                            id: infoIconHandler
                            anchors.fill: parent
                            enabled: true
                            hoverEnabled: true
                            onEntered: {
                                descriptionPopover.name = name
                                descriptionPopover.year = releaseYear
                                descriptionPopover.description = overview
                                descriptionPopover.open()
                            }
                            onExited: descriptionPopover.close()
                        }

                        Text {
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "ⓘ"
                        }
                    }

                    Item {
                        width: parent.width * 0.65
                        height: parent.height

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            text: ShowsUI.searchResults.useOriginalName ? originalName : name
                        }
                    }

                    Item {
                        width: parent.width * 0.15
                        height: parent.height

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            text: originalLanguage
                        }
                    }

                    Item {
                        width: parent.width * 0.15
                        height: parent.height

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            text: releaseYear
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: descriptionPopover
        x: 70
        y: 70
        width: parent.width - 80
        height: parent.height - 80

        property var name: ""
        property var poster: ""
        property var year: ""
        property var description: ""

        contentItem: Column {
            padding: 5
            spacing: 5

            Row {
                spacing: 5
                height: 24

                Text {
                    text: descriptionPopover.name
                }

                Text {
                    text: descriptionPopover.year
                }
            }

            Rectangle {
                width: parent.width - 10
                height: parent.height - 40
                radius: 5
                border.color: "darkgrey"
                border.width: 1

                Text {
                    padding: 10
                    width: parent.width
                    text: descriptionPopover.description
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}
