#ifndef DOCKGROUP_H
#define DOCKGROUP_H

#include "dock.h"

#include <QQuickPaintedItem>
//#include <QtQuickTemplates2/private/qquicksplitview_p.h>

class DockGroupPrivate;
class DockWidget;
class DockGroupResizeHandler;
class DockGroup : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    Q_PROPERTY(qreal panelSize READ panelSize WRITE setPanelSize NOTIFY
                   panelSizeChanged)
    Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(bool enableResizing READ enableResizing WRITE setEnableResizing
                   NOTIFY enableResizingChanged)
    Q_PROPERTY(Dock::DockWidgetDisplayType displayType READ displayType WRITE
                   setDisplayType NOTIFY displayTypeChanged)
    Q_PROPERTY(qreal minimumSize READ minimumSize WRITE setMinimumSize NOTIFY
                   minimumSizeChanged)
    Q_PROPERTY(qreal maximumSize READ maximumSize WRITE setMaximumSize NOTIFY
                   maximumSizeChanged)
    Q_PROPERTY(QList<DockWidget *> dockWidgets READ dockWidgets NOTIFY
                   dockWidgetsChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
                   currentIndexChanged)
    Q_PROPERTY(Qt::Edge tabPosition READ tabPosition WRITE setTabPosition NOTIFY
                   tabPositionChanged)
    Q_PROPERTY(QQuickItem *tabBar READ tabBar WRITE setTabBar NOTIFY tabBarChanged)

    Q_DECLARE_PRIVATE(DockGroup);
    DockGroupPrivate *d_ptr;

public:
    DockGroup(QQuickItem *parent = nullptr);

    void paint(QPainter *painter);
    bool isOpen() const;
    qreal panelSize() const;
    Dock::Area area() const;
    bool enableResizing() const;
    QList<DockWidget *> widgets() const;
    Dock::DockWidgetDisplayType displayType() const;
    qreal minimumSize() const;
    qreal maximumSize() const;
    QList<DockWidget *> dockWidgets() const;
    int currentIndex() const;
    Qt::Edge tabPosition() const;
    void componentComplete();

    QQuickItem * tabBar() const;

public slots:
    void addDockWidget(DockWidget *item);
    void removeDockWidget(DockWidget *item);
    void setIsOpen(bool isOpen);
    void setPanelSize(qreal panelSize);
    void setArea(Dock::Area area);
    void setEnableResizing(bool enableResizing);
    void setDisplayType(Dock::DockWidgetDisplayType displayType);
    void setMinimumSize(qreal minimumSize);
    void setMaximumSize(qreal maximumSize);
    void setTabPosition(Qt::Edge tabPosition);
    void setCurrentIndex(int currentIndex);

    void setTabBar(QQuickItem * tabBar);

signals:
    void requestResize(const QRectF &rect, bool *ok);
    void isOpenChanged(bool isOpen);
    void panelSizeChanged(qreal panelSize);
    void areaChanged(Dock::Area area);
    void enableResizingChanged(bool enableResizing);
    void displayTypeChanged(Dock::DockWidgetDisplayType displayType);
    void minimumSizeChanged(qreal minimumSize);
    void maximumSizeChanged(qreal maximumSize);
    void dockWidgetsChanged(QList<DockWidget *> name);
    void currentIndexChanged(int currentIndex);
    void tabPositionChanged(Qt::Edge tabPosition);

    void tabBarChanged(QQuickItem * tabBar);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void updatePolish();

private slots:
    void dockWidget_closed();
    void tabBar_currentIndexChanged(int index);
    void handler_moving(qreal pos, bool *ok);
    void handler_moved();

protected:
    void hoverMoveEvent(QHoverEvent *event);
    bool childMouseEventFilter(QQuickItem *, QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // DOCKGROUP_H
