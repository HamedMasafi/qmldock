import QtQuick 2.0
import QtQuick.Controls 2.15

SplitView {
    id: root
    property int panelSize: isOpen ? 300 : 0
    property var area: Qt.AlignTop
    property bool ownFirst: false
    property bool ownEnd: false
    property bool isOpen: dockWidgetsCount > 0
    property alias color: bg.color
    property int dockWidgetsCount: 0
    background: Rectangle {
        id: bg
        border.color: 'green'
    }

    function addDockWidget(dock) {
        addItem(dock)
        dockWidgetsCount++
        dock.dockGroup = root;

        dock.onVisibleChanged.connect(function(){
            if (dock.visible)
                dockWidgetsCount++;
            else
                dockWidgetsCount--;
        })
    }

    function removeDockWidget(dock) {
        removeItem(dock)
        dockWidgetsCount--
    }

}
