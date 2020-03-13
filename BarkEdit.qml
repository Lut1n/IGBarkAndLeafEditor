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
        id: barkeditScroll
        anchors.fill: parent

        clip: true

        Column{
            width: barkeditScroll.width

            RowSlider {
                width: parent.width
                text: qsTr("octaves"); color: "white"
                from: 0; to: 4
                value: barkmodel.dft_octaves
                onValueChanged: barkmodel.octaves = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("freq"); color: "white"
                from: 1.0; to: 10.0
                value: barkmodel.dft_freq
                onValueChanged: barkmodel.freq = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("persist"); color: "white"
                from: 0.1; to: 1.0
                value: barkmodel.dft_persist
                onValueChanged: barkmodel.persist = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("scale x"); color: "white"
                from: 0.1; to: 10.0
                value: barkmodel.dft_scaleX
                onValueChanged: barkmodel.scaleX = value
            }
            RowSlider {
                width: parent.width
                text: qsTr("scale y"); color: "white"
                from: 0.1; to: 10.0
                value: barkmodel.dft_scaleY
                onValueChanged: barkmodel.scaleY = value
            }
        }
    }
}
