import QtQuick
import QtQuick.Controls
import com.realdesert 1.0
import com.realdesert.ui 1.0

Item {
    Column {
        anchors.fill: parent
        spacing: 2

        Item {
            width: parent.width
            height: 24

            CheckBox {
                checked: ShowUI.episodes.playables
                text: "Only playables"
                onClicked: ShowUI.episodes.playables = !ShowUI.episodes.playables
            }
        }

        Item {
            width: parent.width
            height: 24

            CheckBox {
                checked: ShowUI.episodes.favorites
                text: "Only Favourites"
                onClicked: ShowUI.episodes.favorites = !ShowUI.episodes.favorites
            }
        }

        Foldable {
            width: parent.width
            title: "Seasons"
            content: Component {
                SelectList {
                    width: parent.width
                    model: ShowUI.episodes.seasons
                    onClicked: function(id) {
                        ShowUI.episodes.toggleSeason(id);
                    }
                }
            }
        }
    }
}
