import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        Column {
            id: filterColumn
            spacing: 5
            width: parent.width * 0.3

            Button {
                width: parent.width
                height: 24

                text: "Add Movies"
                onClicked: movieSearchDialog.open()
            }
        }

        //List of shows
    }

    MovieSearchDialog {
        id: movieSearchDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10
        modal: true
    }
}

