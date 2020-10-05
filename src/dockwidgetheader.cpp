#include "dockwidget.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"

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

DockWidgetHeader::DockWidgetHeader(DockWidget *parent)
    : QQuickPaintedItem(parent), parentDock(parent)
    , pinButton(new DockWidgetHeaderButton(this))
    , closeButton(new DockWidgetHeaderButton(this))
      , _enableMove(true)

{
    setHeight(20);
    setAcceptedMouseButtons(Qt::LeftButton);
    setClip(true);
    setAcceptHoverEvents(true);

    pinButton->setIcon("A");
    pinButton->setY(5);
    pinButton->setVisible(false);

    closeButton->setIcon("B");
    closeButton->setY(5);

    connect(closeButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::close);

    connect(pinButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::detach);
}

void DockWidgetHeader::paint(QPainter *painter)
{
//    painter->fillRect(clipRect(), Qt::gray);
    painter->drawText(8, 0, width() - 60, height(), Qt::AlignVCenter, _title);
    painter->drawLine(5, height() - 1, width() - 5, height() - 1);
}

void DockWidgetHeader::mousePressEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    _moveEmitted = false;
    _lastMousePos = event->windowPos();
    _lastParentPos = parentDock->position();
}

void DockWidgetHeader::mouseMoveEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    if (_moveEmitted) {
        emit moving(event->pos() + position());
    } else {
        emit moveStarted();
        _moveEmitted = true;
    }
    parentDock->setPosition(_lastParentPos + (event->windowPos() - _lastMousePos));
}

void DockWidgetHeader::mouseReleaseEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    Q_UNUSED(event)

    if (_moveEmitted)
        emit moveEnded();

    _moveEmitted = false;

    //    QQuickPaintedItem::mouseReleaseEvent(event);
}

void DockWidgetHeader::hoverMoveEvent(QHoverEvent *event)
{

}

void DockWidgetHeader::drawButton(QPainter *painter,
                                  int index,
                                  const QImage &icon,
                                  ButtonStatus status)
{
    auto xx = width() - (0 + (24 * index));
    QRectF rc(xx, 2, 18, 18);
    painter->drawImage(rc, icon);


    switch (status) {
    case Hovered:
        painter->drawEllipse(rc);
        break;
    default:
        break;
    }
}

void DockWidgetHeader::geometryChanged(const QRectF &newGeometry,
                                       const QRectF &oldGeometry)
{
    closeButton->setX(width() - 20);
    pinButton->setX(width() - 40);
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}
