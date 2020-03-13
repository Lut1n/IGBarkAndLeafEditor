import QtQuick 2.0

Rectangle
{
    id: root
    property string text: "text"

    signal clicked()

    implicitWidth: 100
    implicitHeight: 30

    border.color: text? '#FF000032': 'transparent'
    border.width: enabled && mouseArea.pressed? 3 : 1
    radius: 6
    opacity: enabled  &&  !mouseArea.pressed? 1: 0.5

    color: "#FF303132"

    Text
    {
        text: root.text
        color: "white"
        font.pixelSize: 0.5 * root.height
        anchors.centerIn: parent
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked:  root.clicked()
    }
}
