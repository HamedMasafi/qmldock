#include "dockwidget.h"
#include "dockwidget_p.h"
#include "dockwidgetheader.h"
#include "debugrect.h"
#include "dockwidgetbackground.h"
#include "dockwindow.h"
#include "style/abstractstyle.h"
#include "dockarea.h"

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
      , dockArea{nullptr}
      , dockGroup{nullptr}
      , isClosed{false}
      , detachable{false}
      , isDetached{false}
{

}

DockGroup *DockWidget::dockGroup() const
{
    Q_D(const DockWidget);
    return d->dockGroup;
}

void DockWidget::setDockGroup(DockGroup *dockGroup)
{
    Q_D(DockWidget);
    d->dockGroup = dockGroup;
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
    // end the drag before re-parenting
    QMouseEvent endDrag(QEvent::NonClientAreaMouseMove, QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    const bool handledEndDrag = qApp->sendEvent(d->titleBar, &endDrag);
    qDebug()<<"handledEndDrag"<<handledEndDrag;

    // set this attribute to avoid a hide()-event spoiling the drag-and-drop
//        d->dockWindow->setFlag(Qt::WA_WState_Hidden, true);


    setArea(Dock::Detached);
    qApp->processEvents();
    // ... do re-parenting

}

void DockWidget::close()
{
    Q_D(DockWidget);
    if (d->isDetached && d->dockWindow)
        d->dockWindow->setVisible(false);
    else
        setVisible(false);
    d->isClosed = true;
    emit closed();
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
        setParentItem(d->dockArea);
        d->isDetached = false;
        d->dockWindow->deleteLater();
        d->dockWindow->hide();
        d->dockWindow = nullptr;
    }
    d->area = area;
    emit areaChanged(d->area);
}

void DockWidget::setClosable(bool closable)
{
    Q_D(DockWidget);
    if (d->closable == closable)
        return;

    if (d->titleBar)
        d->titleBar->setCloseButtonVisible(closable);
    d->closable = closable;
    emit closableChanged(d->closable);
}

void DockWidget::setResizable(bool resizable)
{
    Q_D(DockWidget);
    if (d->resizable == resizable)
        return;

    d->resizable = resizable;
    emit resizableChanged(d->resizable);
}

void DockWidget::setMovable(bool movable)
{
    Q_D(DockWidget);
    if (d->movable == movable)
        return;

    d->movable = movable;
    if (d->titleBar)
        d->titleBar->setEnableMove(movable);
    emit movableChanged(d->movable);
}

void DockWidget::setShowHeader(bool showHeader)
{
    Q_D(DockWidget);
    if (d->showHeader == showHeader)
        return;

    if (d->titleBar)
        d->titleBar->setVisible(showHeader);
    d->showHeader = showHeader;
    emit showHeaderChanged(d->showHeader);
}

void DockWidget::setDetachable(bool detachable)
{
    Q_D(DockWidget);
    if (d->detachable == detachable)
        return;

    d->detachable = detachable;
    if (d->titleBar)
        d->titleBar->setPinButtonVisible(detachable);
    emit detachableChanged(d->detachable);
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
    emit contentItemChanged(d->contentItem);
}

void DockWidget::setTitle(QString title)
{
    Q_D(DockWidget);
    if (d->title == title)
        return;

    d->title = title;
    if (d->titleBar)
        d->titleBar->setTitle(title);
    emit titleChanged(d->title);
}

void DockWidget::setAllowedAreas(Dock::Areas allowedAreas)
{
    Q_D(DockWidget);
    if (d->allowedAreas == allowedAreas)
        return;

    d->allowedAreas = allowedAreas;
    emit allowedAreasChanged(d->allowedAreas);
}

void DockWidget::setTitleBar(QQuickItem *titleBar)
{
    Q_D(DockWidget);
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
    emit titleBarChanged(d->titleBarItem);
}

void DockWidget::header_moveStarted()
{
    //    if (isDetached)
    //        d->dockWindow->startSystemMove();

    emit beginMove();
}

void DockWidget::header_moving(const QPointF &windowPos, const QPointF &cursorPos)
{
    Q_D(DockWidget);
    if (d->isDetached) {
        emit moving(d->dockArea->mapFromGlobal(cursorPos));
        d->dockWindow->setPosition(windowPos.toPoint());
    } else {
        setPosition(windowPos);
        emit moving(cursorPos);
    }
}

void DockWidget::header_moveEnded()
{
    emit moved();
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
        emit beginMove();
//        detach();
        _moveEmitted = false;
        break;

    case QEvent::MouseMove: {
        if (_moveEmitted) {
            emit moving(me->localPos());
        } else {
            emit beginMove();
            _moveEmitted = true;
        }
        auto pt = _lastChildPos + (me->windowPos() - _lastMousePos);

        setPosition(pt);
        emit moving(pt + me->pos());
        break;
    }

    case QEvent::MouseButtonRelease:
        emit moved();
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

    if (d->isDetached) {
        rc.adjust(10, 10, -10, -10);
    } else {
        rc.adjust(dockStyle->widgetPadding(), dockStyle->widgetPadding(),
                  -dockStyle->widgetPadding(), -dockStyle->widgetPadding());
    }

    d->titleBarItem->setWidth(rc.width());
    d->titleBarItem->setPosition(rc.topLeft());

    if (d->contentItem) {
        d->contentItem->setPosition(
            QPointF(rc.left(),
                    rc.top() + (d->showHeader ? d->titleBarItem->height() : 0)));

        d->contentItem->setWidth(rc.width());
        d->contentItem->setHeight(
            rc.height() - (d->showHeader ? d->titleBarItem->height() : 0));
        d->contentItem->setVisible(false);
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
    if (!d->titleBarItem) {
        d->titleBar = new DockWidgetHeader(this);
        d->titleBarItem = d->titleBar;
        d->titleBar->setPosition(
            QPointF(dockStyle->widgetPadding(), dockStyle->widgetPadding()));
        d->titleBar->setSize(QSizeF(width(), 30));
        d->titleBar->setVisible(true);
        d->titleBar->setZ(999);
        d->titleBar->setTitle(d->title);

        connect(d->titleBar,
                &DockWidgetHeader::moveStarted,
                this,
                &DockWidget::header_moveStarted);
        connect(d->titleBar,
                &DockWidgetHeader::moving,
                this,
                &DockWidget::header_moving);
        connect(d->titleBar,
                &DockWidgetHeader::moveEnded,
                this,
                &DockWidget::header_moveEnded);
    }
    setSize(QSizeF(200, 200));
    QQuickPaintedItem::componentComplete();
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
    int e{0};
    auto b = 10;
    if (event->pos().x() < b) {
        e |= Qt::LeftEdge;
    }
    if (event->pos().x() >= width() - b) {
        e |= Qt::RightEdge;
    }
    if (event->pos().y() < b) {
        e |= Qt::TopEdge;
    }
    if (event->pos().y() >= height() - b) {
        e |= Qt::BottomEdge;
    }

    if (e) {
        if (d->isDetached && d->dockWindow)
            d->dockWindow->startSystemResize((Qt::Edge) e);
    }
    event->accept();
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

