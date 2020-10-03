#include "dockstyle.h"
#include "docktabbutton.h"

#include <QDebug>
#include <QFontMetrics>
DockTabButton::DockTabButton(QString title, QQuickItem *parent)
    : QQuickPaintedItem(parent), _title(title), _status(Dock::Normal)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setHeight(DockStyle::instance()->tabBarButtonHeight());
}

void DockTabButton::paint(QPainter *painter)
{
    qDebug() << _status;
    DockStyle::instance()->paintTabButton(painter, this, _status);
}

QString DockTabButton::title() const
{
    return _title;
}

void DockTabButton::setTitle(const QString &title)
{
//    setWidth(QFontMetrics(font()).horizontalAdvance(title) + 10);
    _title = title;
    update();
}

void DockTabButton::setChecked(bool checked)
{
    _status = checked ? Dock::Checked : Dock::Normal;
    update();
}


void DockTabButton::mousePressEvent(QMouseEvent *event)
{
    _status = Dock::Pressed;
    update();
}

void DockTabButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (clipRect().contains(event->pos())) {
        _status = Dock::Hovered;
        emit clicked();
    } else {
        _status = Dock::Normal;
    }
    update();
}

void DockTabButton::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Normal) {
        _status = Dock::Hovered;
        update();
    }
}

void DockTabButton::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
    if (_status == Dock::Hovered) {
        _status = Dock::Normal;
        update();
    }
}
