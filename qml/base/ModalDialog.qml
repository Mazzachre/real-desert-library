import QtQuick
import QtQuick.Controls
import com.realdesert 1.0

Dialog {
    modal: false
    onOpened: Blocker.opened()
    onClosed: Blocker.closed()
    // onVisibilityChanged: {
    //     if (visible) {
    //         Blocker.opened()
    //     } else {
    //         Blocker.closed()
    //     }
    // }
}
