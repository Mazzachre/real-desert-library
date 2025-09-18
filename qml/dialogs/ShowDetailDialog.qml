import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0
import com.realdesert.ui 1.0

Dialog {
    id: dialog
    title: "Show data"
    modal: true
    closePolicy: Popup.CloseOnEscape
    standardButtons: Dialog.Cancel

    property var data: {}


}
