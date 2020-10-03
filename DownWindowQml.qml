import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick 2.12

Window {
    id: window
    visible: false
    width: 200
    height: 200
    flags: Qt.Tool | Qt.FramelessWindowHint
    property int resizeGap: 10
    Item {
        id: floatingWindowContainer
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: 'red'
        }
    }

    DragHandler {
        id: resizeHandler

        grabPermissions: TapHandler.CanTakeOverFromAnything
        target: null
        onActiveChanged: {
            if (active) {
                var border = 30;
                const p = resizeHandler.centroid.position;
                let e = 0;
                if (p.x < border) e |= Qt.LeftEdge;
                if (p.x >= width - border) e |= Qt.RightEdge;
                if (p.y < border) e |= Qt.TopEdge;
                if (p.y >= height - border) e |= Qt.BottomEdge;
                window.startSystemResize(e);
            }
        }
    }
}
