#include "dockarea.h"

#include "debugrect.h"
#include "dockarea_p.h"
#include "dockgroupresizehandler.h"
#include "style/abstractstyle.h"
#include "docktabbar.h"
#include "dockwidget.h"

#include <QCursor>
#include <QDebug>
#include <QPainter>

DockAreaPrivate::DockAreaPrivate(DockArea *parent)
    : q_ptr(parent), mousepRessed{false}, currentIndex{-1},
      area(Dock::Float), enableResizing{true}, tabBar{nullptr},
      tabBarItem{nullptr}, displayType{Dock::SplitView}
      , minimumSize(80), maximumSize(400)
      , tabPosition{Qt::TopEdge}
{
}

void DockAreaPrivate::rearrange()
{
    updateUsableArea();
    arrangeTabBar();
    reorderHandles();

    reorderItems();
}

void DockAreaPrivate::arrangeTabBar()
{
    Q_Q(DockArea);

    if (!tabBarItem || displayType != Dock::TabbedView)
        return;

    switch (tabPosition) {
    case Qt::TopEdge:
        tabBarItem->setX(0);
        tabBarItem->setY(0);
        tabBarItem->setWidth(q->width() - dockStyle->resizeHandleSize());
        break;
    case Qt::LeftEdge:
        tabBarItem->setX(0);
        tabBarItem->setY(q->height());
        tabBarItem->setWidth(q->height());
        break;
    case Qt::RightEdge:
        tabBarItem->setY(0);
        tabBarItem->setX(q->width());
        tabBarItem->setWidth(q->height());
        break;
    case Qt::BottomEdge:
        tabBarItem->setX(0);
        tabBarItem->setY(q->height() - tabBarItem->height());
        tabBarItem->setWidth(q->width());
        break;
    }
}

DockAreaResizeHandler *DockAreaPrivate::createHandlers()
{
    Q_Q(DockArea);

    DockAreaResizeHandler *h{nullptr};
    switch (area) {
    case Dock::Left:
    case Dock::Right:
        h = new DockAreaResizeHandler(Qt::Horizontal, q);
        h->setX(0);
        h->setWidth(q->width());
        break;

    case Dock::Top:
    case Dock::Bottom:
        h = new DockAreaResizeHandler(Qt::Vertical, q);
        h->setY(0);
        h->setHeight(q->height());
        break;

    default:
        return nullptr;
    }
    if (!h)
        return nullptr;

    h->setIndex(handlers.count());
    QObject::connect(h,
                     &DockAreaResizeHandler::moving,
                     q,
                     &DockArea::handler_moving);

    QObject::connect(h,
                     &DockAreaResizeHandler::moved,
                     q,
                     &DockArea::handler_moved);
    return h;
}

bool DockAreaPrivate::isHorizontal() const
{
    return area == Dock::Top || area == Dock::Bottom;
}

bool DockAreaPrivate::isVertical() const
{
    return area == Dock::Right || area == Dock::Left;
}

bool DockAreaPrivate::acceptResizeEvent(const QPointF &point)
{
    Q_Q(DockArea);

    if (!enableResizing)
        return false;

    switch (area) {
    case Dock::Right:
        return point.x() < dockStyle->resizeHandleSize();
        break;

    case Dock::Left:
        return point.x() > q->width() - dockStyle->resizeHandleSize();
        break;

    case Dock::Bottom:
        return point.y() < dockStyle->resizeHandleSize();

    case Dock::Top:
        return point.y() > q->height() - dockStyle->resizeHandleSize();

    default:
        return false;
    }
}

