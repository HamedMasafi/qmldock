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

    DockContainer {
        id: dockArea
        anchors.fill: parent
        enableStateStoring: true

        DockArea {
            id: centerDockGroup
            area: Dock.Center
            displayType: Dock.TabbedView
        }

        DockArea {
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
            contentItem: ListView {
                id: listView60
                model: 60
                clip: true
                delegate: Text {
                    width: listView60.width
                    text: modelData
                    color: 'white'
                }
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
            titleBar: null
            contentItem: Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: qsTr("I have no titlebar!")
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
            contentItem: CornerOwnerSettings { dockContainer: dockArea }
        }
        DockWidget {
            area: Dock.Center
            title: "Tab potision"
            contentItem: TabPosition {
                dockContainer: centerDockGroup
            }
        }


    }
}
