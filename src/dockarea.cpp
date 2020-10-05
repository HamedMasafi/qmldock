#include "dockarea.h"
#include "dockmoveguide.h"
#include "dockwidget.h"
#include "dockgroup.h"
#include "dockgroupresizehandler.h"

#include <QDebug>
#include <QPainter>
#define Z_GROUP 100
#define Z_WIDGET 200
#define Z_WIDGET_FLOAT 300
#define Z_RESIZER 400
#define Z_GUIDE 500
DockArea::DockArea(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setClip(true);

    createGroup(Dock::Center);
    createGroup(Dock::Left);
    createGroup(Dock::Right);
    createGroup(Dock::Top);
    createGroup(Dock::Bottom);

    _dockGroups[Dock::Center]->setDisplayType(Dock::TabbedView);
    _dockGroups[Dock::Center]->setTabPosition(Qt::LeftEdge);

    _dockMoveGuide = new DockMoveGuide(this);
    _dockMoveGuide->setVisible(false);
    _dockMoveGuide->setPosition(QPointF(0, 0));
    _dockMoveGuide->setSize(QSizeF(30, 30));
    _dockMoveGuide->setZ(Z_GUIDE);
    _dockMoveGuide->update();

    _dockGroups[Dock::Center]->setColor(Qt::gray);
    _dockGroups[Dock::Top]->setColor(Qt::red);
    _dockGroups[Dock::Right]->setColor(Qt::green);
    _dockGroups[Dock::Bottom]->setColor(Qt::blue);
    _dockGroups[Dock::Left]->setColor(Qt::transparent);

//    auto d = new DockWidget(_dockGroups[1]);
//    _dockGroups[1]->addDockWidget(d);

//    auto d2 = new DockWidget(_dockGroups[1]);
    //    _dockGroups[1]->addDockWidget(d2);
}

void DockArea::paint(QPainter *painter)
{
    painter->fillRect(clipRect(), Qt::lightGray);
}

QList<DockWidget *> DockArea::dockWidgets() const
{
    return _dockWidgets;
}

void DockArea::itemChange(QQuickItem::ItemChange change,
                          const QQuickItem::ItemChangeData &data)
{
    if (change == QQuickItem::ItemChildAddedChange) {
        auto dw = qobject_cast<DockWidget *>(data.item);
        if (dw) {
            addDockWidget(dw);
        }
    }

    if (change == QQuickItem::ItemChildRemovedChange) {
        auto dw = qobject_cast<DockWidget *>(data.item);
        if (dw) {
            _dockWidgets.removeOne(dw);
            if (dw->dockGroup())
                dw->dockGroup()->removeDockWidget(dw);
      }
    }

    QQuickItem::itemChange(change, data);
}

void DockArea::addDockWidget(DockWidget *widget)
{
    widget->setZ(Z_WIDGET);

    _dockWidgets.append(widget);
    connect(widget,
            &DockWidget::beginMove,
            this,
            &DockArea::dockWidget_beginMove);

    connect(widget, &DockWidget::moving, this, &DockArea::dockWidget_moving);
    connect(widget, &DockWidget::moved, this, &DockArea::dockWidget_moved);
    connect(widget,
            &QQuickItem::visibleChanged,
            this,
            &DockArea::dockWidget_visibleChanged);

    switch (widget->area()) {
    case Dock::Left:
    case Dock::Right:
    case Dock::Top:
    case Dock::Bottom:
    case Dock::Center:
        _dockGroups[widget->area()]->addDockWidget(widget);

        break;
    case Dock::Float:
        break;
    }

    emit dockWidgetsChanged(_dockWidgets);
}

