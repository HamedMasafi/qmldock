#include "dockwidget.h"
#include "dockwidget_p.h"
#include "dockwidgetheader.h"
#include "debugrect.h"
#include "dockwidgetbackground.h"
#include "dockwidgetmovehandler.h"
#include "dockwindow.h"
#include "dockarea.h"
#include "style/abstractstyle.h"
#include "dockcontainer.h"

#include <QDebug>
#include <QPainter>
#include <QQuickWindow>
#include <QApplication>
#include <QScopeGuard>

DockWidgetPrivate::DockWidgetPrivate(DockWidget *parent)
    : q_ptr(parent)
//      , area{Dock::Float}
      , allowedAreas{Dock::AllAreas}
      , originalSize{200, 200}
      , closable{true}
      , resizable{true}
      , movable{true}
      , showHeader{true}
      , contentItem{nullptr}
      , titleBarItem{nullptr}
      , titleBar{nullptr}
      , dockWindow{nullptr}
      , dockContainer{nullptr}
      , dockArea{nullptr}
      , isClosed{false}
      , autoCreateHeader{true}
      , isActive{false}
      , detachable{false}
      , isDetached{false}
{

}

DockArea *DockWidget::dockArea() const
{
    Q_D(const DockWidget);
    return d->dockArea;
}

void DockWidget::setDockArea(DockArea *dockArea)
{
    Q_D(DockWidget);
    d->dockArea = dockArea;
}

Dock::Area DockWidget::area() const
{
    Q_D(const DockWidget);
    return d->area;
}

bool DockWidget::closable() const
{
    Q_D(const DockWidget);
    return d->closable;
}

bool DockWidget::resizable() const
{
    Q_D(const DockWidget);
    return d->resizable;
}

bool DockWidget::movable() const
{
    Q_D(const DockWidget);
    return d->movable;
}

bool DockWidget::showHeader() const
{
    Q_D(const DockWidget);
    return d->showHeader;
}

bool DockWidget::detachable() const
{
    Q_D(const DockWidget);
    return d->detachable;
}

void DockWidget::paint(QPainter *painter)
{
    dockStyle->paintDockWidget(painter, this);
//    QQuickPaintedItem::paint(painter);
}

