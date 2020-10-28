#include "dockmoveguide.h"

#include <QPainter>
#include <QQuickWindow>
#include "style/abstractstyle.h"

DockMoveGuide::DockMoveGuide(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    _window = new QQuickWindow;
    setParentItem(_window->contentItem());
    _window->hide();
    _window->setFlags(Qt::FramelessWindowHint | Qt::Tool
                      | Qt::WindowStaysOnTopHint);
    setPosition(QPointF(0, 0));
}

void DockMoveGuide::begin(const QPointF &pos, const QSizeF &size)
{
    _window->setPosition(pos.toPoint());
    _window->resize(size.toSize());
    _window->show();

    setVisible(true);
    setSize(size);
    QRegion region;
    QRect rc(0,
             0,
             dockStyle->dropButtonSize() + 10,
             dockStyle->dropButtonSize() + 10);

    { //center
        rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));
        region = region.united(rc);
        _areas.insert(Dock::Center, rc);
    }

    rc.setSize(QSize(dockStyle->dropButtonSize(), dockStyle->dropButtonSize()));
    rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));
    { //left
        rc.moveLeft(size.width() / 2 - dockStyle->dropButtonSize()
                    - dockStyle->dropButtonSpace());
        region = region.united(rc);
        _areas.insert(Dock::Left, rc);

        rc.moveLeft(50);
        region = region.united(rc);
        _areas.insert(Dock::Left, rc);
    }

    { //right
        rc.moveRight(size.width() / 2 + dockStyle->dropButtonSize()
                     + dockStyle->dropButtonSpace());
        region = region.united(rc);
        _areas.insert(Dock::Right, rc);

        rc.moveRight(size.width() - 50);
        region = region.united(rc);
        _areas.insert(Dock::Right, rc);
    }

    //back to center
    rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));

    { //top
        rc.moveTop(size.height() / 2 - dockStyle->dropButtonSize()
                   - dockStyle->dropButtonSpace());
        region = region.united(rc);
        _areas.insert(Dock::Top, rc);

        rc.moveTop(50);
        region = region.united(rc);
        _areas.insert(Dock::Top, rc);
    }

    { //bottom
        rc.moveBottom(size.height() / 2 + dockStyle->dropButtonSize()
                      + dockStyle->dropButtonSpace());
        region = region.united(rc);
        _areas.insert(Dock::Bottom, rc);

        rc.moveBottom(size.height() - 50);
        region = region.united(rc);
        _areas.insert(Dock::Bottom, rc);
    }

    _window->setMask(region);
}

void DockMoveGuide::end()
{
    _window->hide();
}

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


QPointF DockMoveGuide::mousePos() const
{
    return _mousePos;
}

void DockMoveGuide::setMousePos(const QPointF &mousePos)
{
    _mousePos = mousePos;
    update();
}

void DockMoveGuide::paint(QPainter *painter)
{
    _area = Dock::Detached;

    if (_allowedAreas & Dock::Float && clipRect().contains(_mousePos))
        _area = Dock::Float;

    for (auto i = _areas.begin(); i != _areas.end(); ++i) {
        auto contains = i.value().contains(_mousePos);
        dockStyle->paintDropButton(painter, i.key(), i.value(), contains);
        if (contains) {
            _area = i.key();
        }
    }
    return;

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
