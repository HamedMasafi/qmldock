#include "style/abstractstyle.h"
#include "dockwidget.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"
#include "dockwidgetmovehandler.h"
#include "dockwindow.h"

#include <QDebug>
#include <QPainter>

QString DockWidgetHeader::title() const
{
    return _title;
}

void DockWidgetHeader::setTitle(const QString &title)
{
    _title = title;
    update();
}

bool DockWidgetHeader::closeButtonVisible() const
{
    return closeButton->isVisible();
}

void DockWidgetHeader::setCloseButtonVisible(bool closeButtonVisible)
{
    closeButton->setVisible(closeButtonVisible);
}

bool DockWidgetHeader::enableMove() const
{
    return _enableMove;
}

void DockWidgetHeader::setEnableMove(bool enableMove)
{
    _enableMove = enableMove;
}

bool DockWidgetHeader::pinButtonVisible() const
{
    return pinButton->isVisible();
}

void DockWidgetHeader::setPinButtonVisible(bool pinButtonVisible)
{
    pinButton->setVisible(pinButtonVisible);
}

DockWidget *DockWidgetHeader::parentDock() const
{
    return _parentDock;
}

DockWidgetMoveHandler *DockWidgetHeader::moveHandler() const
{
    return _moveHandler;
}

DockWidgetHeader::DockWidgetHeader(DockWidget *parent)
    : QQuickPaintedItem(parent),
      _parentDock(parent)
      , pinButton{new DockWidgetHeaderButton(this)}
      , closeButton{new DockWidgetHeaderButton(this)}
      , _moveHandler{new DockWidgetMoveHandler(this)}
      , _enableMove(true)

{
    setHeight(20);
//    setAcceptedMouseButtons(Qt::LeftButton);
    setClip(true);
//    setAcceptHoverEvents(true);

    pinButton->setIcon(Dock::PinIcon);
    pinButton->setY(5);
    pinButton->setVisible(false);

    closeButton->setIcon(Dock::CloseIcon);
    closeButton->setY(5);

    connect(closeButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::close);

    connect(pinButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::detach);

    _moveHandler->setParentItem(this);
    _moveHandler->setDockWidget(parent);

    AbstractStyle::registerThemableItem(this);
}

void DockWidgetHeader::paint(QPainter *painter)
{
    dockStyle->paintDockWidgetHeader(painter, this);
}

//void DockWidgetHeader::mousePressEvent(QMouseEvent *event)
//{
//    if (!_enableMove) {
//        event->ignore();
//        return;
//    }
//    _moveEmitted = false;
//    if (_parentDock->getIsDetached()) {
//        _lastParentPos = _parentDock->dockWindow()->position();
//        _lastMousePos = event->globalPos();
//    } else {
//        _lastMousePos = event->windowPos();
//        _lastParentPos = _parentDock->position();
//    }
//    grabMouse();
//}

//void DockWidgetHeader::mouseMoveEvent(QMouseEvent *event)
//{
//    if (!_enableMove) {
//        event->ignore();
//        return;
//    }
//    if (_moveEmitted) {
//        if (_parentDock->getIsDetached())
//            Q_EMIT moving(_lastParentPos + (event->globalPos() - _lastMousePos),
//                        event->pos() + _parentDock->dockWindow()->position());
//        else
//            Q_EMIT moving(_lastParentPos + (event->windowPos() - _lastMousePos),
//                    event->pos() + _parentDock->position());
//    } else {
//        Q_EMIT moveStarted();
//        _moveEmitted = true;
//    }
////    parentDock->setPosition(_lastParentPos + (event->windowPos() - _lastMousePos));
//}

//void DockWidgetHeader::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (!_enableMove) {
//        event->ignore();
//        return;
//    }
//    Q_UNUSED(event)

//    if (_moveEmitted)
//        Q_EMIT moveEnded();

//    _moveEmitted = false;
//    ungrabMouse();
//}

void DockWidgetHeader::geometryChanged(const QRectF &newGeometry,
                                       const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);

    if (!isComponentComplete())
        return;

    closeButton->setX(width() - 20);
    pinButton->setX(width() - 40);
    _moveHandler->setHeight(height());
    _moveHandler->setWidth(width() - 40);
}
