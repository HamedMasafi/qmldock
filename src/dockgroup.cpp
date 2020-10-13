#include "dockgroup.h"

#include "debugrect.h"
#include "dockgroup_p.h"
#include "dockgroupresizehandler.h"
#include "dockstyle.h"
#include "docktabbar.h"
#include "dockwidget.h"

#include <QCursor>
#include <QDebug>
#include <QPainter>

DockGroupPrivate::DockGroupPrivate(DockGroup *parent)
    : q_ptr(parent), mousepRessed{false},
      area(Dock::Float), enableResizing{true}
      , tabBar{nullptr}, displayType{Dock::SplitView}
      , minimumSize(80), maximumSize(400)
      , tabPosition{Qt::TopEdge}
{
}

DockGroupResizeHandler *DockGroupPrivate::createHandlers()
{
    Q_Q(DockGroup);

    DockGroupResizeHandler *h{nullptr};
    switch (area) {
    case Dock::Left:
    case Dock::Right:
        h = new DockGroupResizeHandler(Qt::Horizontal, q);
        h->setX(0);
        h->setWidth(q->width());
        break;

    case Dock::Top:
    case Dock::Bottom:
        h = new DockGroupResizeHandler(Qt::Vertical, q);
        h->setY(0);
        h->setHeight(q->height());
        break;

    case Dock::Float:
    case Dock::Center:
        return nullptr;
    }
    if (!h)
        return nullptr;

    h->setIndex(handlers.count());
    QObject::connect(h,
                     &DockGroupResizeHandler::moving,
                     q,
                     &DockGroup::handler_moving);

    QObject::connect(h,
                     &DockGroupResizeHandler::moved,
                     q,
                     &DockGroup::handler_moved);
    return h;
}

bool DockGroupPrivate::isHorizontal() const
{
    return area == Dock::Top || area == Dock::Bottom;
}

bool DockGroupPrivate::isVertical() const
{
    return area == Dock::Right || area == Dock::Left;
}

bool DockGroupPrivate::acceptResizeEvent(const QPointF &point)
{
    Q_Q(DockGroup);

    if (!enableResizing)
        return false;

    switch (area) {
    case Dock::Right:
        return point.x() < DockStyle::instance()->resizeHandleSize();
        break;

    case Dock::Left:
        return point.x() > q->width() - DockStyle::instance()->resizeHandleSize();
        break;

    case Dock::Bottom:
        return point.y() < DockStyle::instance()->resizeHandleSize();

    case Dock::Top:
        return point.y() > q->height() - DockStyle::instance()->resizeHandleSize();

    default:
        return false;
    }
}

void DockGroupPrivate::fitItem(QQuickItem *item)
{
    Q_Q(DockGroup);

    switch (area) {
    case Dock::Right:
        item->setX(q->x() + (enableResizing ? DockStyle::instance()->resizeHandleSize() : 1.));
        item->setWidth(q->width() - 2
                       - (enableResizing ? DockStyle::instance()->resizeHandleSize() : 0.));
        break;

    case Dock::Left:
        item->setX(q->x() + 1);
        item->setWidth(q->width() - 2
                       - (enableResizing ? DockStyle::instance()->resizeHandleSize() : 0.) - 2);
        break;

    case Dock::Top:
        item->setY(q->y() + 1);
        item->setHeight(q->height() - 2
                        - (enableResizing ? DockStyle::instance()->resizeHandleSize() : 0.));
        break;
    case Dock::Bottom:
        item->setY(q->y() + (enableResizing ? DockStyle::instance()->resizeHandleSize() : 1.));
        item->setHeight(q->height() - 2
                        - (enableResizing ? DockStyle::instance()->resizeHandleSize() : 0.));
        break;

    case Dock::Center:
        item->setPosition(usableArea.topLeft());// QPointF(q->x() + DockStyle::instance()->tabMargin(),
//                                  q->y() + DockStyle::instance()->tabMargin()
//                                      + DockStyle::instance()->tabBarHeight()));
        item->setSize(usableArea.size());
//            QSizeF(q->width() - 2 * +DockStyle::instance()->tabMargin(),
//                   q->height() - (2 * +DockStyle::instance()->tabMargin())
//                       - DockStyle::instance()->tabBarHeight()));
        break;

    case Dock::Float:
        break;
    }
}

