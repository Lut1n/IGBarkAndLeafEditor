import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5


Drawer {
    id: drawer

    width: window.width / 2
    height: window.height

    modal: true
    interactive: true

    property alias edittreebutton: edittreebutton
    property alias editleafbutton: editleafbutton
    property alias editbarkbutton: editbarkbutton


    Rectangle{
        anchors.fill: parent
        color: "#FF505152"
    }

    ScrollView{
        id: sceneeditScroll
        anchors.fill: parent

        clip: true

        Column {
            id: listView
            width: sceneeditScroll.width

            Text {
                text: qsTr("Output frame :")
                color: "white"
            }
            ComboBox {
                width: parent.width
                currentIndex: 0
                model: ListModel {
                    id: cbItems
                    ListElement { text: "Default"     }
                    ListElement { text: "Lighting"    }
                    ListElement { text: "Emissive"    }
                    ListElement { text: "Normals"     }
                    ListElement { text: "ShadowMap"   }
                    ListElement { text: "Environment" }
                    ListElement { text: "Bloom"       }
                    ListElement { text: "Color"       }
                    ListElement { text: "Reflectivity"}
                    ListElement { text: "Depth"       }
                    ListElement { text: "SSAO"        }
                }
                onCurrentIndexChanged: displayoptions.gBufferIndex=currentIndex
            }

            MenuSeparator{
                width: parent.width
            }

            Text {
                text: qsTr("Features :")
                color: "white"
            }

            ButtonGroup {
                id: childGroup
                exclusive: false
                checkState: parentBox.checkState
            }

            CheckBox {
                id: parentBox
                text: qsTr("All")
                checkState: childGroup.checkState
            }

            CheckBox {
                checked: false
                text: qsTr("Phong")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
                onCheckedChanged: displayoptions.phong=checked
            }

            CheckBox {
                checked: false
                text: qsTr("Environment")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
                onCheckedChanged: displayoptions.environment=checked
            }

            CheckBox {
                checked: false
                text: qsTr("Shadow")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
                onCheckedChanged: displayoptions.shadow=checked
            }

            CheckBox {
                checked: false
                text: qsTr("Bloom")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
                onCheckedChanged: displayoptions.bloom=checked
            }

            CheckBox {
                checked: false
                text: qsTr("SSAO")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
                onCheckedChanged: displayoptions.ssao=checked
            }

            MenuSeparator{
                width: parent.width
            }

            Text {
                text: qsTr("Model edition :")
                color: "white"
            }

            CustomButton{
                width: parent.width
                id: edittreebutton
                text: qsTr("Edit tree")
            }

            CustomButton{
                width: parent.width
                id: editleafbutton
                text: qsTr("Edit leaf")
            }

            CustomButton{
                width: parent.width
                id: editbarkbutton
                text: qsTr("Edit bark")
            }

        }
    }
}