DockWidget::DockWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent), d_ptr(new DockWidgetPrivate(this))
{
    setClip(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    AbstractStyle::registerThemableItem(this);
}

DockWidget::~DockWidget()
{
    Q_D(DockWidget);
    delete d;
}

void DockWidget::detach()
{
    setArea(Dock::Detached);
}

void DockWidget::beginDetach()
{
    Q_D(DockWidget);
    setArea(Dock::Detached);

    //    d->titleBar->grabMouse();
}

void DockWidget::open()
{
    //    Q_D(DockWidget);
    //    if (d->isDetached && d->dockWindow)
    //        d->dockWindow->setVisible(true);
    //    else
    //        setVisible(false);
    //    d->isClosed = true;
    if (!dockArea())
        return;
    Q_EMIT opened();
}

void DockWidget::close()
{
//    Q_D(DockWidget);
//    if (d->isDetached && d->dockWindow)
//        d->dockWindow->setVisible(false);
//    else
//        setVisible(false);
//    d->isClosed = true;
//    dockArea()->removeDockWidget(this);
    Q_EMIT closed();
}

void DockWidget::restoreSize()
{
    Q_D(DockWidget);
    setSize(d->originalSize);
}

void DockWidget::setArea(Dock::Area area)
{
    Q_D(DockWidget);

    if (d->area == area)
        return;

    if (area == Dock::Detached) {
        auto wpos = mapToGlobal(QPointF(0, 0)).toPoint();
        if (!d->dockWindow)
            d->dockWindow = new DockWindow(this);
        d->dockWindow->setPosition(wpos);
        d->dockWindow->setTitle(title());
        d->dockWindow->resize(size().toSize());
        d->dockWindow->show();
        d->isDetached = true;
    }

    if (d->area == Dock::Detached) {
        setParentItem(d->dockContainer);
        setVisible(true);
        d->isDetached = false;
        d->dockWindow->hide();
//        d->dockWindow->deleteLater();
//        d->dockWindow = nullptr;
    }
    d->area = area;
    Q_EMIT areaChanged(d->area);
}

void DockWidget::setClosable(bool closable)
{
    Q_D(DockWidget);
    if (d->closable == closable)
        return;

    if (d->titleBar)
        d->titleBar->setCloseButtonVisible(closable);
    d->closable = closable;
    Q_EMIT closableChanged(d->closable);
}

void DockWidget::setResizable(bool resizable)
{
    Q_D(DockWidget);
    if (d->resizable == resizable)
        return;

    d->resizable = resizable;
    Q_EMIT resizableChanged(d->resizable);
}

void DockWidget::setMovable(bool movable)
{
    Q_D(DockWidget);
    if (d->movable == movable)
        return;

    d->movable = movable;
    if (d->titleBar)
        d->titleBar->setEnableMove(movable);
    Q_EMIT movableChanged(d->movable);
}

void DockWidget::setShowHeader(bool showHeader)
{
    Q_D(DockWidget);
    if (d->showHeader == showHeader)
        return;

    if (d->titleBar)
        d->titleBar->setVisible(showHeader);
    d->showHeader = showHeader;
    Q_EMIT showHeaderChanged(d->showHeader);
}

void DockWidget::setDetachable(bool detachable)
{
    Q_D(DockWidget);
    if (d->detachable == detachable)
        return;

    d->detachable = detachable;
    if (d->titleBar)
        d->titleBar->setPinButtonVisible(detachable);
    Q_EMIT detachableChanged(d->detachable);
}

void DockWidget::setContentItem(QQuickItem *contentItem)
{
    Q_D(DockWidget);
    if (d->contentItem == contentItem)
        return;

    d->contentItem = contentItem;

    d->contentItem->setParentItem(this);
//    d->contentItem->setPosition(QPointF(
//                                   dockStyle->widgetPadding(),
//                                   (d->showHeader ? d->titleBarItem->height() : 0)
//                                    +dockStyle->widgetPadding()
//                                   ));
    geometryChanged(QRectF(), QRectF());
    Q_EMIT contentItemChanged(d->contentItem);
}

void DockWidget::setTitle(QString title)
{
    Q_D(DockWidget);
    if (d->title == title)
        return;

    d->title = title;
    if (d->titleBar)
        d->titleBar->setTitle(title);
    Q_EMIT titleChanged(d->title);
}

void DockWidget::setAllowedAreas(Dock::Areas allowedAreas)
{
    Q_D(DockWidget);
    if (d->allowedAreas == allowedAreas)
        return;

    d->allowedAreas = allowedAreas;
    Q_EMIT allowedAreasChanged(d->allowedAreas);
}

void DockWidget::setTitleBar(QQuickItem *titleBar)
{
    Q_D(DockWidget);

    d->autoCreateHeader = false;
    if (d->titleBarItem == titleBar)
        return;

    if (d->titleBar) {
        d->titleBar->setParent(nullptr);
        d->titleBar->deleteLater();
        d->titleBar = nullptr;
    }
    titleBar->setZ(1000);
    titleBar->setParentItem(this);
    d->titleBarItem = titleBar;
    Q_EMIT titleBarChanged(d->titleBarItem);
}

void DockWidget::setIsActive(bool isActive)
{
    Q_D(DockWidget);
    if (d->isActive == isActive)
        return;

    d->isActive = isActive;
    update();
    Q_EMIT isActiveChanged(d->isActive);
}

void DockWidget::header_moveStarted()
{
    //    if (isDetached)
    //        d->dockWindow->startSystemMove();

    //    beginDetach();
    Q_EMIT beginMove();
}

void DockWidget::header_moving(const QPointF &windowPos, const QPointF &cursorPos)
{
    Q_D(DockWidget);
    if (d->isDetached) {
        Q_EMIT moving(d->dockContainer->mapFromGlobal(cursorPos));
        d->dockWindow->setPosition(windowPos.toPoint());
    } else {
        setPosition(windowPos);
        Q_EMIT moving(cursorPos);
    }
}

void DockWidget::header_moveEnded()
{
    Q_EMIT moved();
}

bool DockWidget::childMouseEventFilter(QQuickItem *item, QEvent *e)
{
    Q_D(DockWidget);
    static QPointF _lastMousePos;
    static QPointF _lastChildPos;
    static bool _moveEmitted = false;

    if (item != d->titleBar || !d->movable) {
        return false;
    }

    auto me = static_cast<QMouseEvent *>(e);
    if (d->isDetached) {
        if (me->button() == Qt::LeftButton)
            d->dockWindow->startSystemMove();
        return true;
    }

    switch (e->type()) {
    case QEvent::MouseButtonPress:
        _lastMousePos = me->windowPos();
        _lastChildPos = position();
        Q_EMIT beginMove();
//        detach();
        _moveEmitted = false;
        break;

    case QEvent::MouseMove: {
        if (_moveEmitted) {
            Q_EMIT moving(me->localPos());
        } else {
            Q_EMIT beginMove();
            _moveEmitted = true;
        }
        auto pt = _lastChildPos + (me->windowPos() - _lastMousePos);

        setPosition(pt);
        Q_EMIT moving(pt + me->pos());
        break;
    }

    case QEvent::MouseButtonRelease:
        Q_EMIT moved();
        break;;

    default:
        break;
    }

    return true;
}

void DockWidget::itemChange(QQuickItem::ItemChange change,
                            const QQuickItem::ItemChangeData &data)
{
    QQuickItem::itemChange(change, data);

}

void DockWidget::geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry)
{
    Q_D(DockWidget);
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
    if (!isComponentComplete())
        return;

    if (d->area == Dock::Float) {
        d->originalSize = newGeometry.size();
    }


    QRectF rc(QPointF(0, 0), size());

//    if (d->isDetached) {
//        rc.adjust(10, 10, -10, -10);
//    } else {
//    }
    auto p = dockStyle->widgetPadding(this);
    rc.adjust(p, p, -p, -p);

    qreal titlebarHeight{0};

    if (d->titleBarItem) {
        d->titleBarItem->setWidth(rc.width());
        d->titleBarItem->setPosition(rc.topLeft());
        titlebarHeight = d->showHeader ? d->titleBarItem->height() : 0;
    }

    if (d->contentItem) {
        d->contentItem->setPosition(
            QPointF(rc.left(), rc.top() + titlebarHeight));

        d->contentItem->setWidth(rc.width());
        d->contentItem->setHeight(rc.height() - titlebarHeight);
    }
}