void DockGroupPrivate::reorderItems()
{
    Q_Q(DockGroup);
    int ss;

    //    for (auto h : _handlers) {
    //        _dockWidgets.at(h->index())->setY(y() + ss);
    //        _dockWidgets.at(h->index())->setHeight(h->y() - ss);
    //        ss += h->y() + h->height();
    //    }
    //    _dockWidgets.last()->setY(y() + ss);
    //    _dockWidgets.last()->setHeight(height() - ss);

    qreal freeSize;

    if (isVertical()) {
        ss = q->y();
        freeSize = (q->height()
                    - (DockStyle::instance()->resizeHandleSize() * (dockWidgets.count() - 1)));
    }

    if (isHorizontal()) {
        ss = q->x();
        freeSize = (q->width()
                    - (DockStyle::instance()->resizeHandleSize() * (dockWidgets.count() - 1)));
    }
    QList<qreal> sl;
    for (int i = 0; i < dockWidgets.count(); i++) {
        auto dw = dockWidgets.at(i);

        switch (displayType) {
        case Dock::SplitView:
            if (isVertical()) {
                dw->setHeight(itemSizes.at(i) * freeSize);
                dw->setY(ss);
                dw->setX(q->x() + usableArea.x());
                dw->setWidth(usableArea.width());
                ss += dw->height() + DockStyle::instance()->resizeHandleSize();
                sl.append(dw->height());
            }
            if (isHorizontal()) {
                dw->setWidth(itemSizes.at(i) * freeSize);
                dw->setX(ss);
                dw->setY(q->y() + usableArea.y());
                dw->setHeight(usableArea.height());
                ss += dw->width() + DockStyle::instance()->resizeHandleSize();
                sl.append(dw->width());
            }
            if (i < dockWidgets.count() - 1) {
                if (isVertical())
                    handlers.at(i)->setY(ss - q->y() - DockStyle::instance()->resizeHandleSize());

                if (isHorizontal())
                    handlers.at(i)->setX(ss - q->x() - DockStyle::instance()->resizeHandleSize());
            }
            break;;

        case Dock::TabbedView:
        case Dock::StackedView:
            dw->setPosition(q->position() + usableArea.topLeft());
            dw->setSize(usableArea.size());
            break;

        case Dock::Hidden:
            break;
        }

    }
}

void DockGroupPrivate::reorderHandles()
{
    Q_Q(DockGroup);

    int index{0};
    for (auto h : handlers) {
        h->setIndex(index++);
        if (isVertical()) {
            h->setX(0);
            h->setWidth(q->width());
        }
        if (isHorizontal()) {
            h->setY(0);
            h->setHeight(q->height());
        }
    }
}

void DockGroupPrivate::normalizeItemSizes()
{
    qreal sum{0};
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    itemSizes.resize(dockWidgets.count());
#else
    {
        auto d = dockWidgets.count() - itemSizes.count();
        if (d > 0)
            for (auto i = 0; i < d; i++)
                itemSizes.append(0);

        if (d < 0)
            for (auto i = 0; i < -d; i++)
                itemSizes.removeLast();
    }
#endif

    for (auto i = 0; i != itemSizes.count(); ++i) {
        if (qFuzzyCompare(0, itemSizes.at(i)))
            itemSizes[i] = 1. / itemSizes.count();
        sum += itemSizes.at(i);
    }

    for (auto i = 0; i != itemSizes.count(); ++i) {
        if (i == itemSizes.count())
            itemSizes[i] = 1 - sum;
        else
            itemSizes[i] = itemSizes.at(i) / sum;
    }
}

QRectF DockGroupPrivate::updateUsableArea()
{
    Q_Q(DockGroup);
    usableArea = QRectF(1, 1, q->width() - 2, q->height() - 2);

    if (enableResizing)
        switch (area) {
        case Dock::Right:
            usableArea.setLeft(DockStyle::instance()->resizeHandleSize() + 1);
            break;

        case Dock::Left:
            usableArea.setRight(usableArea.right()
                                - DockStyle::instance()->resizeHandleSize());
            break;

        case Dock::Top:
            usableArea.setBottom(usableArea.bottom()
                                - DockStyle::instance()->resizeHandleSize());
            break;
        case Dock::Bottom:
            usableArea.setTop(DockStyle::instance()->resizeHandleSize() + 1);
            break;

        case Dock::Center:
        case Dock::Float:
            break;
        }
    if (tabBar && displayType == Dock::TabbedView) {
        switch (tabPosition) {
        case Qt::TopEdge:
            usableArea.setTop(usableArea.top() + DockStyle::instance()->tabBarHeight());
            break;
        case Qt::RightEdge:
            usableArea.setRight(usableArea.right() - DockStyle::instance()->tabBarHeight());
            break;
        case Qt::LeftEdge:
            usableArea.setLeft(usableArea.left() + DockStyle::instance()->tabBarHeight());
            break;
        case Qt::BottomEdge:
            usableArea.setBottom(usableArea.bottom() + DockStyle::instance()->tabBarHeight());
            break;
        }
    }
    return usableArea;
}

