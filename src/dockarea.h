#ifndef DOCKAREA_H
#define DOCKAREA_H

#include "dock.h"

#include <QQuickPaintedItem>
//#include <QtQuickTemplates2/private/qquicksplitview_p.h>

class DockAreaPrivate;
class DockWidget;
class DockAreaResizeHandler;
class DockArea : public QQuickPaintedItem {
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
    Q_PROPERTY(bool isPinned READ isPinned WRITE setIsPinned NOTIFY isPinnedChanged)

    Q_DECLARE_PRIVATE(DockArea);
    DockAreaPrivate *d_ptr;

public:
    DockArea(QQuickItem *parent = nullptr);
    virtual ~DockArea();

    void paint(QPainter *painter) override;
    void componentComplete() override;
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
    QQuickItem * tabBar() const;

    bool isPinned() const;

private:
    qreal minSize() const;
    qreal realSize() const;
    void addDockWidget(DockWidget *item);
    void removeDockWidget(DockWidget *item);

public Q_SLOTS:
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
    void setIsPinned(bool newIsPinned);

Q_SIGNALS:
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

    void isPinnedChanged();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;
    void hoverMoveEvent(QHoverEvent *event) override;
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private Q_SLOTS:
    void dockWidget_closed();
    void tabBar_tabClicked(int index);
    void tabBar_closeButtonClicked(int index);
    void handler_moving(qreal pos, bool *ok);
    void handler_moved();

    friend class DockContainer;
};

QDebug operator<<(QDebug debug, const DockArea *area);

#endif // DOCKAREA_H
