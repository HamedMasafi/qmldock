import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Kaj.Dock 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    DockArea {
        id: dockArea
        anchors.fill: parent

        DockGroup {
            id: centerDockGroup
            area: Dock.Center
            displayType: Dock.TabbedView
        }

        DockGroup {
            id: leftDockGroup
            area: Dock.Left
            displayType: Dock.TabbedView
            tabPosition: Qt.LeftEdge
            panelSize: 200
        }

        CusomTitleBar {
        }

        DockWidget {
            area: Dock.Right
            title: "Right dock 1"
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("I am a dock on right")
            }
        }
        DockWidget {
            area: Dock.Right
            title: "Right dock 2"
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: qsTr("I am a dock on right too! Resize us!")
            }
        }
        DockWidget {
            area: Dock.Bottom
            title: "Bottom dock"
            allowedAreas: Dock.AllSides
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("I can't dock to center area")
            }
        }
        DockWidget {
            title: "Top dock"
            area: Dock.Top
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: qsTr("I am a dock on top. Move me outside of window!")
            }
        }

        DockWidget {
            area: Dock.Center
            title: "Center 1"
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("I am a dock on center")
            }
        }
//        DockWidget {
//            area: Dock.Center
//            title: "Center 2"
//            contentItem: Label {
//                verticalAlignment: Text.AlignVCenter
//                horizontalAlignment: Text.AlignHCenter
//                text: qsTr("I am a dock on center")
//            }
//        }
        DockWidget {
            area: Dock.Center
            title: "Settings"
            contentItem: CornerOwnerSettings { dockArea: dockArea }
        }
        DockWidget {
            area: Dock.Center
            title: "Tab potision"
            contentItem: TabPosition {
                dockGroup: centerDockGroup
            }
        }


    }
}
