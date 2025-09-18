import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Button {
        x: 0
        y: 0
        width: (parent.width * 0.3) - 5
        height: 24
        text: "Add Series"
        onClicked: showSearchDialog.open()
    }

    ShowsFilter {
        x: 0
        y: 29
        width: (parent.width * 0.3) - 5
        height: parent.height - 29
    }

    ShowsList {
        x : parent.width * 0.3
        y: 0
        width: parent.width * 0.7
        height: parent.height
    }

    ShowSearchDialog {
        id: showSearchDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10
        modal: true
    }
}
