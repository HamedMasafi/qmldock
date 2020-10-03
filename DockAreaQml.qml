import QtQuick 2.0
import AI 1.0

Item {
    id: dockArea
    Component.onCompleted: {
        console.log("*", children.length)
        for (var i = 0; i < children.length; i++) {
            var c = children[i];
            console.log(i, c)

            if (c.dockArea === undefined) {
                console.log("ignore", c)
                continue;
            }


            console.log("da",i, c.dockArea)
            addDock(c)
            if (c.dockArea === "left") {
                leftArea.addDockWidget(c);

                console.log("add to left")
                --i;
                continue;
            }
            if (c.dockArea === "right") {
                rightArea.addDockWidget(c);
                console.log("add to right")
                --i;
                continue;
            }
            c.visible = false;
        }
    }

    function addDock(c) {
        c.onBeginDrag.connect(function() {
            if (c.dockGroup === null || c.dockGroup === undefined)
                return;
var f = false;
            for (var i = 0; i < c.dockGroup.count; i++) {
                if (c.dockGroup.itemAt(i) === c) {
                    c.dockGroup.takeItem(i)
                    f = true;
                }
            }
            if (!f) {
                console.log("Item not found");
                return;
            }

//            c.dockGroup.takeItem(i)
            c.parent = dockArea
            c.visible = true;
            c.x = c.y = 0
            c.dockGroup = null
            console.log("change parent")
            return;
        })
    }

    DockGroupQml {
        id: topArea
        height: panelSize
        anchors {
            left: parent.left
            leftMargin: leftArea.width
            top: parent.top
            right: parent.right
            rightMargin: rightArea.width
        }
        color: 'red'
    }
    DockGroupQml {
        id: leftArea
        orientation: Qt.Vertical
        width: panelSize
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        color: 'gray'

//        Rectangle {
//            color: 'red'
//        }
//        Rectangle {
//            color: 'blue'
//        }
    }
    DockGroupQml {
        id: rightArea
        width: panelSize
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        color: 'green'
    }
    DockGroupQml {
        id: bottomArea
        height: panelSize
        anchors {
            left: parent.left
            leftMargin: leftArea.width
            bottom: parent.bottom
            right: parent.right
            rightMargin: rightArea.width
        }
        color: 'blue'
    }
}
