import QtQuick 2.0

DockAreaQml {

    DockWidgetQml {
        dockArea: 'left'
        title: "Sample dock 2"
        contentItem: Text {
            anchors.centerIn: parent
            text: "This is dock 2"
        }
    }
    DockWidgetQml {
        id: dock
        title: "Sample dock"
        dockArea: 'left'
        contentItem: Rectangle {
            anchors.centerIn: parent
            color: 'green'
            Text {
                anchors.centerIn: parent
                text: qsTr("text: ") + nn
            }
        }
    }
}