void DockArea::reorderDockGroups()
{
    QRectF rc;

    rc.setLeft(panelSize(Dock::Left));
    rc.setTop(panelSize(Dock::Top));
    rc.setWidth(width() - panelSize(Dock::Right) - panelSize(Dock::Left));
    rc.setHeight(height() - panelSize(Dock::Top) - panelSize(Dock::Bottom));

    _dockGroups[Dock::Left]->setPosition(QPointF(0, 0));
    _dockGroups[Dock::Left]->setSize(QSizeF(rc.left(), height()));

    _dockGroups[Dock::Top]->setPosition(QPointF(rc.left(), 0));
    _dockGroups[Dock::Top]->setSize(QSizeF(rc.width(), rc.top()));

    _dockGroups[Dock::Right]->setPosition(QPointF(rc.right(), 0));
    _dockGroups[Dock::Right]->setSize(QSizeF(width() - rc.right(), height()));

    _dockGroups[Dock::Bottom]->setPosition(rc.bottomLeft());
    _dockGroups[Dock::Bottom]->setSize(
        QSizeF(rc.width(), height() - rc.height()));

//    DockGroupResizeHandler *handler;
//    if (_dockGroups[Dock::Left]->isOpen()) {
//        handler = _resizeHandlers[Dock::Left];

//        handler->setVisible(true);
//        handler->setPosition(QPointF(rc.left() - DockStyle::instance()->resizeHandleSize(), 0));
//        rc.setLeft(rc.left() + DockStyle::instance()->resizeHandleSize());
//        handler->setHeight(_resizeHandlers[Dock::Left]->height());
//        handler->setWidth(20);
//    } else {
//        _resizeHandlers[Dock::Left]->setVisible(false);
//    }


    _dockGroups[Dock::Center]->setPosition(rc.topLeft());
    _dockGroups[Dock::Center]->setSize(rc.size());
}

void DockArea::dockWidget_beginMove()
{
    auto dw = qobject_cast<DockWidget *>(sender());

    for (auto d : _dockWidgets)
        d->setZ(d->z() - 1);
    dw->setZ(Z_WIDGET_FLOAT);

    if (dw->dockGroup()) {
        dw->setArea(Dock::Float);
        dw->dockGroup()->removeDockWidget(dw);
        dw->restoreSize();
    }
    //        dw->setParentItem(this);
    //        dw->setVisible(true);
    //        dw->setPosition(QPointF(0, 0));
    //        dw->setSize(QSizeF(200, 200));

    _dockMoveGuide->setSize(size());
    _dockMoveGuide->setVisible(true);
}

void DockArea::dockWidget_moving(const QPointF &pt)
{
    _dockMoveGuide->setMousePos(pt);
}

void DockArea::dockWidget_moved()
{
    _dockMoveGuide->setVisible(false);

    auto d = qobject_cast<DockWidget *>(sender());
    if (!d)
        return;

    switch (_dockMoveGuide->area()) {
    case Dock::Left:
    case Dock::Right:
    case Dock::Top:
    case Dock::Bottom:
    case Dock::Center:
        if (d->dockGroup() != _dockGroups[_dockMoveGuide->area()])
            _dockGroups[_dockMoveGuide->area()]->addDockWidget(d);
        d->setZ(Z_WIDGET);
        reorderDockGroups();
        break;

    case Dock::Float:
        break;
    }
}

void DockArea::dockWidget_visibleChanged()
{
    auto dw = qobject_cast<DockWidget *>(sender());
    if (!dw)
        return;

    if (dw->dockGroup()) {
        dw->dockGroup()->removeDockWidget(dw);
    }
}

void DockArea::geometryChanged(const QRectF &newGeometry,
                               const QRectF &oldGeometry)
{
    if (_dockGroups.keys().length() == 5) {
        reorderDockGroups();
    }
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

int DockArea::panelSize(Dock::Area area) const
{
    return _dockGroups[area]->isOpen()
               ? _dockGroups[area]->panelSize()
               : 0.;
}

DockGroup *DockArea::createGroup(Dock::Area area)
{
    if (_dockGroups.contains(area))
        return _dockGroups.value(area);

    auto d = new DockGroup(area, this);
    d->setVisible(true);
    d->setZ(Z_GROUP);
    d->setPanelSize(200);
    d->setDisplayType(Dock::SplitView);
    connect(d, &DockGroup::panelSizeChanged, this, &DockArea::reorderDockGroups);
    _dockGroups.insert(area, d);

    return d;
}