void DockAreaPrivate::fitItem(QQuickItem *item)
{
    Q_Q(DockArea);

    switch (area) {
    case Dock::Right:
        item->setX(q->x() + (enableResizing ? dockStyle->resizeHandleSize() : 1.));
        item->setWidth(q->width() - 2
                       - (enableResizing ? dockStyle->resizeHandleSize() : 0.));
        break;

    case Dock::Left:
        item->setX(q->x() + 1);
        item->setWidth(q->width() - 2
                       - (enableResizing ? dockStyle->resizeHandleSize() : 0.) - 2);
        break;

    case Dock::Top:
        item->setY(q->y() + 1);
        item->setHeight(q->height() - 2
                        - (enableResizing ? dockStyle->resizeHandleSize() : 0.));
        break;
    case Dock::Bottom:
        item->setY(q->y() + (enableResizing ? dockStyle->resizeHandleSize() : 1.));
        item->setHeight(q->height() - 2
                        - (enableResizing ? dockStyle->resizeHandleSize() : 0.));
        break;

    case Dock::Center:
        item->setPosition(usableArea.topLeft());// QPointF(q->x() + dockStyle->tabMargin(),
//                                  q->y() + dockStyle->tabMargin()
//                                      + dockStyle->tabBarHeight()));
        item->setSize(usableArea.size());
//            QSizeF(q->width() - 2 * +dockStyle->tabMargin(),
//                   q->height() - (2 * +dockStyle->tabMargin())
//                       - dockStyle->tabBarHeight()));
        break;

    case Dock::Float:
        break;
    }
}

