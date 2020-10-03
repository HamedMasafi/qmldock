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
//    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
public:
    DockWidget(QQuickItem *parent = nullptr);

    DockGroup *dockGroup() const;
    void setDockGroup(DockGroup *dockGroup);

    Dock::Area area() const;

public slots:
    void detach();
    void close();
    void restoreSize();
    void setArea(Dock::Area area);

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

private:
    Dock::Area m_area;
    QSizeF _originalSize;
    bool isDetached;
    DockWindow *dockWindow;

protected:
    void itemChange(ItemChange, const ItemChangeData &);

    // QQuickItem interface
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
};

#endif // DOCKWIDGET_H
