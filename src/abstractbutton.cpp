#include "abstractbutton.h"
#include "style/abstractstyle.h"

AbstractButton::AbstractButton(QQuickItem *parent)
    : QQuickPaintedItem(parent), _status{Dock::Normal}
{
    setSize(QSizeF(16, 16));
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}
Dock::Icon AbstractButton::icon() const
{
    return _icon;
}

void AbstractButton::setIcon(const Dock::Icon &icon)
{
    _icon = icon;
}

void AbstractButton::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Normal) {
        _status = Dock::Hovered;
        update();
    }
}

void AbstractButton::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Hovered) {
        _status = Dock::Normal;
        update();
    }
}

void AbstractButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    _status = Dock::Pressed;
    update();
}

void AbstractButton::mouseReleaseEvent(QMouseEvent *event)
{
    _status = Dock::Normal;
    update();
    if (clipRect().contains(event->pos()))
        emit clicked();
}


