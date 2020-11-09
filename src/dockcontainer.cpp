#include "dockcontainer.h"
#include "dockmoveguide.h"
#include "dockwidget.h"
#include "dockarea.h"
#include "dockgroupresizehandler.h"
#include "style/abstractstyle.h"
#include "dockwindow.h"
#include "dockarea.h"

#include <QDebug>
#include <QPainter>
#include <QSettings>
#define Z_GROUP 100
#define Z_WIDGET 200
#define Z_WIDGET_FLOAT 300
#define Z_RESIZER 400
#define Z_GUIDE 500
DockContainer::DockContainer(QQuickItem *parent) : QQuickPaintedItem(parent)
        , m_topLeftOwner(Qt::LeftEdge)
        , m_topRightOwner(Qt::RightEdge)
        , m_bottomLeftOwner(Qt::LeftEdge)
        , m_bottomRightOwner(Qt::RightEdge)
{

    _dockMoveGuide = new DockMoveGuide(this);
}

DockContainer::~DockContainer()
{
    if (m_enableStateStoring)
        storeSettings();
}

void DockContainer::componentComplete()
{
    if (!_dockAreas.contains(Dock::Center)) {
        createGroup(Dock::Center);
        _dockAreas[Dock::Center]->setDisplayType(Dock::TabbedView);
    }
    connect(window(), &QQuickWindow::activeFocusItemChanged, [this]() {
        auto dockWidget = Dock::findInParents<DockWidget>(
            window()->activeFocusItem());
        if (dockWidget)
            qDebug() << "Dock=" << dockWidget->title();
    });
    createGroup(Dock::Left);
    createGroup(Dock::Right);
    createGroup(Dock::Top);
    createGroup(Dock::Bottom);

    for (auto &dw : _initialWidgets)
        addDockWidget(dw);

    for (auto &dg: _dockAreas.values()) {
        connect(dg, &DockArea::panelSizeChanged, this, &DockContainer::reorderDockAreas);
        connect(dg, &DockArea::isOpenChanged, this, &DockContainer::reorderDockAreas);
    }

    _dockAreas[Dock::Left]->polish();
    _dockAreas[Dock::Top]->polish();
    _dockAreas[Dock::Right]->polish();
    _dockAreas[Dock::Bottom]->polish();
    _dockAreas[Dock::Center]->polish();
    //    reorderDockAreas();
    geometryChanged(QRectF(), QRectF());

    QQuickItem::componentComplete();
}

void DockContainer::storeSettings()
{
    QSettings set;
    QMetaEnum e = QMetaEnum::fromType<Dock::Area>();

    for (auto &dw : _dockWidgets) {
        set.beginGroup(dw->title());
        set.setValue("area", e.valueToKey(dw->area()));
        set.setValue("position", QStringLiteral("%1,%2").arg(dw->x()).arg(dw->y()));
        set.setValue("size", QStringLiteral("%1,%2").arg(dw->width()).arg(dw->height()));
        set.setValue("visible", dw->isVisible());
        set.endGroup();
    }

    set.sync();
    qDebug() << set.fileName();
}

void DockContainer::restoreSettings()
{

}

void DockContainer::paint(QPainter *painter)
{
    dockStyle->paintDockContainer(painter, this);
}

QList<DockWidget *> DockContainer::dockWidgets() const
{
    return _dockWidgets;
}

void DockContainer::itemChange(QQuickItem::ItemChange change,
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

        auto dg = qobject_cast<DockArea *>(data.item);
        if (dg) {
            if (!_dockAreas.contains(dg->area()))
                _dockAreas.insert(dg->area(), dg);
        }
    }

    if (change == QQuickItem::ItemChildRemovedChange) {
        auto dw = qobject_cast<DockWidget *>(data.item);
        if (dw) {
            _dockWidgets.removeOne(dw);
            if (dw->dockArea())
                dw->dockArea()->removeDockWidget(dw);
      }
    }

    QQuickItem::itemChange(change, data);
}

