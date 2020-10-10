#include "dockarea.h"
#include "dockmoveguide.h"
#include "dockwidget.h"
#include "dockgroup.h"
#include "dockgroupresizehandler.h"
#include "dockstyle.h"
#include "dockwindow.h"

#include <QDebug>
#include <QPainter>
#define Z_GROUP 100
#define Z_WIDGET 200
#define Z_WIDGET_FLOAT 300
#define Z_RESIZER 400
#define Z_GUIDE 500
DockArea::DockArea(QQuickItem *parent) : QQuickPaintedItem(parent)
        , m_topLeftOwner(Qt::LeftEdge)
        , m_topRightOwner(Qt::RightEdge)
        , m_bottomLeftOwner(Qt::LeftEdge)
        , m_bottomRightOwner(Qt::RightEdge)
{

    _dockMoveGuide = new DockMoveGuide(this);
    _dockMoveGuide->setVisible(false);
    _dockMoveGuide->setPosition(QPointF(0, 0));
    _dockMoveGuide->setSize(QSizeF(30, 30));
    _dockMoveGuide->setZ(Z_GUIDE);
    _dockMoveGuide->update();
}

void DockArea::componentComplete()
{
    if (!_dockGroups.contains(Dock::Center)) {
        createGroup(Dock::Center);
        _dockGroups[Dock::Center]->setDisplayType(Dock::TabbedView);
    }
    createGroup(Dock::Left);
    createGroup(Dock::Right);
    createGroup(Dock::Top);
    createGroup(Dock::Bottom);


    for (auto &dw : _initialWidgets)
        addDockWidget(dw);

    QQuickItem::componentComplete();
}

void DockArea::paint(QPainter *painter)
{
    DockStyle::instance()->paintDockArea(painter, this);
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
            if (isComponentComplete())
                addDockWidget(dw);
            else
                _initialWidgets.append(dw);
        }

        auto dg = qobject_cast<DockGroup *>(data.item);
        if (dg) {
            if (!_dockGroups.contains(dg->area()))
                _dockGroups.insert(dg->area(), dg);
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
    widget->setZ(widget->area() == Dock::Float ? Z_WIDGET_FLOAT : Z_WIDGET);

    widget->setDockArea(this);
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

    qreal leftStart, leftEnd;
    qreal topStart, topEnd;
    qreal rightStart, rightEnd;
    qreal bottomStart, bottomEnd;

    if (m_topLeftOwner == Qt::LeftEdge) {
        leftStart = 0;
        topStart = rc.left();
    } else {
        leftStart = rc.top();
        topStart = 0;
    }

    if (m_topRightOwner == Qt::RightEdge) {
        topEnd = rc.right();
        rightStart = 0;
    } else {
        topEnd = width();
        rightStart = rc.top();
    }

    if (m_bottomLeftOwner == Qt::LeftEdge) {
        leftEnd = height();
        bottomStart = rc.left();
    } else {
        leftEnd = rc.bottom();
        bottomStart = 0;
    }

    if (m_bottomRightOwner == Qt::RightEdge) {
        bottomEnd = rc.right();
        rightEnd = height();
    } else {
        bottomEnd = width();
        rightEnd = rc.bottom();
    }
    _dockGroups[Dock::Left]->setPosition(QPointF(0, leftStart));
    _dockGroups[Dock::Left]->setSize(QSizeF(rc.left(), leftEnd - leftStart));

    _dockGroups[Dock::Top]->setPosition(QPointF(topStart, 0));
    _dockGroups[Dock::Top]->setSize(QSizeF(topEnd - topStart, rc.top()));

    _dockGroups[Dock::Right]->setPosition(QPointF(rc.right(), rightStart));
    _dockGroups[Dock::Right]->setSize(QSizeF(width() - rc.right(), rightEnd - rightStart));

    _dockGroups[Dock::Bottom]->setPosition(QPointF(bottomStart, rc.bottom()));
    _dockGroups[Dock::Bottom]->setSize(QSizeF(bottomEnd - bottomStart, height() - rc.bottom()));

    _dockGroups[Dock::Center]->setPosition(rc.topLeft());
    _dockGroups[Dock::Center]->setSize(rc.size());
}

void DockArea::setTopLeftOwner(Qt::Edge topLeftOwner)
{
    if (topLeftOwner != Qt::LeftEdge && topLeftOwner != Qt::TopEdge) {
        qWarning() << "Invalid value for topLeftOwner: " << topLeftOwner;
        return;
    }
    if (m_topLeftOwner == topLeftOwner)
        return;

    m_topLeftOwner = topLeftOwner;
    if (isComponentComplete())
        reorderDockGroups();
    emit topLeftOwnerChanged(m_topLeftOwner);
}