DockGroup::DockGroup(QQuickItem *parent)
    : QQuickPaintedItem(parent), d_ptr(new DockGroupPrivate(this))
{
    Q_D(DockGroup);
    d->area = Dock::Float;
    setClip(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    d->tabBar = new DockTabBar(this);
    d->tabBar->setVisible(false);
    d->tabBar->setZ(10000);
    d->tabBar->setTransformOrigin(QQuickItem::TopLeft);
    connect(d->tabBar,
            &DockTabBar::currentIndexChanged,
            this,
            &DockGroup::tabBar_currentIndexChanged);
}

void DockGroup::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(DockGroup);

    if (d->mousepRessed)
        return;

    switch (d->area) {
    case Dock::Right:
    case Dock::Left:
        setCursor(d->acceptResizeEvent(event->pos()) ? Qt::SizeHorCursor
                                                     : Qt::ArrowCursor);
        break;

    case Dock::Bottom:
    case Dock::Top:
        setCursor(d->acceptResizeEvent(event->pos()) ? Qt::SizeVerCursor
                                                     : Qt::ArrowCursor);
        break;

    case Dock::Float:
    case Dock::Center:
        break;
    }
}

bool DockGroup::childMouseEventFilter(QQuickItem *, QEvent *e)
{
    Q_D(DockGroup);

    static QPointF _lastMousePos;
    static QPointF _lastChildPos;

    auto me = static_cast<QMouseEvent *>(e);
    switch (e->type()) {
    case QEvent::MouseButtonPress:
        _lastMousePos = me->windowPos();
        _lastChildPos = position();
        e->accept();
        break;

    case QEvent::MouseMove: {
        auto pt = _lastChildPos + (me->windowPos() - _lastMousePos);
        switch (d->area) {
        case Dock::Right:
        case Dock::Left:
            setWidth(pt.x());
            break;

        case Dock::Bottom:
        case Dock::Top:
            break;

        case Dock::Float:
        case Dock::Center:
            break;
        }

        break;
    }

    default:
        break;
    }

    return false;
}

void DockGroup::mousePressEvent(QMouseEvent *event)
{
    Q_D(DockGroup);

    d->mousepRessed = true;
    switch (d->area) {
    case Dock::Right:
    case Dock::Left:
        d->lastMousePos = event->windowPos().x();
        d->lastGroupSize = width();
        setKeepMouseGrab(true);
        break;

    case Dock::Bottom:
    case Dock::Top:
        d->lastMousePos = event->windowPos().y();
        d->lastGroupSize = height();
        setKeepMouseGrab(true);
        break;

    case Dock::Float:
    case Dock::Center:
        break;
    }
}

void DockGroup::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(DockGroup);

    switch (d->area) {
    case Dock::Left:
        setPanelSize(d->lastGroupSize
                     + (event->windowPos().x() - d->lastMousePos));
        break;

    case Dock::Right:
        setPanelSize(d->lastGroupSize
                     + (d->lastMousePos - event->windowPos().x()));
        break;

    case Dock::Top:
        setPanelSize(d->lastGroupSize
                     + (event->windowPos().y() - d->lastMousePos));
        break;

    case Dock::Bottom:
        setPanelSize(d->lastGroupSize
                     + (d->lastMousePos - event->windowPos().y()));
        break;

    default:
        break;
    }
}

void DockGroup::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    Q_D(DockGroup);
    d->mousepRessed = false;
    setKeepMouseGrab(false);
}

