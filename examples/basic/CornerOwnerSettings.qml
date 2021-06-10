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
                text: "Top left owner"
            }
            ComboBox {
                model: ["Left", "Top"]
                onCurrentTextChanged: dockContainer.topLeftOwner =
                                      (currentIndex == 0)
                                      ? Qt.LeftEdge
                                      : Qt.TopEdge
            }
        }
        RowLayout {
            Label {
                text: "Top right owner"
            }
            ComboBox {
                model: ["Right", "Top"]
                onCurrentTextChanged: dockContainer.topRightOwner =
                                      (currentIndex == 0)
                                      ? Qt.RightEdge
                                      : Qt.TopEdge
            }
        }
        RowLayout {
            Label {
                text: "Bottom left owner"
            }
            ComboBox {
                model: ["Left", "Bottom"]
                onCurrentTextChanged: dockContainer.bottomLeftOwner =
                                      (currentIndex == 0)
                                      ? Qt.LeftEdge
                                      : Qt.BottomEdge
            }
        }
        RowLayout {
            Label {
                text: "Bottom Right owner"
            }
            ComboBox {
                model: ["Right", "Bottom"]
                onCurrentTextChanged: dockContainer.bottomRightOwner =
                                      (currentIndex == 0)
                                      ? Qt.RightEdge
                                      : Qt.BottomEdge
            }
        }
    }
}
