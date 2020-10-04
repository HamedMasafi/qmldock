#ifndef DOCKAREA_H
#define DOCKAREA_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockWidget;
class DockMoveGuide;
class DockGroup;
class DockGroupResizeHandler;
class DockArea : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QList<DockWidget *> dockWidgets READ dockWidgets NOTIFY dockWidgetsChanged)

    QList<DockWidget *> _dockWidgets;
    DockMoveGuide *_dockMoveGuide;
    QMap<Dock::Area, DockGroup *> _dockGroups;
public:
    DockArea(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    QList<DockWidget *> dockWidgets() const;

signals:

    void dockWidgetsChanged(QList<DockWidget *> dockWidgets);

protected:
    void itemChange(ItemChange, const ItemChangeData &);

public slots:
    void addDockWidget(DockWidget *widget);
    void reorderDockGroups();

private slots:
    void dockWidget_beginMove();
    void dockWidget_moving(const QPointF &pt);
    void dockWidget_moved();
    void dockWidget_visibleChanged();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private:
    inline int panelSize(Dock::Area area) const;
    DockGroup *createGroup(Dock::Area area);

};

#endif // DOCKAREA_H
