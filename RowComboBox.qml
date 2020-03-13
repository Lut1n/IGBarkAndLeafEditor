import QtQuick 2.0
import QtQuick.Controls 2.5

Item {

    property alias currentIndex: combobox.currentIndex
    property alias model: combobox.model
    property alias text: label.text
    property alias color: label.color

    id: item
    height: childrenRect.height

    Row {
        width: parent.width

        Label {
            id: label
            text: qsTr("no name")
            color: "white"
            width: item.width * 0.5
        }
        ComboBox {
            id: combobox
            width: item.width * 0.5
        }
    }
}
