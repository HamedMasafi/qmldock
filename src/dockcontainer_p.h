#ifndef DOCKCONTAINER_P_H
#define DOCKCONTAINER_P_H

#include "dock.h"

class DockWidget;
class DockMoveGuide;
class DockArea;
class DockContainer;
class DockContainerPrivate {
    DockContainer *q_ptr;
    Q_DECLARE_PUBLIC(DockContainer);
public:
    DockContainerPrivate(DockContainer *parent);
    QList<DockWidget *> initialWidgets;
    QList<DockWidget *> dockWidgets;
    QMap<Dock::Area, DockArea *> dockAreas;

    DockMoveGuide *dockMoveGuide;

    Qt::Edge topLeftOwner;
    Qt::Edge topRightOwner;
    Qt::Edge bottomLeftOwner;
    Qt::Edge bottomRightOwner;
    DockWidget *activeDockWidget;
    Dock::DockWidgetDisplayType defaultDisplayType;

    bool enableStateStoring;
};

#endif // DOCKCONTAINER_P_H
