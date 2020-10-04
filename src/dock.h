#ifndef DOCK_H
#define DOCK_H

#include <QObject>

namespace Dock {
    Q_NAMESPACE

    enum Area {
        Center,
        Top,
        Bottom,
        Left,
        Right,
        Float
    };
    Q_ENUM_NS(Area)

    enum ButtonStatus {
        Normal,
        Hovered,
        Pressed,
        Checked
    };
    Q_ENUM_NS(ButtonStatus)

    enum DockWidgetDisplayType {
        Hidden,
        SplitView,
        TabbedView
    };
    Q_ENUM_NS(DockWidgetDisplayType)
//    extern const qreal resizeHandleSize;
}
#endif // DOCK_H
