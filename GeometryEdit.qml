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
        id: geoeditScroll
        anchors.fill: parent

        clip: true

        Column{
            width: geoeditScroll.width

            RowSlider{
                width: parent.width
                text: qsTr("base radius"); color: "white"
                from: 0.01; to: 10.0
                value: treemodel.dft_baseRadius
                onValueChanged: treemodel.baseRadius = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("base height"); color: "white"
                from: 0.1; to: 10.0
                value: treemodel.dft_baseHeight

                onValueChanged: treemodel.baseHeight = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("node count"); color: "white"
                from: 1; to: 10
                value: treemodel.dft_nodeCount
                onValueChanged: treemodel.nodeCount = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch div"); color: "white"
                from: 1; to: 10
                value: treemodel.dft_branchDiv
                onValueChanged: treemodel.branchDiv = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("branch staggering"); color: "white"
                from: 0.0; to: 1.0
                value: treemodel.dft_staggering
                onValueChanged: treemodel.staggering = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("tilt"); color: "white"
                from: 0; to: 3.0
                value: treemodel.dft_tilt
                onValueChanged: treemodel.tilt = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("radius factor"); color: "white"
                from: 0.1; to: 1.0
                value: treemodel.dft_radiusFct
                onValueChanged: treemodel.radiusFct = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("height factor"); color: "white"
                from: 0.1; to: 1.0
                value: treemodel.dft_heightFct
                onValueChanged: treemodel.heightFct = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("leaf size"); color: "white"
                from: 0.1; to: 10.0
                value: treemodel.dft_leafSize
                onValueChanged: treemodel.leafSize = value
            }

            RowSlider {
                width: parent.width
                text: qsTr("leaf count"); color: "white"
                from: 0; to: 10
                value: treemodel.dft_leafCount
                onValueChanged: treemodel.leafCount = value
            }

            Text { text: qsTr("leaf color"); color: "white" }
            ColorEdit
            {
                width: parent.width
                color: treemodel.dft_leafColor;
                onColorChanged: treemodel.leafColor = color
            }

            Text { text: qsTr("bark color"); color: "white" }
            ColorEdit
            {
                width: parent.width
                color: treemodel.dft_barkColor;
                onColorChanged: treemodel.barkColor = color
            }
        }
    }
}
