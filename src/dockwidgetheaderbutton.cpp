#include "dockstyle.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>

QString DockWidgetHeaderButton::icon() const
{
    return _icon;
}

void DockWidgetHeaderButton::setIcon(const QString &icon)
{
    _icon = icon;
}

DockWidgetHeaderButton::DockWidgetHeaderButton(QQuickItem *parent)
    : QQuickPaintedItem(parent), _status{Dock::Normal}
{
    setSize(QSizeF(16, 16));
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DockWidgetHeaderButton::paint(QPainter *painter)
{
    DockStyle::instance()->paintWidgetButton(painter, this, _status);
}

void DockWidgetHeaderButton::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Normal) {
        _status = Dock::Hovered;
        update();
    }
}

void DockWidgetHeaderButton::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Hovered) {
        _status = Dock::Normal;
        update();
    }
}

void DockWidgetHeaderButton::mousePressEvent(QMouseEvent *event)
{
    _status = Dock::Pressed;
    update();
}

void DockWidgetHeaderButton::mouseReleaseEvent(QMouseEvent *event)
{
    _status = Dock::Normal;
    update();
    if (clipRect().contains(event->pos()))
        emit clicked();
}


