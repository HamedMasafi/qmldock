import QtQuick 2.0
import QtQuick.Controls 2.12
import AI 1.0

DockArea {
    clip: true
    DockWidget{
        id: dockWidget
        title: "Sample dock"
        area: Dock.Left
        contentItem: Rectangle {
            color: 'yellow'
            Text {
                anchors.centerIn: parent
                text: qsTr("Hi")
            }
        }
    }

    DockWidget{
        title: "Sample dock 2"
        area: Dock.Left

        contentItem: Item {
            Text {
                anchors.centerIn: parent
                text: qsTr("inside frame")
            }
        }
    }

    DockWidget{
        title: "List view"
        area: Dock.Right

        contentItem: ListView {
            model: 60
            delegate: Text {
                text: modelData
            }
        }
    }

    DockWidget{
        title: "Center"
        area: Dock.Center

        contentItem: ListView {
            model: 60
            delegate: Text {
                text: modelData
            }
        }
    }

    DockWidget{
        title: "Center 2"
        area: Dock.Center

        contentItem: Item {
            Text {
                anchors.centerIn: parent
                text: "Page 2"
            }
        }
    }
}