void DockGroup::geometryChanged(const QRectF &newGeometry,
                                const QRectF &oldGeometry)
{
    Q_D(DockGroup);

    if (!d->dockWidgets.count())
        return;

    d->updateUsableArea();

    if (d->tabBar && d->displayType == Dock::TabbedView) {
        switch (d->tabPosition) {
        case Qt::TopEdge:
            d->tabBar->setWidth(width());
            d->tabBar->setHeight(DockStyle::instance()->tabBarHeight());
            break;
        case Qt::LeftEdge:
            d->tabBar->setX(0);
            d->tabBar->setY(height());
            d->tabBar->setWidth(height());
            d->tabBar->setHeight(DockStyle::instance()->tabBarHeight());
            break;
        case Qt::RightEdge:
            d->tabBar->setX(width());
            d->tabBar->setWidth(height());
            d->tabBar->setHeight(dockStyle->tabBarHeight());
            break;
        case Qt::BottomEdge:
            d->tabBar->setWidth(width());
            d->tabBar->setHeight(DockStyle::instance()->tabBarHeight());
            d->tabBar->setY(height() - d->tabBar->height());
            break;
        }
    }
    for (auto dw : d->dockWidgets)
        d->fitItem(dw);

    if (d->displayType == Dock::SplitView)
        d->reorderHandles();
    d->reorderItems();
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void DockGroup::updatePolish()
{
    geometryChanged(QRectF(), QRectF());
    QQuickPaintedItem::updatePolish();
}

void DockGroup::tabBar_currentIndexChanged(int index)
{
    Q_D(DockGroup);
    if (d->displayType != Dock::TabbedView && d->displayType != Dock::StackedView)
        return;

    for (int i = 0; i < d->dockWidgets.count(); ++i) {
        d->dockWidgets.at(i)->setVisible(i == index);
    }
}

void DockGroup::handler_moving(qreal pos, bool *ok)
{
    Q_D(DockGroup);

    auto handler = qobject_cast<DockGroupResizeHandler *>(sender());
    if (!handler)
        return;

    auto ps = handler->index() ? pos - d->handlers.at(handler->index() - 1)->pos()
                               - DockStyle::instance()->resizeHandleSize()
                         : pos;

    auto ns = handler->index() == d->handlers.count() - 1
                  ? (d->isVertical() ? height() : width()) - pos
                        - DockStyle::instance()->resizeHandleSize()
                  : d->handlers.at(handler->index() + 1)->pos() - pos
                        - DockStyle::instance()->resizeHandleSize();

    auto prevDockWidget = d->dockWidgets.at(handler->index());
    auto nextDockWidget = d->dockWidgets.at(handler->index() + 1);

    if (ns > 30 && ps > 30) {
        if (d->isVertical()) {
            nextDockWidget->setY(pos + DockStyle::instance()->resizeHandleSize());
            nextDockWidget->setHeight(ns);
            prevDockWidget->setHeight(ps);
        }
        if (d->isHorizontal()) {
            nextDockWidget->setX(pos + DockStyle::instance()->resizeHandleSize());
            nextDockWidget->setWidth(ns);
            prevDockWidget->setWidth(ps);
        }
        *ok = true;
    }
}

void DockGroup::handler_moved()
{
    Q_D(DockGroup);
    qreal freeSize;
    qreal totalSpace{0};

    if (d->isVertical()) {
        foreach (auto dw, d->dockWidgets)
            totalSpace += dw->height();
        freeSize = (height()
                    - (DockStyle::instance()->resizeHandleSize() * (d->dockWidgets.count() - 1)));
    }

    if (d->isHorizontal()) {
        foreach (auto dw, d->dockWidgets)
            totalSpace += dw->width();
        freeSize = (width()
                    - (DockStyle::instance()->resizeHandleSize() * (d->dockWidgets.count() - 1)));
    }

    int index{0};
    foreach (auto dw, d->dockWidgets) {
        if (d->isVertical())
            d->itemSizes[index++] = (dw->height() / totalSpace);
        //*freeSize;
        if (d->isHorizontal())
            d->itemSizes[index++] = (dw->width() / totalSpace);
        //*freeSize;
    }
    d->reorderItems();
}

bool DockGroup::isOpen() const
{
    Q_D(const DockGroup);
    return d->dockWidgets.count();
    //isOpen;
}

qreal DockGroup::panelSize() const
{
    Q_D(const DockGroup);
    return d->panelSize;
}

Dock::Area DockGroup::area() const
{
    Q_D(const DockGroup);
    return d->area;
}

bool DockGroup::enableResizing() const
{
    Q_D(const DockGroup);
    return d->enableResizing;
}

QList<DockWidget *> DockGroup::widgets() const
{
    Q_D(const DockGroup);
    return d->dockWidgets;
}

Dock::DockWidgetDisplayType DockGroup::displayType() const
{
    Q_D(const DockGroup);
    return d->displayType;
}

qreal DockGroup::minimumSize() const
{
    Q_D(const DockGroup);
    return d->minimumSize;
}

qreal DockGroup::maximumSize() const
{
    Q_D(const DockGroup);
    return d->maximumSize;
}

QList<DockWidget *> DockGroup::dockWidgets() const
{
    Q_D(const DockGroup);
    return d->dockWidgets;
}
Qt::Edge DockGroup::tabPosition() const
{
    Q_D(const DockGroup);
    return d->tabPosition;
}

int DockGroup::currentIndex() const
{
    Q_D(const DockGroup);
    return d->tabBar->currentIndex();
}

void DockGroup::addDockWidget(DockWidget *item)
{
    Q_D(DockGroup);
    item->setArea(d->area);
    item->setDockGroup(this);
    //    addItem(item);
    d->dockWidgets.append(item);
    d->normalizeItemSizes();

    if (d->dockWidgets.count() > 1)
        d->handlers.append(d->createHandlers());

    if (d->tabBar) {
        d->tabBar->addTab(item->title());
        d->tabBar->setCurrentIndex(d->dockWidgets.count() - 1);
    }

//    if (isComponentComplete())
//        geometryChanged(QRectF(), QRectF());
    d->fitItem(item);

    if (d->displayType == Dock::SplitView)
        d->reorderHandles();
    d->reorderItems();

    setIsOpen(d->dockWidgets.count());
    emit dockWidgetsChanged(d->dockWidgets);
}

void DockGroup::removeDockWidget(DockWidget *item)
{
    Q_D(DockGroup);
    auto index = d->dockWidgets.indexOf(item);
    if (index == -1)
        return;

    d->dockWidgets.removeOne(item);
    item->setDockGroup(nullptr);

    if (d->tabBar) {
        d->tabBar->removeTab(index);
        d->tabBar->setCurrentIndex(d->tabBar->currentIndex());
    }

    if (d->handlers.count()) {
        auto h = d->handlers.takeAt(d->handlers.count() - 1);

        if (h) {
            h->setParentItem(nullptr);
            h->deleteLater();
        }
    }

    if (!d->dockWidgets.count()){
        update();
    }

    setIsOpen(d->dockWidgets.count());
    geometryChanged(QRect(), QRect());
    d->normalizeItemSizes();

    if (d->displayType == Dock::SplitView)
        d->reorderHandles();
    d->reorderItems();

    emit dockWidgetsChanged(d->dockWidgets);
}

void DockGroup::setIsOpen(bool isOpen)
{
    Q_D(DockGroup);
    if (d->isOpen == isOpen)
        return;

    d->isOpen = isOpen;
    emit isOpenChanged(isOpen);
}

void DockGroup::setPanelSize(qreal panelSize)
{
    Q_D(DockGroup);
    if (qFuzzyCompare(d->panelSize, panelSize))
        return;

    d->panelSize = qBound(d->minimumSize, panelSize, d->maximumSize);
    emit panelSizeChanged(panelSize);
}

void DockGroup::setArea(Dock::Area area)
{
    Q_D(DockGroup);
    if (d->area == area)
        return;

    d->area = area;
    emit areaChanged(area);
}

void DockGroup::setEnableResizing(bool enableResizing)
{
    Q_D(DockGroup);
    if (d->enableResizing == enableResizing)
        return;

    d->enableResizing = enableResizing;
    emit enableResizingChanged(enableResizing);
}

void DockGroup::setDisplayType(Dock::DockWidgetDisplayType displayType)
{
    Q_D(DockGroup);

    if (d->displayType == displayType)
        return;

    d->tabBar->setVisible(displayType == Dock::TabbedView);
    d->displayType = displayType;
    emit displayTypeChanged(displayType);
}

void DockGroup::setMinimumSize(qreal minimumSize)
{
    Q_D(DockGroup);
    if (qFuzzyCompare(d->minimumSize, minimumSize))
        return;

    d->minimumSize = minimumSize;
    emit minimumSizeChanged(d->minimumSize);

    if (d->panelSize < minimumSize)
        setPanelSize(minimumSize);
}

void DockGroup::setMaximumSize(qreal maximumSize)
{
    Q_D(DockGroup);
    if (qFuzzyCompare(d->maximumSize, maximumSize))
        return;

    d->maximumSize = maximumSize;
    emit maximumSizeChanged(d->maximumSize);

    if (d->panelSize > maximumSize)
        setPanelSize(maximumSize);
}

void DockGroup::setTabPosition(Qt::Edge tabPosition)
{
    Q_D(DockGroup);

    if (d->tabPosition == tabPosition)
        return;

    switch (tabPosition) {
    case Qt::LeftEdge:
        d->tabBar->setRotation(-90);
        break;

    case Qt::RightEdge:
        d->tabBar->setRotation(90);
        break;
    }
    d->tabPosition = tabPosition;
    d->updateUsableArea();
    d->tabBar->setEdge(tabPosition);
    emit tabPositionChanged(d->tabPosition);
}
void DockGroup::setCurrentIndex(int currentIndex)
{
    Q_D(DockGroup);
    if (d->tabBar->currentIndex() == currentIndex)
        return;

    d->tabBar->setCurrentIndex(currentIndex);
    emit currentIndexChanged(currentIndex);
}

void DockGroup::paint(QPainter *painter)
{
    DockStyle::instance()->paintGroup(painter, this);
}