bool DockWidget::getIsDetached() const
{
    Q_D(const DockWidget);
    return d->isDetached;
}

Dock::Areas DockWidget::allowedAreas() const
{
    Q_D(const DockWidget);
    return d->allowedAreas;
}

DockWindow *DockWidget::dockWindow() const
{
    Q_D(const DockWidget);
    return d->dockWindow;
}

QQuickItem *DockWidget::titleBar() const
{
    Q_D(const DockWidget);
    return d->titleBarItem;
}

void DockWidget::componentComplete()
{
    Q_D(DockWidget);
    if (!d->titleBarItem && d->autoCreateHeader) {
        d->titleBar = new DockWidgetHeader(this);
        d->titleBarItem = d->titleBar;
        auto p = dockStyle->widgetPadding(this);
        d->titleBar->setPosition(QPointF(p, p));
        d->titleBar->setSize(QSizeF(width(), 30));
        d->titleBar->setVisible(true);
        d->titleBar->setZ(999);
        d->titleBar->setTitle(d->title);
        d->titleBar->setCloseButtonVisible(d->closable);

        auto handler = d->titleBar->moveHandler();
        connect(handler,
                &DockWidgetMoveHandler::moveStarted,
                this,
                &DockWidget::header_moveStarted);
        connect(handler,
                &DockWidgetMoveHandler::moving,
                this,
                &DockWidget::header_moving);
        connect(handler,
                &DockWidgetMoveHandler::moveEnded,
                this,
                &DockWidget::header_moveEnded);
    }
    setSize(QSizeF(200, 200));
    QQuickPaintedItem::componentComplete();
}

