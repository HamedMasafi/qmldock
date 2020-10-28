#ifndef DOCKWIDGET_P_H
#define DOCKWIDGET_P_H

#include "dock.h"

#include <QRectF>
#include <QSizeF>

class DockWidget;
class DockWindow;
class QQuickItem;
class DockArea;
class DockGroup;
class DockWidgetHeader;
class DockWidgetPrivate {
    DockWidget *q_ptr;
    Q_DECLARE_PUBLIC(DockWidget);

public:
    DockWidgetPrivate(DockWidget *parent);

    Dock::Area area;
    Dock::Areas allowedAreas;

    QSizeF originalSize;

    bool closable;
    bool resizable;
    bool movable;
    bool showHeader;

    QString title;

    QQuickItem * contentItem;
    QQuickItem * titleBarItem;
    DockWidgetHeader *titleBar;
    DockWindow *dockWindow;
    DockArea *dockArea;
    DockGroup *dockGroup;

    bool isClosed;
    int resizeEdge;
    QRectF rectBeforeResize;
    QPointF resizeMousePos;

    Q_DECL_DEPRECATED
    bool detachable;
    Q_DECL_DEPRECATED
    bool isDetached;
};

#endif // DOCKWIDGET_P_H
