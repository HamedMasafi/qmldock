#ifndef DOCKAREA_P_H
#define DOCKAREA_P_H

#include <QColor>
#include <QMap>
#include <QPointF>
#include <QRectF>
#include "dock.h"

class QQuickItem;
class DockWidget;
class DockArea;
class DockAreaResizeHandler;
class DockTabBar;
class DockAreaPrivate {
    DockArea *q_ptr;
    Q_DECLARE_PUBLIC(DockArea);

public:
    DockAreaPrivate(DockArea *parent);

    qreal panelSize;
    QList<DockWidget *> dockWidgets;
    QList<DockAreaResizeHandler *> handlers;
    QList<qreal> itemSizes;

    int currentIndex;
    bool isOpen;
    qreal lastGroupSize;
    qreal lastMousePos;
    bool mousePressed;
    Dock::Area area;
    bool enableResizing;
    DockTabBar *tabBar;
    QQuickItem *tabBarItem;
    QRectF usableArea;
    Dock::DockWidgetDisplayType displayType;
    QPointF tabBarPosition;
    qreal tabBarWidth;
    bool isPinned{true};

    void relayout();
    void arrangeTabBar();
    bool isHorizontal() const;
    bool isVertical() const;
    bool acceptResizeEvent(const QPointF &point);
    void fitItem(QQuickItem *item);
    void reorderItems();
    void reorderHandles();
    void normalizeItemSizes();

    void updateTabbedView();
    QRectF updateUsableArea();
    DockAreaResizeHandler *createHandlers();

    qreal minimumSize;
    qreal maximumSize;
    Qt::Edge tabPosition;
};

#endif // DOCKAREA_P_H
