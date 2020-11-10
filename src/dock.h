#ifndef DOCK_H
#define DOCK_H

#include <QObject>
#include <QQuickItem>
#include <qobjectdefs.h>

namespace Dock {
Q_NAMESPACE

enum Area {
    NoArea = 0,
    Top = 1,
    Bottom = 2,
    Left = 4,
    Right = 8,
    Float = 16,
    Detached = 32,
    Center = 64,
    AllSides = Top | Left | Right | Bottom,
    AllInsides = AllSides | Center | Float,
    AllAreas = AllInsides | Detached
};
Q_ENUM_NS(Area)
Q_DECLARE_FLAGS(Areas, Area)
Q_FLAG_NS(Areas)

enum ButtonStatus { Normal, Hovered, Pressed, Checked };
Q_ENUM_NS(ButtonStatus)

enum DockWidgetDisplayType { Hidden, SplitView, TabbedView, StackedView };
Q_ENUM_NS(DockWidgetDisplayType)

enum Icon {
    CloseIcon,
    PinIcon,
    PopoutIcon,

    CenterDropIcon,
    LeftDropIcon,
    RightDropIcon,
    UpDropIcon,
    DownDropIcon,

    LeftArrowIcon,
    RightArrowIcon,
    UpArrowIcon,
    DownArrowIcon,

};

template<class T>
T *findInParents(QQuickItem *item)
{
    auto i = item;

    while (i) {
        auto p = qobject_cast<T *>(i);
        if (p)
            return p;
        i = i->parentItem();
    }
    return nullptr;
}
} // namespace Dock
#endif // DOCK_H
