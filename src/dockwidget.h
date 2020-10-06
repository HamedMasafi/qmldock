#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockGroup;
class DockWidgetHeader;
class DockWindow;
class DockWidget : public QQuickPaintedItem {
    Q_OBJECT
    DockGroup *_dockGroup;
    DockWidgetHeader *_header;

    Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(bool closable READ closable WRITE setClosable NOTIFY closableChanged)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool showHeader READ showHeader WRITE setShowHeader NOTIFY showHeaderChanged)
    Q_PROPERTY(bool detachable READ detachable WRITE setDetachable NOTIFY detachableChanged)
    Q_PROPERTY(QQuickItem *contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    DockWidget(QQuickItem *parent = nullptr);

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

public slots:
    void detach();
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

private slots:
    void header_moveStarted();
    void header_moving(const QPointF &hotspot);
    void header_moveEnded();

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void itemChange(ItemChange, const ItemChangeData &) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

signals:
    void beginMove();
    void moving(const QPointF &pt);
    void moved();
    void areaChanged(Dock::Area area);
    void closableChanged(bool closable);
    void resizableChanged(bool resizable);
    void movableChanged(bool movable);
    void showHeaderChanged(bool showHeader);
    void detachableChanged(bool detachable);
    void contentItemChanged(QQuickItem * contentItem);
    void titleChanged(QString title);

private:
    Dock::Area m_area;
    QSizeF _originalSize;
    bool isDetached;
    DockWindow *dockWindow;

    bool m_closable;
    bool m_resizable;
    bool m_movable;
    bool m_showHeader;
    bool m_detachable;

    QQuickItem * m_contentItem;
    QString m_title;
};

#endif // DOCKWIDGET_H
