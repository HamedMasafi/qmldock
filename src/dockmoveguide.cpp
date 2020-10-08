#include "dockmoveguide.h"

#include <QPainter>

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

    QRectF rc(0, 0, 30, 30);
    bool b;

    painter->setOpacity(.6);

    rc.moveCenter(clipRect().center());
    if (_allowedAreas & Dock::Center) {
        b = drawRect(painter, rc);
        if (b)
            _area = Dock::Center;
    }

    if (_allowedAreas & Dock::Right) {
        rc.moveLeft(width() / 2 + 30);
        b = drawRect(painter, rc);
        if (b) {
            painter->setOpacity(0.2);
            painter->fillRect(width() - 300, 0, 300, height(), Qt::green);
            _area = Dock::Right;
        }
    }

    if (_allowedAreas & Dock::Left) {
        rc.moveRight(width() / 2 - 30);
        b = drawRect(painter, rc);
        if (b) {
            painter->setOpacity(0.2);
            painter->fillRect(0, 0, 300, height(), Qt::green);
            _area = Dock::Left;
        }
    }

    if (_allowedAreas & Dock::Bottom) {
        rc.moveCenter(clipRect().center());
        rc.moveTop(height() / 2 + 30);
        b = drawRect(painter, rc);
        if (b) {
            painter->setOpacity(0.2);
            painter->fillRect(0, height() - 300, width(), 300, Qt::green);
            _area = Dock::Bottom;
        }
    }

    if (_allowedAreas & Dock::Top) {
        rc.moveBottom(height() / 2 - 30);
        b = drawRect(painter, rc);
        if (b) {
            painter->setOpacity(0.2);
            painter->fillRect(0, 0, width(), 300, Qt::green);
            _area = Dock::Top;
        }
    }

    painter->setOpacity(1);
}
