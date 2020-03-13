import QtQuick 2.0
import QtQuick.Controls 2.5


Item {

    anchors.right: parent.right
    width: childrenRect.width
    height: childrenRect.height

    Rectangle
    {
        width: childrenRect.width
        height: childrenRect.height
        color: "black"

        visible: logtext.text!=""

        Column {
            width: childrenRect.width
            height: childrenRect.height


            ProgressBar {
                indeterminate: false
                from: 0.0
                to:1.0
                value: loadinginfo.taskprogress
            }

            Text {
                id: logtext
                color: "green"
                text: loadinginfo.message
            }
        }
    }

}

