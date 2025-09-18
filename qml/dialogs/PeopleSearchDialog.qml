import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0

Dialog {
    id: dialog
    title: "Match Episodes to files"
    modal: true
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Cancel

    signal personSelected(var person)
    property string context

    onRejected: searchModel.clear()

    Component.onCompleted: {
        searchModel = Qt.createQmlObject(
            'import com.realdesert 1.0; PeopleSearch {}',
            dialog
        );
        // searchModel.context = root.searchContext;
    }

    property var searchModel

    Column {
        spacing: 2

        Row {
            width: parent.width
            spacing: 5

            TextField {
                id: searchField
                height: 24
                width: parent.width - searchButton.width - 5
            }

            Button {
                id: searchButton
                text: "Search"
                onClicked: console.log(searchField.text)
            }
        }

        ListView {
            width: parent.width
            model: dialog.searchModel

            delegate: Row {
                width: parent.width

                Text {
                    text: name
                }
            }
        }
    }
}