void DockAreaPrivate::reorderItems()
{
    Q_Q(DockArea);
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
                    - (dockStyle->resizeHandleSize() * (dockWidgets.count() - 1)));
    }

    if (isHorizontal()) {
        ss = q->x();
        freeSize = (q->width()
                    - (dockStyle->resizeHandleSize() * (dockWidgets.count() - 1)));
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
                ss += dw->height() + dockStyle->resizeHandleSize();
                sl.append(dw->height());
            }
            if (isHorizontal()) {
                dw->setWidth(itemSizes.at(i) * freeSize);
                dw->setX(ss);
                dw->setY(q->y() + usableArea.y());
                dw->setHeight(usableArea.height());
                ss += dw->width() + dockStyle->resizeHandleSize();
                sl.append(dw->width());
            }
            if (i < dockWidgets.count() - 1) {
                if (isVertical())
                    handlers.at(i)->setY(ss - q->y() - dockStyle->resizeHandleSize());

                if (isHorizontal())
                    handlers.at(i)->setX(ss - q->x() - dockStyle->resizeHandleSize());
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

void DockAreaPrivate::reorderHandles()
{
    Q_Q(DockArea);

    int index{0};
    for (auto h : handlers) {
        if (displayType == Dock::SplitView) {
        h->setIndex(index++);
        if (isVertical()) {
            h->setX(0);
            h->setWidth(q->width());
        }
        if (isHorizontal()) {
            h->setY(0);
            h->setHeight(q->height());
        }
        } else {
            h->setVisible(false);
        }
    }
}

void DockAreaPrivate::normalizeItemSizes()
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

QRectF DockAreaPrivate::updateUsableArea()
{
    Q_Q(DockArea);
    usableArea = QRectF(1, 1, q->width() - 2, q->height() - 2);

    if (enableResizing)
        switch (area) {
        case Dock::Right:
            usableArea.setLeft(dockStyle->resizeHandleSize() + 1);
            break;

        case Dock::Left:
            usableArea.setRight(usableArea.right()
                                - dockStyle->resizeHandleSize());
            break;

        case Dock::Top:
            usableArea.setBottom(usableArea.bottom()
                                - dockStyle->resizeHandleSize());
            break;
        case Dock::Bottom:
            usableArea.setTop(dockStyle->resizeHandleSize() + 1);
            break;

        default:
            break;
        }
    if (tabBarItem && displayType == Dock::TabbedView) {
        switch (tabPosition) {
        case Qt::TopEdge:
            usableArea.setTop(usableArea.top() + tabBarItem->height());
            break;
        case Qt::RightEdge:
            usableArea.setRight(usableArea.right() - tabBarItem->height());
            break;
        case Qt::LeftEdge:
            usableArea.setLeft(usableArea.left() + tabBarItem->height());
            break;
        case Qt::BottomEdge:
            usableArea.setBottom(usableArea.bottom() - tabBarItem->height());
            break;
        }
        auto a = dockStyle->widgetTabPadding();
        usableArea.adjust(a, a, -a, -a);
    }
    return usableArea;
}

DockArea::DockArea(QQuickItem *parent)
    : QQuickPaintedItem(parent), d_ptr(new DockAreaPrivate(this))
{
    Q_D(DockArea);
    d->area = Dock::Float;
    setClip(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DockArea::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(DockArea);

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

bool DockArea::childMouseEventFilter(QQuickItem *, QEvent *e)
{
    Q_D(DockArea);

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

void DockArea::mousePressEvent(QMouseEvent *event)
{
    Q_D(DockArea);

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

void DockArea::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(DockArea);

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

void DockArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    Q_D(DockArea);
    d->mousepRessed = false;
    setKeepMouseGrab(false);
}

void DockArea::geometryChanged(const QRectF &newGeometry,
                                const QRectF &oldGeometry)
{
    Q_D(DockArea);

    if (!d->dockWidgets.count() || !isComponentComplete())
        return;

    d->rearrange();
    d->arrangeTabBar();
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void DockArea::updatePolish()
{
    geometryChanged(QRectF(), QRectF());
    qDebug() << "update polish";
    QQuickPaintedItem::updatePolish();
}

void DockArea::dockWidget_closed()
{
    Q_D(DockArea);
    if (d->displayType == Dock::TabbedView
        || d->displayType == Dock::StackedView) {
        removeDockWidget(qobject_cast<DockWidget *>(sender()));
    }
}

void DockArea::tabBar_currentIndexChanged(int index) {}

void DockArea::tabBar_tabClicked(int index)
{
    Q_D(DockArea);
    if (d->displayType != Dock::TabbedView
        && d->displayType != Dock::StackedView)
        return;

    for (int i = 0; i < d->dockWidgets.count(); ++i) {
        d->dockWidgets.at(i)->setVisible(i == index);
    }
    d->tabBar->setCurrentIndex(index);
}

void DockArea::tabBar_closeButtonClicked(int index)
{
    Q_D(DockArea);
    auto item = d->dockWidgets.at(index);
    if (item)
        item->close();
}

void DockArea::handler_moving(qreal pos, bool *ok)
{
    Q_D(DockArea);

    auto handler = qobject_cast<DockAreaResizeHandler *>(sender());
    if (!handler)
        return;

    auto ps = handler->index() ? pos - d->handlers.at(handler->index() - 1)->pos()
                               - dockStyle->resizeHandleSize()
                         : pos;

    auto ns = handler->index() == d->handlers.count() - 1
                  ? (d->isVertical() ? height() : width()) - pos
                        - dockStyle->resizeHandleSize()
                  : d->handlers.at(handler->index() + 1)->pos() - pos
                        - dockStyle->resizeHandleSize();

    auto prevDockWidget = d->dockWidgets.at(handler->index());
    auto nextDockWidget = d->dockWidgets.at(handler->index() + 1);

    if (ns > 30 && ps > 30) {
        if (d->isVertical()) {
            nextDockWidget->setY(y() + pos + dockStyle->resizeHandleSize());
            nextDockWidget->setHeight(ns);
            prevDockWidget->setHeight(ps);
        }
        if (d->isHorizontal()) {
            nextDockWidget->setX(x() + pos + dockStyle->resizeHandleSize());
            nextDockWidget->setWidth(ns);
            prevDockWidget->setWidth(ps);
        }
        *ok = true;
    }
}

void DockArea::handler_moved()
{
    Q_D(DockArea);
    qreal freeSize;
    qreal totalSpace{0};

    if (d->isVertical()) {
        foreach (auto dw, d->dockWidgets)
            totalSpace += dw->height();
        freeSize = (height()
                    - (dockStyle->resizeHandleSize() * (d->dockWidgets.count() - 1)));
    }

    if (d->isHorizontal()) {
        foreach (auto dw, d->dockWidgets)
            totalSpace += dw->width();
        freeSize = (width()
                    - (dockStyle->resizeHandleSize() * (d->dockWidgets.count() - 1)));
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

bool DockArea::isOpen() const
{
    Q_D(const DockArea);
    return d->dockWidgets.count();
    //isOpen;
}

qreal DockArea::panelSize() const
{
    Q_D(const DockArea);
    return d->panelSize;
}

Dock::Area DockArea::area() const
{
    Q_D(const DockArea);
    return d->area;
}

bool DockArea::enableResizing() const
{
    Q_D(const DockArea);
    return d->enableResizing;
}

QList<DockWidget *> DockArea::widgets() const
{
    Q_D(const DockArea);
    return d->dockWidgets;
}

Dock::DockWidgetDisplayType DockArea::displayType() const
{
    Q_D(const DockArea);
    return d->displayType;
}

qreal DockArea::minimumSize() const
{
    Q_D(const DockArea);
    return d->minimumSize;
}

qreal DockArea::maximumSize() const
{
    Q_D(const DockArea);
    return d->maximumSize;
}

QList<DockWidget *> DockArea::dockWidgets() const
{
    Q_D(const DockArea);
    return d->dockWidgets;
}
Qt::Edge DockArea::tabPosition() const
{
    Q_D(const DockArea);
    return d->tabPosition;
}

void DockArea::componentComplete()
{
    Q_D(DockArea);
    if (!d->tabBarItem) {
        d->tabBar = new DockTabBar(this);
        d->tabBar->setVisible(false);
        d->tabBar->setZ(10000);
        d->tabBar->setTransformOrigin(QQuickItem::TopLeft);
        d->tabBar->setHeight(dockStyle->tabBarSize());
        d->tabBarItem = d->tabBar;
        connect(d->tabBar,
                &DockTabBar::tabClicked,
                this,
                &DockArea::tabBar_tabClicked);
        connect(d->tabBar,
                &DockTabBar::closeButtonClicked,
                this,
                &DockArea::tabBar_closeButtonClicked);
    }

    d->tabBarItem->setVisible(d->displayType == Dock::TabbedView);
    QQuickPaintedItem::componentComplete();
}

QQuickItem *DockArea::tabBar() const
{
    Q_D(const DockArea);
    return d->tabBarItem;
}

int DockArea::currentIndex() const
{
    Q_D(const DockArea);
    return d->currentIndex;
}

void DockArea::addDockWidget(DockWidget *item)
{
    Q_D(DockArea);
    item->setArea(d->area);
    item->setDockArea(this);
    //    addItem(item);
    d->dockWidgets.append(item);
    d->normalizeItemSizes();

    if (d->tabBar)
        d->tabBar->addTab(item->title());

    if (d->dockWidgets.count() > 1) {
        auto h = d->createHandlers();
        if (h)
            d->handlers.append(h);

        if (d->displayType == Dock::TabbedView
            || d->displayType == Dock::StackedView)
            item->setVisible(false);

    } else {
        setCurrentIndex(0);
    }


    if (isComponentComplete())
        d->rearrange();

    connect(item, &DockWidget::closed, this, &DockArea::dockWidget_closed);

    setIsOpen(d->dockWidgets.count());
    emit dockWidgetsChanged(d->dockWidgets);
}

void DockArea::removeDockWidget(DockWidget *item)
{
    Q_D(DockArea);
    auto index = d->dockWidgets.indexOf(item);
    if (index == -1)
        return;

    d->dockWidgets.removeOne(item);
    item->setDockArea(nullptr);

    if (d->tabBar) {
        d->tabBar->removeTab(index);
        //d->tabBar->setCurrentIndex(d->tabBar->currentIndex());
    }
    setCurrentIndex(currentIndex());

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

void DockArea::setIsOpen(bool isOpen)
{
    Q_D(DockArea);
    if (d->isOpen == isOpen)
        return;

    d->isOpen = isOpen;
    emit isOpenChanged(isOpen);
}

void DockArea::setPanelSize(qreal panelSize)
{
    Q_D(DockArea);
    if (qFuzzyCompare(d->panelSize, panelSize))
        return;

    d->panelSize = qBound(d->minimumSize, panelSize, d->maximumSize);
    emit panelSizeChanged(panelSize);
}

void DockArea::setArea(Dock::Area area)
{
    Q_D(DockArea);
    if (d->area == area)
        return;

    d->area = area;
    emit areaChanged(area);
}

void DockArea::setEnableResizing(bool enableResizing)
{
    Q_D(DockArea);
    if (d->enableResizing == enableResizing)
        return;

    d->enableResizing = enableResizing;
    emit enableResizingChanged(enableResizing);
}

void DockArea::setDisplayType(Dock::DockWidgetDisplayType displayType)
{
    Q_D(DockArea);

    if (d->displayType == displayType)
        return;

    if (d->tabBarItem)
        d->tabBarItem->setVisible(displayType == Dock::TabbedView);
    d->displayType = displayType;
    emit displayTypeChanged(displayType);
}

void DockArea::setMinimumSize(qreal minimumSize)
{
    Q_D(DockArea);
    if (qFuzzyCompare(d->minimumSize, minimumSize))
        return;

    d->minimumSize = minimumSize;
    emit minimumSizeChanged(d->minimumSize);

    if (d->panelSize < minimumSize)
        setPanelSize(minimumSize);
}

void DockArea::setMaximumSize(qreal maximumSize)
{
    Q_D(DockArea);
    if (qFuzzyCompare(d->maximumSize, maximumSize))
        return;

    d->maximumSize = maximumSize;
    emit maximumSizeChanged(d->maximumSize);

    if (d->panelSize > maximumSize)
        setPanelSize(maximumSize);
}

void DockArea::setTabPosition(Qt::Edge tabPosition)
{
    Q_D(DockArea);

    if (!isComponentComplete())
        return;

    if (d->tabPosition == tabPosition)
        return;
    switch (tabPosition) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        d->tabBarItem->setRotation(0);
        break;

    case Qt::LeftEdge:
        d->tabBarItem->setRotation(-90);
        break;

    case Qt::RightEdge:
        d->tabBarItem->setRotation(90);
        break;
    }
    d->tabPosition = tabPosition;
//    d->updateUsableArea();

    if (d->tabBar)
        d->tabBar->setEdge(tabPosition);
    geometryChanged(QRectF(), QRectF());
    update();
    emit tabPositionChanged(d->tabPosition);
}
void DockArea::setCurrentIndex(int currentIndex)
{
    Q_D(DockArea);
    auto newIndex = qBound(0, currentIndex, d->dockWidgets.count() - 1);

    if (d->currentIndex == newIndex)
        return;

    d->currentIndex = newIndex;
    for (int i = 0; i < d->dockWidgets.count(); ++i)
        d->dockWidgets.at(i)->setVisible(i == currentIndex);

    if (d->tabBar)
        d->tabBar->setCurrentIndex(d->currentIndex);
    emit currentIndexChanged(d->currentIndex);
}

void DockArea::setTabBar(QQuickItem *tabBar)
{
    Q_D(DockArea);

    if (d->tabBarItem == tabBar)
        return;

    tabBar->setParentItem(this);
    d->tabBarItem = tabBar;
    polish();
    emit tabBarChanged(tabBar);
}

void DockArea::paint(QPainter *painter)
{
    dockStyle->paintDockArea(painter, this);
}
