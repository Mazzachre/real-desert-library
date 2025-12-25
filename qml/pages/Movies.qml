import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Button {
        x: 0
        y: 0
        width: (parent.width * 0.3) - 5
        height: 24

        text: "Add Movies"
        onClicked: movieSearchDialog.open()
    }

    MoviesFilter {
        x: 0
        y: 29
        width: (parent.width * 0.3) - 5
        height: parent.height - 29
    }

    MoviesList {
        x : parent.width * 0.3
        y: 0
        width: parent.width * 0.7
        height: parent.height
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

