import QtQuick 2.0
import QtQuick.Controls 2.5

Item {

    width: parent.width * 0.9
    height: parent.height
    x: parent.width * 0.05

    Rectangle{
        anchors.fill: parent
        color: "#EE2D2F30"
    }

    Column
    {
        anchors.fill: parent

        Text {
            id: histlabel
            text: qsTr("Log history :")
            color: "white"
        }

        ScrollView
        {
            id: logHistoryScroll
            width: parent.width
            height: parent.height * 0.25

            clip: true

            Column
            {
                width: logHistoryScroll.width

                Text {
                    width: parent.width
                    id: logHistory
                    color: "green"
                    text: loadinginfo.history
                }
            }
        }

        MenuSeparator{
            id: separator
            width: parent.width
        }

        Text {
            id: ctrllabel
            text: qsTr("Control panel :")
            color: "white"
        }

        ScrollView{
            id: paramsScroll
            width: parent.width
            height: parent.height - ctrllabel.height - histlabel.height - logHistoryScroll.height - separator.height

            clip: true

            Column{
                width: paramsScroll.width

                Label{ text: qsTr("Lighting :"); color: "white" }
                RowSlider {
                    width: parent.width
                    text: qsTr("ambient"); color: "white"
                    from: 0.0; to: 1.0
                    value: 0.3
                    onValueChanged: igconfig.ambient = value
                }
                RowSlider {
                    width: parent.width
                    text: qsTr("power"); color: "white"
                    from: 0.0; to: 512.0
                    value: 200.0
                    onValueChanged: igconfig.power = value
                }

                MenuSeparator{
                    width: parent.width
                }

                Label{ text: qsTr("Shadows :"); color: "white" }
                RowComboBox {
                    width: parent.width
                    text: qsTr("Resolution"); color: "lightgrey"
                    currentIndex: 0
                    model: ListModel {
                        // id: shadowsRes
                        ListElement { text: "Default"     }
                        ListElement { text: "16"    }
                        ListElement { text: "32"    }
                        ListElement { text: "64"    }
                        ListElement { text: "128"   }
                        ListElement { text: "256"   }
                        ListElement { text: "512"   }
                    }
                    onCurrentIndexChanged: igconfig.shadowRes=currentIndex
                }

                RowComboBox{
                    width: parent.width
                    text: qsTr("Sampling"); color: "lightgrey"
                    currentIndex: 0
                    model: ListModel {
                        // id: shadowSamples
                        ListElement { text: "Default"     }
                        ListElement { text: "x4"    }
                        ListElement { text: "x8"    }
                        ListElement { text: "x16"   }
                    }
                    onCurrentIndexChanged: igconfig.shadowSamples=currentIndex

                }

                MenuSeparator{
                    width: parent.width
                }

                Label{ text: qsTr("Environment :"); color: "white" }
                RowComboBox {
                    width: parent.width
                    text: qsTr("Resolution"); color: "lightgrey"
                    currentIndex: 0
                    model: ListModel {
                        // id: environmentRes
                        ListElement { text: "Default"     }
                        ListElement { text: "16"    }
                        ListElement { text: "32"    }
                        ListElement { text: "64"    }
                        ListElement { text: "128"   }
                        ListElement { text: "256"   }
                        ListElement { text: "512"   }
                    }
                    onCurrentIndexChanged: igconfig.environmentRes=currentIndex
                }

                MenuSeparator{
                    width: parent.width
                }

                Label{ text: qsTr("Ambient Occlusion :"); color: "white" }
                RowComboBox {
                    width: parent.width
                    text: qsTr("Sampling"); color: "lightgrey"
                    currentIndex: 0
                    model: ListModel {
                        // id: ssaoSamples
                        ListElement { text: "Default"     }
                        ListElement { text: "x4"    }
                        ListElement { text: "x8"    }
                        ListElement { text: "x16"   }
                    }
                    onCurrentIndexChanged: igconfig.ssaoSamples=currentIndex
                }

                MenuSeparator{
                    width: parent.width
                }

                Label{ text: qsTr("Other options :"); color: "white" }

                RowCheckBox{

                    width: parent.width
                    checked: true
                    text: qsTr("Transparency")
                    color: "lightgrey"
                    onCheckedChanged: igconfig.transparency=checked
                }

                RowCheckBox{

                    width: parent.width
                    checked: true
                    text: qsTr("Animation")
                    color: "lightgrey"
                    onCheckedChanged: igconfig.animated=checked
                }



                MenuSeparator{
                    width: parent.width
                }

                Label{ text: qsTr("About :"); color: "white" }
                Label{ text: qsTr("Author : Mathieu Boulet"); color: "lightgrey"; font.pixelSize: 12 }
                Label{ text: qsTr("App github : no"); color: "lightgrey"; font.pixelSize: 12 }
                Label{ text: qsTr("ImpGears github : https://github.com/Lut1n/ImpGears"); color: "lightgrey"; font.pixelSize: 12 }
            }
        }
    }
}
