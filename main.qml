import QtQuick 2.12
import QtQuick.Controls 2.5
import ImpGears 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ImpGears Demo ES3")

    header : ToolBar {
        id: headerBar

        // anchors.fill: parent;
        width: parent.width
        // parent: window.overlay

        Rectangle
        {
            anchors.fill: parent
            color: "#FF404142"
        }

        CustomButton
        {
            anchors.left: parent.left
            text: qsTr("<")
            onClicked: {
                loadinginfo.editMode = 0
                treemodel.editing = false
                leafmodel.editing = false
                barkmodel.editing = false
            }
            visible: loadinginfo.editMode>0
        }

        CustomButton
        {
            anchors.left: parent.left
            text: qsTr("<")
            onClicked: {
                controlPanelButton.ena=0
                treemodel.editing = false
                leafmodel.editing = false
                barkmodel.editing = false
            }
            visible: controlPanelButton.ena==1
        }

        Label {
            anchors.centerIn: parent
            text: "ImpGears Demo ES3"
            color: "white"
        }

        CustomButton
        {
            anchors.right: parent.right
            text: qsTr("OK")
            onClicked: {
                loadinginfo.validMode = loadinginfo.editMode
            }
            visible: loadinginfo.editMode==1
        }
    }

    footer : ToolBar {
        id: footerBar

        // anchors.fill: parent;

        width: parent.width
        // y: parent.height - childrenRect.height
        // parent: window.overlay
        Rectangle
        {
            anchors.fill: parent
            color: "#FF404142"
        }

        Label {
            anchors.left: parent.left
            text: "Last update : 13/03/2020\nImpGears commit:\n fd8e8fafb5b8450053ce195b16a33ffe7467ae26" //developper : Mathieu Boulet"
            color: "white"
            font.pixelSize: 10
        }

        CustomButton {
            property int ena: 0

            anchors.right: parent.right
            id: controlPanelButton
            text: qsTr("Params")

            onClicked: ena=ena==1?0:1
        }
    }


    ImpGearsItem
    {
       id: impGearsID
       anchors.fill : parent

       SequentialAnimation on t {
           NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
           NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
           loops: Animation.Infinite
           running: true
       }
    }

    LoadingNotifier {
        y: headerBar.height
        visible: loadinginfo.editMode==0
    }

    GeometryEdit {
        visible: loadinginfo.editMode==1
    }

    LeafEdit {
        visible: loadinginfo.editMode==2
    }

    BarkEdit {
        visible: loadinginfo.editMode==3
    }

    ControlPanel{
        visible: controlPanelButton.ena==1
    }

    SceneEdit {
        id: sceneedit
        y: headerBar.height
        height: parent.height - headerBar.height - footerBar.height

        interactive: loadinginfo.editMode==0
        visible: loadinginfo.editMode==0

        edittreebutton.onClicked:{
                loadinginfo.editMode = 1
                treemodel.editing = true
            }

        editleafbutton.onClicked:{
                loadinginfo.editMode = 2
                leafmodel.editing = true
            }

        editbarkbutton.onClicked:{
                loadinginfo.editMode = 3
                barkmodel.editing = true
            }
    }
}
