import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Kaj.Dock 1.0

Pane {
    clip: true

    property DockContainer dockContainer: null

    ColumnLayout {
        RowLayout {
            Label {
                text: "Tab position"
            }
            ComboBox {
                model: ["Top", "Left", "Right", "Bottom"]
                onCurrentTextChanged: {
                    switch (currentIndex) {
                    case 0:
                        dockContainer.tabPosition = Qt.TopEdge
                        break;
                    case 1:
                        dockContainer.tabPosition = Qt.LeftEdge
                        break;
                    case 2:
                        dockContainer.tabPosition = Qt.RightEdge
                        break;
                    case 3:
                        dockContainer.tabPosition = Qt.BottomEdge
                        break;
                    }
                }
            }
        }
    }
}
