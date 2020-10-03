#ifndef DOCKGROUP_H
#define DOCKGROUP_H

#include <QQuickPaintedItem>
#include "dock.h"
//#include <QtQuickTemplates2/private/qquicksplitview_p.h>

class DockGroupPrivate;
class DockWidget;
class DockGroupResizeHandler;
class DockGroup : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    Q_PROPERTY(qreal panelSize READ panelSize WRITE setPanelSize NOTIFY panelSizeChanged)
    Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(bool enableResizing READ enableResizing WRITE setEnableResizing
                   NOTIFY enableResizingChanged)
    Q_PROPERTY(Dock::DockWidgetDisplayType displayType READ displayType WRITE setDisplayType NOTIFY displayTypeChanged)

    Q_DECLARE_PRIVATE(DockGroup);
    DockGroupPrivate *d_ptr;
public:
    DockGroup(QQuickItem *parent = nullptr);
    DockGroup(Dock::Area area, QQuickItem *parent = nullptr);

    void paint(QPainter *painter);
    void setColor(const QColor &color);
    bool isOpen() const;
    qreal panelSize() const;

    Dock::Area area() const;

    bool enableResizing() const;

    QList<DockWidget *> widgets() const;

    Dock::DockWidgetDisplayType displayType() const;

public slots:
    void addDockWidget(DockWidget *item);
    void removeDockWidget(DockWidget *item);
    void setIsOpen(bool isOpen);
    void setPanelSize(qreal panelSize);

    void setArea(Dock::Area area);

    void setEnableResizing(bool enableResizing);

    void setDisplayType(Dock::DockWidgetDisplayType displayType);

signals:
    void requestResize(const QRectF &rect, bool *ok);
    void isOpenChanged(bool isOpen);
    void panelSizeChanged(qreal panelSize);

    void areaChanged(Dock::Area area);

    void enableResizingChanged(bool enableResizing);

    void displayTypeChanged(Dock::DockWidgetDisplayType displayType);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private slots:
    void tabBar_currentIndexChanged(int index);
    void handler_moving(qreal pos, bool *ok);
    void handler_moved();

private:
//    bool isHorizontal() const;
//    bool isVertical() const;
//    bool acceptResizeEvent(const QPointF &point);
//    void fitItem(QQuickItem *item);

//    bool m_isOpen;
//    bool m_ownStartArea;
//    bool m_ownEndArea;
//    qreal m_panelSize;
//    QColor _color;
//    QList<DockWidget *> _dockWidgets;
//    QList<DockGroupResizeHandler *> _handlers;
//    QMap<int, qreal> _itemSizes;
//    qreal _lastGroupSize;
//    qreal _lastMousePos;
//    bool _mousepRessed;

//    void reorderItems();
//    void reorderHandles();
//    DockGroupResizeHandler *createHandlers();
//    Dock::Area m_area;
//    bool m_enableResizing;

protected:
    void hoverMoveEvent(QHoverEvent *event);
    bool childMouseEventFilter(QQuickItem *, QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};


#endif // DOCKGROUP_H
