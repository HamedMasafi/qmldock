#include "dockmoveguide.h"

#include <QPainter>
#include "style/abstractstyle.h"

Dock::Area DockMoveGuide::area() const
{
    return _area;
}

Dock::Areas DockMoveGuide::allowedAreas() const
{
    return _allowedAreas;
}

void DockMoveGuide::setAllowedAreas(const Dock::Areas &allowedAreas)
{
    _allowedAreas = allowedAreas;
}

DockMoveGuide::DockMoveGuide(QQuickItem *parent) : QQuickPaintedItem(parent) {}

QPointF DockMoveGuide::mousePos() const
{
    return _mousePos;
}

void DockMoveGuide::setMousePos(const QPointF &mousePos)
{
    _mousePos = mousePos;
    update();
}

bool DockMoveGuide::drawRect(QPainter *painter, const QRectF &rc)
{
    auto b = rc.contains(_mousePos);
    painter->setOpacity(b ? 1 : .2);
    painter->fillRect(rc, Qt::blue);
    return b;
}

void DockMoveGuide::paint(QPainter *painter)
{
    _area = Dock::Detached;

    if (_allowedAreas & Dock::Float && clipRect().contains(_mousePos))
        _area = Dock::Float;

    QRectF rc(0, 0, dockStyle->dropButtonSize(), dockStyle->dropButtonSize());

    painter->setOpacity(.6);

    rc.moveCenter(clipRect().center());
    if (_allowedAreas & Dock::Center) {
        auto b = rc.contains(_mousePos);
        dockStyle->paintDropButton(painter, Dock::Center, rc, b);
        if (b) {
            _area = Dock::Center;
        }
    }

    if (_allowedAreas & Dock::Right) {
        rc.moveLeft(width() / 2 + 30);
        auto b = rc.contains(_mousePos);
        dockStyle->paintDropButton(painter, Dock::Right, rc, b);
        if (b) {
            _area = Dock::Right;
        }
    }

    if (_allowedAreas & Dock::Left) {
        rc.moveRight(width() / 2 - 30);
        auto b = rc.contains(_mousePos);
        dockStyle->paintDropButton(painter, Dock::Left, rc, b);
        if (b) {
            _area = Dock::Left;
        }
    }

    if (_allowedAreas & Dock::Bottom) {
        rc.moveCenter(clipRect().center());
        rc.moveTop(height() / 2 + 30);
        auto b = rc.contains(_mousePos);
        dockStyle->paintDropButton(painter, Dock::Bottom, rc, b);
        if (b) {
            _area = Dock::Bottom;
        }
    }

    if (_allowedAreas & Dock::Top) {
        rc.moveBottom(height() / 2 - 30);
        auto b = rc.contains(_mousePos);
        dockStyle->paintDropButton(painter, Dock::Top, rc, b);
        if (b) {
            _area = Dock::Top;
        }
    }

    painter->setOpacity(1);
}
