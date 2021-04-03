#include "dockmoveguide.h"

#include <QApplication>
#include <QPainter>
#include <QQuickWindow>
#include <QScreen>
#include "style/abstractstyle.h"
#include "dockcontainer.h"
#include "movedropguide.h"

void DockMoveGuide::insertToAreas(Dock::Area a, const QRectF &rc)
{
    _areas.insert(a, qMakePair<QRectF, QRectF>(rc, {mapToGlobal(rc.topLeft()), rc.size()}));
}

DockMoveGuide::DockMoveGuide(DockContainer *parent) : QQuickPaintedItem(parent)
      ,_parentDockContainer(parent)
{
    _window = new QQuickWindow;
    setParentItem(_window->contentItem());
    _window->hide();
    _window->setFlags(Qt::FramelessWindowHint | Qt::Tool
                      | Qt::WindowStaysOnTopHint);
    setPosition(QPointF(0, 0));

    _dropArea = new MoveDropGuide(parent);
    _dropArea->setParentItem(parent);
    _dropArea->setVisible(false);
    _dropArea->setZ(9999999);
    _dropArea->setSize({200, 200});

    setCursor(Qt::ArrowCursor);
}

void DockMoveGuide::begin(const QPointF &pos, const QSizeF &size)
{
    QRect windowRect = QRect(pos.toPoint(), size.toSize()).intersected(qApp->primaryScreen()->geometry());
    _window->setPosition(windowRect.topLeft());
    _window->resize(windowRect.size());
    _window->show();

    setVisible(true);
    setSize(size);
    QRegion region;
    QRect rc(0,
             0,
             dockStyle->dropButtonSize() + 10,
             dockStyle->dropButtonSize() + 10);

    if (_allowedAreas & Dock::Center) {
        rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));
        region = region.united(rc);
        insertToAreas(Dock::Center, rc);
    }

    rc.setSize(QSize(dockStyle->dropButtonSize(), dockStyle->dropButtonSize()));
    rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));
    if (_allowedAreas & Dock::Left) {
        rc.moveLeft(size.width() / 2 - dockStyle->dropButtonSize()
                    - dockStyle->dropButtonSpace());
        region = region.united(rc);
        insertToAreas(Dock::Left, rc);

        rc.moveLeft(50);
        region = region.united(rc);
        insertToAreas(Dock::Left, rc);
    }

    if (_allowedAreas & Dock::Right) {
        rc.moveRight(size.width() / 2 + dockStyle->dropButtonSize()
                     + dockStyle->dropButtonSpace());
        region = region.united(rc);
        insertToAreas(Dock::Right, rc);

        rc.moveRight(size.width() - 50);
        region = region.united(rc);
        insertToAreas(Dock::Right, rc);
    }

    //back to center
    rc.moveCenter(QPoint(size.width() / 2, size.height() / 2));

    if (_allowedAreas & Dock::Top) {
        rc.moveTop(size.height() / 2 - dockStyle->dropButtonSize()
                   - dockStyle->dropButtonSpace());
        region = region.united(rc);
        insertToAreas(Dock::Top, rc);

        rc.moveTop(50);
        region = region.united(rc);
        insertToAreas(Dock::Top, rc);
    }

    if (_allowedAreas & Dock::Bottom) {
        rc.moveBottom(size.height() / 2 + dockStyle->dropButtonSize()
                      + dockStyle->dropButtonSpace());
        region = region.united(rc);
        insertToAreas(Dock::Bottom, rc);

        rc.moveBottom(size.height() - 50);
        region = region.united(rc);
        insertToAreas(Dock::Bottom, rc);
    }

    _window->setMask(region);
}

void DockMoveGuide::end()
{
    _window->hide();
    _dropArea->setVisible(false);
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

    _area = Dock::Detached;
    auto mouse = QCursor::pos();
    for (auto i = _areas.begin(); i != _areas.end(); ++i) {
        auto contains = i.value().second.contains(mouse);
        if (contains) {
            _area = i.key();
            auto rc = _parentDockContainer->panelRect(_area);
            _dropArea->setPosition(rc.topLeft());
            _dropArea->setSize(rc.size());
            _dropArea->setVisible(true);
        }
    }
    if (_area == Dock::Detached || _area == Dock::Float)
        _dropArea->setVisible(false);


    update();
}

void DockMoveGuide::paint(QPainter *painter)
{

    auto mousePos = QCursor::pos();
    for (auto i = _areas.begin(); i != _areas.end(); ++i) {
        auto contains = i.value().second.contains(mousePos);
        dockStyle->paintDropButton(painter, i.key(), i.value().first, contains);
    }
}
