import QtQuick
import QtQuick.Controls
import com.realdesert 1.0

Dialog {
    modal: false
    onOpened: Blocker.opened()
    onClosed: Blocker.closed()
}
