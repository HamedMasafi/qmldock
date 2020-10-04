#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QtQuickTemplates2/private/qquickpage_p.h>
#include "dock.h"

class DockGroup;
class DockWidgetHeader;
class DockWindow;
class DockWidget : public QQuickPage {
    Q_OBJECT
    DockGroup *_dockGroup;
    DockWidgetHeader *_header;

    Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(bool closable READ closable WRITE setClosable NOTIFY closableChanged)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool showHeader READ showHeader WRITE setShowHeader NOTIFY showHeaderChanged)
    Q_PROPERTY(bool detachable READ detachable WRITE setDetachable NOTIFY detachableChanged)

public:
    DockWidget(QQuickItem *parent = nullptr);

    DockGroup *dockGroup() const;
    void setDockGroup(DockGroup *dockGroup);

    Dock::Area area() const;

    bool closable() const;

    bool resizable() const;

    bool movable() const;

    bool showHeader() const;

    bool detachable() const;

public slots:
    void detach();
    void close();
    void restoreSize();
    void setArea(Dock::Area area);

    void setClosable(bool closable);

    void setResizable(bool resizable);

    void setMovable(bool movable);

    void setShowHeader(bool showHeader);

    void setDetachable(bool detachable);

private slots:
    void header_moveStarted();
    void header_moving(const QPointF &hotspot);
    void header_moveEnded();
    void this_titleChanged();

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *);

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

protected:
    void itemChange(ItemChange, const ItemChangeData &);

    // QQuickItem interface
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
};

#endif // DOCKWIDGET_H
