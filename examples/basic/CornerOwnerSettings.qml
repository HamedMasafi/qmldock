import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Kaj.Dock 1.0

Pane {
    clip: true

    property DockContainer dockArea: null

    ColumnLayout {
        RowLayout {
            Label {
                text: "Top left owner"
            }
            ComboBox {
                model: ["Top", "Left"]
                onCurrentTextChanged: dockArea.topLeftOwner =
                                      (currentIndex == 0)
                                      ? Qt.TopEdge
                                      : Qt.LeftEdge
            }
        }
        RowLayout {
            Label {
                text: "Top right owner"
            }
            ComboBox {
                model: ["Top", "Right"]
                onCurrentTextChanged: dockArea.topRightOwner =
                                      (currentIndex == 0)
                                      ? Qt.TopEdge
                                      : Qt.RightEdge
            }
        }
        RowLayout {
            Label {
                text: "Bottom left owner"
            }
            ComboBox {
                model: ["Bottom", "Left"]
                onCurrentTextChanged: dockArea.bottomLeftOwner =
                                      (currentIndex == 0)
                                      ? Qt.BottomEdge
                                      : Qt.LeftEdge
            }
        }
        RowLayout {
            Label {
                text: "Bottom Right owner"
            }
            ComboBox {
                model: ["Bottom", "Right"]
                onCurrentTextChanged: dockArea.bottomRightOwner =
                                      (currentIndex == 0)
                                      ? Qt.BottomEdge
                                      : Qt.RightEdge
            }
        }
    }
}