void DockArea::setTopRightOwner(Qt::Edge topRightOwner)
{
    if (topRightOwner != Qt::TopEdge && topRightOwner != Qt::RightEdge) {
        qWarning() << "Invalid value for topRightOwner: " << topRightOwner;
        return;
    }

    if (m_topRightOwner == topRightOwner)
        return;

    m_topRightOwner = topRightOwner;
    if (isComponentComplete())
        reorderDockGroups();
    emit topRightOwnerChanged(m_topRightOwner);
}

void DockArea::setBottomLeftOwner(Qt::Edge bottomLeftOwner)
{
    if (bottomLeftOwner != Qt::BottomEdge && bottomLeftOwner != Qt::LeftEdge) {
        qWarning() << "Invalid value for bottomLeftOwner: " << bottomLeftOwner;
        return;
    }

    if (m_bottomLeftOwner == bottomLeftOwner)
        return;

    m_bottomLeftOwner = bottomLeftOwner;
    if (isComponentComplete())
        reorderDockGroups();
    emit bottomLeftOwnerChanged(m_bottomLeftOwner);
}

void DockArea::setBottomRightOwner(Qt::Edge bottomRightOwner)
{
    if (bottomRightOwner != Qt::BottomEdge && bottomRightOwner != Qt::RightEdge) {
        qWarning() << "Invalid value for bottomRightOwner: " << bottomRightOwner;
        return;
    }

    if (m_bottomRightOwner == bottomRightOwner)
        return;

    m_bottomRightOwner = bottomRightOwner;
    if (isComponentComplete())
        reorderDockGroups();
    emit bottomRightOwnerChanged(m_bottomRightOwner);
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

    _dockMoveGuide->setAllowedAreas(dw->allowedAreas());
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

    auto dw = qobject_cast<DockWidget *>(sender());
    if (!dw)
        return;

    switch (_dockMoveGuide->area()) {
    case Dock::Left:
    case Dock::Right:
    case Dock::Top:
    case Dock::Bottom:
    case Dock::Center:
        if (dw->dockGroup() != _dockGroups[_dockMoveGuide->area()]) {
            _dockGroups[_dockMoveGuide->area()]->addDockWidget(dw);
        }
        dw->setZ(Z_WIDGET);
        reorderDockGroups();
        break;
    case Dock::Float:
    case Dock::Detached:
        dw->setArea(_dockMoveGuide->area());
        break;

    default:
        break;
    }
}

void DockArea::dockWidget_visibleChanged()
{
    auto dw = qobject_cast<DockWidget *>(sender());
    if (!dw)
        return;

    if (dw->area() == Dock::Float)
        return;

    if (dw->dockGroup()) {
        auto dt = dw->dockGroup()->displayType();
        if (dt == Dock::TabbedView)
            return;
    }

    if (dw->isVisible()) {
        _dockGroups[dw->area()]->addDockWidget(dw);
    } else {
        if (dw->dockGroup()) {
            dw->dockGroup()->removeDockWidget(dw);
        }
    }
}

void DockArea::geometryChanged(const QRectF &newGeometry,
                               const QRectF &oldGeometry)
{
    if (isComponentComplete())
        reorderDockGroups();
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

int DockArea::panelSize(Dock::Area area) const
{
    return _dockGroups[area]->isOpen()
               ? _dockGroups[area]->panelSize()
               : 0.;
}

DockGroup *DockArea::createGroup(Dock::Area area, DockGroup *item)
{
    if (_dockGroups.contains(area))
        return _dockGroups.value(area);

    if (!item)
        item = new DockGroup(this);
    item->setArea(area);
    item->setVisible(true);
    item->setZ(Z_GROUP);
    item->setPanelSize(200);
    item->setDisplayType(Dock::SplitView);
    connect(item, &DockGroup::panelSizeChanged, this, &DockArea::reorderDockGroups);
    connect(item, &DockGroup::isOpenChanged, this, &DockArea::reorderDockGroups);
    _dockGroups.insert(area, item);

    return item;
}

Qt::Edge DockArea::topLeftOwner() const
{
    return m_topLeftOwner;
}

Qt::Edge DockArea::topRightOwner() const
{
    return m_topRightOwner;
}

Qt::Edge DockArea::bottomLeftOwner() const
{
    return m_bottomLeftOwner;
}

Qt::Edge DockArea::bottomRightOwner() const
{
    return m_bottomRightOwner;
}