void DockContainer::addDockWidget(DockWidget *widget)
{
    widget->setZ(widget->area() == Dock::Float ? Z_WIDGET_FLOAT : Z_WIDGET);

    widget->setDockContainer(this);
//    widget->setParentItem(this);
    _dockWidgets.append(widget);

    connect(widget,
            &DockWidget::beginMove,
            this,
            &DockContainer::dockWidget_beginMove,
            Qt::QueuedConnection);

    connect(widget, &DockWidget::moving, this, &DockContainer::dockWidget_moving);
    connect(widget, &DockWidget::moved, this, &DockContainer::dockWidget_moved);
    connect(widget,
            &QQuickItem::visibleChanged,
            this,
            &DockContainer::dockWidget_visibleChanged);

    if (m_enableStateStoring) {
        QSettings set;
        QMetaEnum e = QMetaEnum::fromType<Dock::Area>();
        set.beginGroup(widget->title());
        if (set.contains("area"))
            widget->setArea(
                (Dock::Area) e.keyToValue(set.value("area", widget->area())
                                              .toString()
                                              .toLocal8Bit()
                                              .data()));
    }
    switch (widget->area()) {
    case Dock::Left:
    case Dock::Right:
    case Dock::Top:
    case Dock::Bottom:
    case Dock::Center:
        _dockAreas[widget->area()]->addDockWidget(widget);
        _dockAreas[widget->area()]->polish();
        break;

        //TODO: remove this or keep!
    case Dock::NoArea:
        _dockAreas[Dock::Center]->addDockWidget(widget);
        _dockAreas[Dock::Center]->polish();
        break;
    default:
        qDebug() << "dock has no area " << widget->title();
        break;
    }

    qDebug() << widget->title() << "added to" << widget->area();
    if (isComponentComplete())
        reorderDockAreas();

    emit dockWidgetsChanged(_dockWidgets);
}

void DockContainer::reorderDockAreas()
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
    _dockAreas[Dock::Left]->setPosition(QPointF(0, leftStart));
    _dockAreas[Dock::Left]->setSize(QSizeF(rc.left(), leftEnd - leftStart));

    _dockAreas[Dock::Top]->setPosition(QPointF(topStart, 0));
    _dockAreas[Dock::Top]->setSize(QSizeF(topEnd - topStart, rc.top()));

    _dockAreas[Dock::Right]->setPosition(QPointF(rc.right(), rightStart));
    _dockAreas[Dock::Right]->setSize(QSizeF(width() - rc.right(), rightEnd - rightStart));

    _dockAreas[Dock::Bottom]->setPosition(QPointF(bottomStart, rc.bottom()));
    _dockAreas[Dock::Bottom]->setSize(QSizeF(bottomEnd - bottomStart, height() - rc.bottom()));

    _dockAreas[Dock::Center]->setPosition(rc.topLeft());
    _dockAreas[Dock::Center]->setSize(rc.size());
}

void DockContainer::setTopLeftOwner(Qt::Edge topLeftOwner)
{
    if (topLeftOwner != Qt::LeftEdge && topLeftOwner != Qt::TopEdge) {
        qWarning() << "Invalid value for topLeftOwner: " << topLeftOwner;
        return;
    }
    if (m_topLeftOwner == topLeftOwner)
        return;

    m_topLeftOwner = topLeftOwner;
    if (isComponentComplete())
        reorderDockAreas();
    emit topLeftOwnerChanged(m_topLeftOwner);
}

void DockContainer::setTopRightOwner(Qt::Edge topRightOwner)
{
    if (topRightOwner != Qt::TopEdge && topRightOwner != Qt::RightEdge) {
        qWarning() << "Invalid value for topRightOwner: " << topRightOwner;
        return;
    }

    if (m_topRightOwner == topRightOwner)
        return;

    m_topRightOwner = topRightOwner;
    if (isComponentComplete())
        reorderDockAreas();
    emit topRightOwnerChanged(m_topRightOwner);
}

void DockContainer::setBottomLeftOwner(Qt::Edge bottomLeftOwner)
{
    if (bottomLeftOwner != Qt::BottomEdge && bottomLeftOwner != Qt::LeftEdge) {
        qWarning() << "Invalid value for bottomLeftOwner: " << bottomLeftOwner;
        return;
    }

    if (m_bottomLeftOwner == bottomLeftOwner)
        return;

    m_bottomLeftOwner = bottomLeftOwner;
    if (isComponentComplete())
        reorderDockAreas();
    emit bottomLeftOwnerChanged(m_bottomLeftOwner);
}

void DockContainer::setBottomRightOwner(Qt::Edge bottomRightOwner)
{
    if (bottomRightOwner != Qt::BottomEdge && bottomRightOwner != Qt::RightEdge) {
        qWarning() << "Invalid value for bottomRightOwner: " << bottomRightOwner;
        return;
    }

    if (m_bottomRightOwner == bottomRightOwner)
        return;

    m_bottomRightOwner = bottomRightOwner;
    if (isComponentComplete())
        reorderDockAreas();
    emit bottomRightOwnerChanged(m_bottomRightOwner);
}

