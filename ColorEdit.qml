import QtQuick 2.4
import QtQuick.Controls 2.5


Rectangle {
    id: background
    color: "#ffffff"
    width: 200
    height: childrenRect.height

    Row {
        width: parent.width

        Slider {
            width: parent.width * 0.33
            value: background.color.r
            onValueChanged: background.color.r = position
        }

        Slider {
            width: parent.width * 0.33
            value: background.color.g
            onValueChanged: background.color.g = position
        }

        Slider {
            width: parent.width * 0.33
            value: background.color.b
            onValueChanged: background.color.b = position
        }
    }
}
