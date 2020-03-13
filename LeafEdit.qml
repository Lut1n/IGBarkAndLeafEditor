import QtQuick 2.0
import QtQuick.Controls 2.5
import ImpGears 1.0

Item
{
    width: parent.width * 0.9
    height: parent.height * 0.4
    x: parent.width * 0.05
    y: parent.height * 0.6

    Rectangle{
        anchors.fill: parent
        color: "#AA000000"
    }

    ScrollView{
        id: leafeditScroll
        anchors.fill: parent

        clip: true

        Column{
            width: leafeditScroll.width

            RowSlider {
                width: parent.width
                text: qsTr("branch depth"); color: "white"
                from: 0; to: 10
                value: leafmodel.dft_branchdepth
                onValueChanged: leafmodel.branchdepth = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch count"); color: "white"
                from: 0; to: 10
                value: leafmodel.dft_branchcount
                onValueChanged: leafmodel.branchcount = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch tilt"); color: "white"
                from: 0.0; to: 3.0
                value: leafmodel.dft_branchtilt
                onValueChanged: leafmodel.branchtilt = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch length"); color: "white"
                from: 0.0; to: 1.0
                value: leafmodel.dft_branchlength
                onValueChanged: leafmodel.branchlength = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch length fct"); color: "white"
                from: 0.0; to: 2.0
                value: leafmodel.dft_branchlengthfct
                onValueChanged: leafmodel.branchlengthfct = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch thickness"); color: "white"
                from: 0.0; to: 1.0
                value: leafmodel.dft_branchthickness
                onValueChanged: leafmodel.branchthickness = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("texture octaves"); color: "white"
                from: 0; to: 4
                value: leafmodel.dft_texoctave
                onValueChanged: leafmodel.texoctave = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("texture frequency"); color: "white"
                from: 0.0; to: 32.0
                value: leafmodel.dft_texfreq
                onValueChanged: leafmodel.texfreq = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("shape freq start"); color: "white"
                from: 0.0; to: 3.0
                value: leafmodel.dft_shapefq1
                onValueChanged: leafmodel.shapefq1 = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("shape freq end"); color: "white"
                from: 0.0; to: 3.0
                value: leafmodel.dft_shapefq2
                onValueChanged: leafmodel.shapefq2 = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("shape min"); color: "white"
                from: 0.0; to: 3.0
                value: leafmodel.dft_shapemin
                onValueChanged: leafmodel.shapemin = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("shape max"); color: "white"
                from: 0.0; to: 3.0
                value: leafmodel.dft_shapemax
                onValueChanged: leafmodel.shapemax = value
            }
        }
    }
}
