import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Row {
        spacing: 5

        Button {
            height: 24
            text: "Back"
            onClicked: {
                ExtrasUI.back();
            }
        }

        //play button
    }

    ExtrasList {
        x: 5
        y: 29
        width: parent.width - 10
        height: parent.height - 29
    }

    ExtrasNamingDialog {
        id: extrasNamingDialog
        x: 5
        y: 5
        width: parent.width - 10
        height: parent.height - 10

        Component.onCompleted: {
            Qt.callLater(() => {
                ExtrasUI.add.requestOpenDialog.connect(() => {
                    extrasNamingDialog.open();
                });
            });
        }

        onRejected: {
            ExtrasUI.add.clear();
        }
    }
}
