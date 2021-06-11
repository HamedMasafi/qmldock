#ifndef DOCKDOCKABLEITEM_P_H
#define DOCKDOCKABLEITEM_P_H

#include <qglobal.h>

class QQuickItem;
class DockPopoutWindow;
class DockDockableItem;
class DockDockableItemPrivate
{
    Q_DECLARE_PUBLIC(DockDockableItem);
    DockDockableItem *q_ptr;

public:
    DockDockableItemPrivate(DockDockableItem *parent);

    bool isPopedOut{false};
    DockPopoutWindow *window{nullptr};
    QQuickItem *item{nullptr};
};

#endif // DOCKDOCKABLEITEM_P_H
