#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockGroup;
class DockWidgetHeader;
class DockWindow;
class DockArea;
class DockWidgetPrivate;
class DockWidget : public QQuickPaintedItem {
    Q_OBJECT

    Q_DECLARE_PRIVATE(DockWidget);
    DockWidgetPrivate *d_ptr;

    Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(bool closable READ closable WRITE setClosable NOTIFY closableChanged)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool showHeader READ showHeader WRITE setShowHeader NOTIFY showHeaderChanged)
    Q_PROPERTY(bool detachable READ detachable WRITE setDetachable NOTIFY detachableChanged)
    Q_PROPERTY(QQuickItem *contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged FINAL)
    Q_PROPERTY(QQuickItem *titleBar READ titleBar WRITE setTitleBar NOTIFY titleBarChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(Dock::Areas allowedAreas READ allowedAreas WRITE setAllowedAreas NOTIFY allowedAreasChanged)
//    Q_PROPERTY(DockWidgetFlags flags READ flags WRITE setFlags NOTIFY flagsChanged)

public:
    enum DockWidgetFlag {
        Movable,
        Detachable,
        Resizable
    };
    Q_DECLARE_FLAGS(DockWidgetFlags, DockWidgetFlag);
    Q_FLAG(DockWidgetFlags)

    DockWidget(QQuickItem *parent = nullptr);
    virtual ~DockWidget();

    DockGroup *dockGroup() const;
    Dock::Area area() const;
    bool closable() const;
    bool resizable() const;
    bool movable() const;
    bool showHeader() const;
    bool detachable() const;
    void paint(QPainter *painter) override;
    QQuickItem * contentItem() const;

    QString title() const;

    DockArea *dockArea() const;
    void setDockArea(DockArea *dockArea);

    bool getIsDetached() const;

    Dock::Areas allowedAreas() const;
    DockWindow *dockWindow() const;

    QQuickItem * titleBar() const;
    void componentComplete() override;

public slots:
    Q_DECL_DEPRECATED
    void detach();

    void beginDetach();
    void close();
    void restoreSize();

    void setDockGroup(DockGroup *dockGroup);
    void setArea(Dock::Area area);
    void setClosable(bool closable);
    void setResizable(bool resizable);
    void setMovable(bool movable);
    void setShowHeader(bool showHeader);
    void setDetachable(bool detachable);

    void setContentItem(QQuickItem * contentItem);

    void setTitle(QString title);

    void setAllowedAreas(Dock::Areas allowedAreas);

    void setTitleBar(QQuickItem * titleBar);

private slots:
    void header_moveStarted();
    void header_moving(const QPointF &windowPos, const QPointF &cursorPos);
    void header_moveEnded();

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void itemChange(ItemChange, const ItemChangeData &) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void beginMove();
    void moving(const QPointF &pt);
    void moved();
    void closed();
    void areaChanged(Dock::Area area);
    void closableChanged(bool closable);
    void resizableChanged(bool resizable);
    void movableChanged(bool movable);
    void showHeaderChanged(bool showHeader);
    void detachableChanged(bool detachable);
    void contentItemChanged(QQuickItem * contentItem);
    void titleChanged(QString title);
    void allowedAreasChanged(Dock::Areas allowedAreas);

    friend class DockWidgetHeader;
    void titleBarChanged(QQuickItem * titleBar);

};

#endif // DOCKWIDGET_H
