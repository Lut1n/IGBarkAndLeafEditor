import QtQuick 2.0
import QtQuick.Controls 2.5

Item {

    property alias from: slider.from
    property alias to: slider.to
    property alias value: slider.value
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
        Slider {
            id: slider
            width: item.width * 0.6
        }
    }
}
