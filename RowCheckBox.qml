import QtQuick 2.0
import QtQuick.Controls 2.5

Item {

    property alias checked: checkbox.checked
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
            width: item.width * 0.4
        }

        CheckBox {
            id: checkbox
            checked: false
            width: item.width * 0.6
        }
    }
}
