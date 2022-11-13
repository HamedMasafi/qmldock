#include "dockgroupresizehandler.h"
#include "style/abstractstyle.h"

#include "dock.h"
#include <QCursor>
#include <qpainter.h>

Qt::Orientation DockAreaResizeHandler::orientation() const {
  return _orientation;
}

DockAreaResizeHandler::DockAreaResizeHandler(Qt::Orientation orientation, QQuickItem *parent)
    : QQuickPaintedItem(parent), _orientation(orientation), _hasHover{false} {
  switch (orientation) {
  case Qt::Horizontal:
    setCursor(Qt::SplitVCursor);
    setHeight(dockStyle->resizeHandleSize());
    break;

  case Qt::Vertical:
    setCursor(Qt::SplitHCursor);
    setWidth(dockStyle->resizeHandleSize());
    break;
  }

  setAcceptedMouseButtons(Qt::LeftButton);
  setAcceptHoverEvents(true);
}

int DockAreaResizeHandler::index() const {
  return _index;
}

void DockAreaResizeHandler::setIndex(int index) {
  _index = index;
}

qreal DockAreaResizeHandler::pos() const {
  if (_orientation)
    return y();

  return x();
}

void DockAreaResizeHandler::setPos(const qreal &pos) {
  _pos = pos;
}

void DockAreaResizeHandler::paint(QPainter *painter) {
  dockStyle->paintResizeHandler(painter, this, _hasHover ? Dock::Hovered : Dock::Normal);
}

void DockAreaResizeHandler::mousePressEvent(QMouseEvent *event) {
  QQuickPaintedItem::mousePressEvent(event);
  event->accept();

  if (_orientation == Qt::Horizontal) {
    _lastMousePos = event->windowPos().y();
    _lastHandlerPos = y();
  } else {
    _lastMousePos = event->windowPos().x();
    _lastHandlerPos = x();
  }
  grabMouse();
}

void DockAreaResizeHandler::mouseMoveEvent(QMouseEvent *event) {
  QQuickPaintedItem::mouseMoveEvent(event);

  bool ok{};
  if (_orientation == Qt::Horizontal) {
    auto newY = _lastHandlerPos + (event->windowPos().y() - _lastMousePos);
    Q_EMIT moving(newY, &ok);

    if (ok) {
      setY(newY);
      setPos(newY);
    }
  } else {
    auto newX = _lastHandlerPos + (event->windowPos().x() - _lastMousePos);
    Q_EMIT moving(newX, &ok);

    if (ok) {
      setX(newX);
      setPos(newX);
    }
  }
}

void DockAreaResizeHandler::mouseReleaseEvent(QMouseEvent *event) {
  Q_EMIT moved();
  ungrabMouse();
  QQuickPaintedItem::mouseReleaseEvent(event);
}

void DockAreaResizeHandler::hoverEnterEvent(QHoverEvent *event) {
  QQuickPaintedItem::hoverEnterEvent(event);
  _hasHover = true;
  update();
}

void DockAreaResizeHandler::hoverLeaveEvent(QHoverEvent *event) {
  QQuickPaintedItem::hoverLeaveEvent(event);
  _hasHover = false;
  update();
}

void DockAreaResizeHandler::drawCircle(QPainter *painter, const QPointF &center, bool hover) {
  Q_UNUSED(hover)
  painter->drawEllipse(center, 2, 2);
}