bool DockWidget::isActive() const
{
    Q_D(const DockWidget);
    return d->isActive;
}

DockContainer *DockWidget::dockContainer() const
{
    Q_D(const DockWidget);
    return d->dockContainer;
}

void DockWidget::setDockContainer(DockContainer *dockContainer)
{
    Q_D(DockWidget);
    setParentItem(dockContainer);
    d->dockContainer = dockContainer;
}

void DockWidget::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(DockWidget);
    if (d->area != Dock::Float && d->area != Dock::Detached) {
        setCursor(Qt::ArrowCursor);
        return;
    }
    QCursor cursor = Qt::ArrowCursor;
    auto b = 10;
    if (event->pos().x() < b && event->pos().y() < b)
        cursor = Qt::SizeFDiagCursor;
    else if (event->pos().x() >= width() - b && event->pos().y() >= height() - b)
        cursor = Qt::SizeFDiagCursor;
    else if (event->pos().x() >= width() - b && event->pos().y() < b)
        cursor = Qt::SizeBDiagCursor;
    else if (event->pos().x() < b && event->pos().y() >= height() - b)
        cursor = Qt::SizeBDiagCursor;
    else if (event->pos().x() < b || event->pos().x() >= width() - b)
        cursor = Qt::SizeHorCursor;
    else if (event->pos().y() < b || event->pos().y() >= height() - b)
        cursor = Qt::SizeVerCursor;

    setCursor(cursor);
}

void DockWidget::mousePressEvent(QMouseEvent *event)
{
    Q_D(DockWidget);
    d->resizeEdge = 0;
    auto b = 10;
    if (event->pos().x() < b) {
        d->resizeEdge |= Qt::LeftEdge;
    }
    if (event->pos().x() >= width() - b) {
        d->resizeEdge |= Qt::RightEdge;
    }
    if (event->pos().y() < b) {
        d->resizeEdge |= Qt::TopEdge;
    }
    if (event->pos().y() >= height() - b) {
        d->resizeEdge |= Qt::BottomEdge;
    }

    if (d->resizeEdge) {
        if (d->isDetached && d->dockWindow) {
            d->dockWindow->startSystemResize((Qt::Edge) d->resizeEdge);
            event->ignore();
        } else {
            event->accept();
        }
        d->rectBeforeResize = QRectF(position(), size());
        d->resizeMousePos = event->windowPos();
    } else {
        event->ignore();
    }
}

void DockWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(DockWidget);
    QRectF rc(position(), size());
    if (d->resizeEdge & Qt::LeftEdge)
        rc.setLeft(d->rectBeforeResize.left()
                                    + event->windowPos().x()
                                    - d->resizeMousePos.x());

    if (d->resizeEdge & Qt::TopEdge)
        rc.setTop(d->rectBeforeResize.top()
                                   + event->windowPos().y()
                                   - d->resizeMousePos.y());

    if (d->resizeEdge & Qt::RightEdge)
        rc.setRight(d->rectBeforeResize.right()
                                     + event->windowPos().x()
                                     - d->resizeMousePos.x());

    if (d->resizeEdge & Qt::BottomEdge)
        rc.setBottom(d->rectBeforeResize.bottom()
                                      + event->windowPos().y()
                                      - d->resizeMousePos.y());

    setPosition(rc.topLeft());
    setSize(rc.size());
}

void DockWidget::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

QQuickItem *DockWidget::contentItem() const
{
    Q_D(const DockWidget);
    return d->contentItem;
}

QString DockWidget::title() const
{
    Q_D(const DockWidget);
    return d->title;
}

