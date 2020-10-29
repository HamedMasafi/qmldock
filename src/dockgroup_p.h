#ifndef DOCKGROUP_P_H
#define DOCKGROUP_P_H

#include <QColor>
#include <QMap>
#include <QPointF>
#include <QRectF>
#include "dock.h"

class QQuickItem;
class DockWidget;
class DockGroup;
class DockGroupResizeHandler;
class DockTabBar;
class DockGroupPrivate {
    DockGroup *q_ptr;
    Q_DECLARE_PUBLIC(DockGroup);

public:
    DockGroupPrivate(DockGroup *parent);

    qreal panelSize;
    QList<DockWidget *> dockWidgets;
    QList<DockGroupResizeHandler *> handlers;
    QList<qreal> itemSizes;

    int currentIndex;
    bool isOpen;
    qreal lastGroupSize;
    qreal lastMousePos;
    bool mousepRessed;
    Dock::Area area;
    bool enableResizing;
    DockTabBar *tabBar;
    QQuickItem *tabBarItem;
    QRectF usableArea;
    Dock::DockWidgetDisplayType displayType;

    bool isHorizontal() const;
    bool isVertical() const;
    bool acceptResizeEvent(const QPointF &point);
    void fitItem(QQuickItem *item);
    void reorderItems();
    void reorderHandles();
    void normalizeItemSizes();
    QRectF updateUsableArea();
    DockGroupResizeHandler *createHandlers();

    qreal minimumSize;
    qreal maximumSize;
    Qt::Edge tabPosition;
};

#endif // DOCKGROUP_P_H
