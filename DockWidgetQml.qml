import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import AI 1.0

Item {
    id: dockWidget
    height: 200
    width: 200
    clip: true


    signal beginDrag()
    signal moveDrag(int x, int y)
    //    property alias contentItem: page.contentItem
    property Item contentItem: null
    property string title
    property bool isFloat: false
    property string dockArea: "left"
    property DockGroupQml dockGroup: null

    SplitView.preferredWidth: 300
    SplitView.preferredHeight: 300
    SplitView.minimumHeight: header.height

    onContentItemChanged: {

    }

    function detach() {
        if (isFloat)
            return;

        isFloat = true
        page.parent = window.contentItem
        //        win.contentItem.children = dockWidget.contentItem.children
        //        floatingWindowContainer.children = dockWidget.contentItem.children
        window.visible = true
        dockWidget.visible = false
    }

    //    Page {
    //        id: page
    //        anchors.fill: parent

    //        background: Rectangle {border.color: 'red'}

    //        header: ToolBar {

    //            RowLayout {
    //                anchors.fill: parent

    //                Label {
    //                    text: dockWidget.title
    //                    Layout.fillHeight: true
    //                    Layout.fillWidth: true
    //                    verticalAlignment: Qt.AlignVCenter
    ////                    MouseArea{
    ////                        id: mouse
    ////                        anchors.fill: parent
    ////                        drag {
    ////                            target: dockWidget
    ////                        }
    ////                        onMouseXChanged: moveDrag(dockWidget.x + mouseX, dockWidget.y + mouseY)
    ////                        onMouseYChanged: moveDrag(dockWidget.x + mouseX, dockWidget.y + mouseY)
    ////                    }
    //                    DragHandler {
    //                        onActiveChanged: if (active) window.startSystemMove();
    //                        target: null
    //                    }

    //                }

    //                ToolButton {
    //                    text: "P"
    //                    onClicked: {
    //                        detach()
    //                    }
    //                }
    //                ToolButton {
    //                    text: "X"
    //                    onClicked: {
    //                        dockWidget.visible = false
    //                    }
    //                }
    //            }
    //        }

    //    }
    Rectangle {
        id: page
        anchors.fill: parent
//        padding: 0
        border.color: 'green'

        Item {
            id: header
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 40
            RowLayout {
                anchors.fill: parent
                spacing: 0
                Label {
                    leftInset: 6
                    text: dockWidget.title
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    verticalAlignment: Qt.AlignVCenter
                    elide: Text.ElideRight
                    MouseArea {
                        anchors.fill: parent
//                        onPressed: dockWidget.beginDrag()
//                        drag.target: dockWidget
//                        Drag.dragType: "Internal"

//                        Drag.onDragStarted: {
//                            dockWidget.beginDrag()
//                        }

                        property int lastX: 0
                        property int lastY: 0
                        property bool moveStarted: false
                        preventStealing: true
                        onPressed: {
                            console.log("begin")
                            lastX = mouseX
                            lastY = mouseY
                            moveStarted = false;
                        }
                        onMouseXChanged: {
                            if (!moveStarted) {
                                moveStarted = true
                                dockWidget.beginDrag()
                            }
//                            dockWidget.x = ()
                        }
                        onMouseYChanged: {
                            if (!moveStarted) {
                                moveStarted = true
                            }
                            dockWidget.beginDrag()
//                            dockWidget.x = ()
                        }
                    }

//                    DragHandler {
//                        onActiveChanged: {
//                            dockWidget.beginDrag()
////                            if (active) {
////                                if (isFloat)
////                                    window.startSystemMove();
////                                else
////                                    detach();
////                            }
//                        }
//                        target: null
//                    }

                }

                ToolButton {
                    text: "P"
                    onClicked: {
                        detach()
                    }
                }
                ToolButton {
                    text: "X"
                    onClicked: {
                        if (isFloat)
                            window.hide();
                        else
                            dockWidget.visible = false;
                    }
                }
            }
        }
        Rectangle {
            color: 'yellow'
            id: contentItemContainer
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: header.height
                leftMargin: 1
                rightMargin: 1
                bottomMargin: 1
            }

            children: [dockWidget.contentItem]
        }
    }


    DownWindowQml {
        id: window
        visible: false
    }
}
