#include "abstractmouseresponsiveitem.h"

Dock::ButtonStatus AbstractMouseResponsiveItem::status() const
{
    return _status;
}

AbstractMouseResponsiveItem::AbstractMouseResponsiveItem(QQuickItem *parent)
: QQuickPaintedItem(parent) , _status{Dock::Normal}
{

}

void AbstractMouseResponsiveItem::mousePressEvent(QMouseEvent *event)
{
    _status = Dock::Pressed;
    update();
}

void AbstractMouseResponsiveItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (clipRect().contains(event->pos()))
        _status = Dock::Hovered;
    else
        _status = Dock::Normal;
    update();
}

void AbstractMouseResponsiveItem::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Normal) {
        _status = Dock::Hovered;
        update();
    }
}

void AbstractMouseResponsiveItem::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Hovered) {
        _status = Dock::Normal;
        update();
    }
}

void AbstractMouseResponsiveItem::paint(QPainter *painter)
{

}