void DockContainer::setEnableStateStoring(bool enableStateStoring)
{
    if (m_enableStateStoring == enableStateStoring)
        return;

    m_enableStateStoring = enableStateStoring;
    emit enableStateStoringChanged(m_enableStateStoring);
}

void DockContainer::dockWidget_beginMove()
{
    auto dw = qobject_cast<DockWidget *>(sender());

    for (auto d : _dockWidgets)
        d->setZ(d->z() - 1);
    dw->setZ(Z_WIDGET_FLOAT);

    if (dw->dockArea()) {
        //        dw->beginDetach();
        dw->setArea(Dock::Float);
        dw->dockArea()->removeDockWidget(dw);
        dw->restoreSize();
    }

    _dockMoveGuide->setAllowedAreas(dw->allowedAreas());
    _dockMoveGuide->begin(mapToGlobal(QPoint(0, 0)),
                size());
//    _dockMoveGuide->setSize(size());
//    _dockMoveGuide->setVisible(true);
}

void DockContainer::dockWidget_moving(const QPointF &pt)
{
    _dockMoveGuide->setMousePos(pt);
}

void DockContainer::dockWidget_moved()
{
    _dockMoveGuide->end();

    auto dw = qobject_cast<DockWidget *>(sender());
    if (!dw)
        return;

    switch (_dockMoveGuide->area()) {
    case Dock::Left:
    case Dock::Right:
    case Dock::Top:
    case Dock::Bottom:
    case Dock::Center:
        if (dw->dockArea() != _dockAreas[_dockMoveGuide->area()]) {
            _dockAreas[_dockMoveGuide->area()]->addDockWidget(dw);
        }
        dw->setZ(Z_WIDGET);
        reorderDockAreas();
        break;
    case Dock::Float:
    case Dock::Detached:
        dw->setArea(_dockMoveGuide->area());
        break;

    default:
        break;
    }
}

void DockContainer::dockWidget_visibleChanged()
{
    auto dw = qobject_cast<DockWidget *>(sender());
    if (!dw)
        return;

    if (dw->area() == Dock::Float)
        return;

    if (dw->dockArea()) {
        auto dt = dw->dockArea()->displayType();
        if (dt == Dock::TabbedView)
            return;
    }

    if (dw->isVisible()) {
        _dockAreas[dw->area()]->addDockWidget(dw);
    } else {
        if (dw->dockArea()) {
            dw->dockArea()->removeDockWidget(dw);
        }
    }
}

void DockContainer::geometryChanged(const QRectF &newGeometry,
                               const QRectF &oldGeometry)
{
    if (isComponentComplete())
        reorderDockAreas();
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

int DockContainer::panelSize(Dock::Area area) const
{
    return _dockAreas[area]->isOpen()
               ? _dockAreas[area]->panelSize()
               : 0.;
}

DockArea *DockContainer::createGroup(Dock::Area area, DockArea *item)
{
    if (_dockAreas.contains(area))
        return _dockAreas.value(area);

    if (!item)
        item = new DockArea(this);
    item->setArea(area);
    item->setVisible(true);
    item->setZ(Z_GROUP);
    item->setPanelSize(120);
    item->setDisplayType(Dock::SplitView);

    _dockAreas.insert(area, item);

    return item;
}

QRectF DockContainer::panelRect(Dock::Area area) const
{
    auto d = _dockAreas.value(area);

    if (d->isOpen())
        return QRectF(d->position(), d->size());

    constexpr qreal s{50};

    switch (area) {
    case Dock::Left:
        return QRectF(0, 0, s, height() - 1);

    case Dock::Top:
        return QRectF(0, 0, width() - 1, s);

    case Dock::Right:
        return QRectF(width() - s, 0, s, height() - 1);

    case Dock::Bottom:
        return QRectF(0, height() - s, width() - 1, s);

    default:
        return QRectF();
    }
}

Qt::Edge DockContainer::topLeftOwner() const
{
    return m_topLeftOwner;
}

Qt::Edge DockContainer::topRightOwner() const
{
    return m_topRightOwner;
}

Qt::Edge DockContainer::bottomLeftOwner() const
{
    return m_bottomLeftOwner;
}

Qt::Edge DockContainer::bottomRightOwner() const
{
    return m_bottomRightOwner;
}

bool DockContainer::enableStateStoring() const
{
    return m_enableStateStoring;
}
