#include "dockgroupresizehandler.h"
#include "dockstyle.h"

#include "dock.h"
#include <QCursor>
#include <qpainter.h>

Qt::Orientation DockGroupResizeHandler::orientation() const
{
    return _orientation;
}

DockGroupResizeHandler::DockGroupResizeHandler(Qt::Orientation orientation,
                                               QQuickItem *parent)
    : QQuickPaintedItem(parent), _orientation(orientation), _hasHover{false}
{
    switch (orientation) {
    case Qt::Horizontal:
        setCursor(Qt::SizeVerCursor);
        setHeight(DockStyle::instance()->resizeHandleSize());
        break;

    case Qt::Vertical:
        setCursor(Qt::SizeHorCursor);
        setWidth(DockStyle::instance()->resizeHandleSize());
        break;
    }

    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}
int DockGroupResizeHandler::index() const
{
    return _index;
}

void DockGroupResizeHandler::setIndex(int index)
{
    _index = index;
}

qreal DockGroupResizeHandler::pos() const
{
    if (_orientation)
        return y();

    return x();
}

void DockGroupResizeHandler::setPos(const qreal &pos)
{
    _pos = pos;
}


void DockGroupResizeHandler::paint(QPainter *painter)
{
    DockStyle::instance()->paintResizeHandler(painter,
                                              this,
                                              _hasHover ? Dock::Hovered
                                                        : Dock::Normal);
}

void DockGroupResizeHandler::mousePressEvent(QMouseEvent *event)
{
    QQuickPaintedItem::mousePressEvent(event);
    event->accept();

    if (_orientation == Qt::Horizontal) {
        _lastMousePos = event->windowPos().y();
        _lastHandlerPos = y();
    } else {
        _lastMousePos = event->windowPos().x();
        _lastHandlerPos = x();
    }
}

void DockGroupResizeHandler::mouseMoveEvent(QMouseEvent *event)
{
    QQuickPaintedItem::mouseMoveEvent(event);

    bool ok {};
    if (_orientation == Qt::Horizontal) {
        auto newY = _lastHandlerPos + (event->windowPos().y() - _lastMousePos);
        emit moving(newY, &ok);

        if (ok) {
            setY(newY);
            setPos(newY);
        }
    } else {
        auto newX = _lastHandlerPos + (event->windowPos().x() - _lastMousePos);
        emit moving(newX, &ok);

        if (ok) {
            setX(newX);
            setPos(newX);
        }
    }
}

void DockGroupResizeHandler::mouseReleaseEvent(QMouseEvent *event)
{
    emit moved();
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void DockGroupResizeHandler::hoverEnterEvent(QHoverEvent *event)
{
    QQuickPaintedItem::hoverEnterEvent(event);
    _hasHover = true;
    update();
}

void DockGroupResizeHandler::hoverLeaveEvent(QHoverEvent *event)
{
    QQuickPaintedItem::hoverLeaveEvent(event);
    _hasHover = false;
    update();
}

void DockGroupResizeHandler::drawCircle(QPainter *painter, const QPointF &center, bool hover)
{
    painter->drawEllipse(center, 2, 2);
}
