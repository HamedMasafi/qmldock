#include "style/abstractstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "dockwidgetheaderbutton.h"

#include <QDebug>
#include <QCursor>
#include <QFontMetrics>

DockTabBar *DockTabButton::parentTabBar() const {
  return _parentTabBar;
}

qreal DockTabButton::fitSize() const {
  if (_showCloseButton)
    return _fitSize + _closeButton->width() + 6;
  else
    return _fitSize;
}

void DockTabButton::setFitSize(const qreal &fitSize) {
  _fitSize = fitSize;
}

bool DockTabButton::showCloseButton() const {
  return _showCloseButton;
}

void DockTabButton::setShowCloseButton(bool showCloseButton) {
  _closeButton->setVisible(showCloseButton);
  _showCloseButton = showCloseButton;
}

DockTabButton::DockTabButton(QString title, DockTabBar *parent)
    : QQuickPaintedItem(parent), _parentTabBar{parent}, _title(title), _status(Dock::Normal), _showCloseButton{true} {
  setAcceptedMouseButtons(Qt::LeftButton);
  setAcceptHoverEvents(true);
  setCursor(Qt::ArrowCursor);

  _closeButton = new DockWidgetHeaderButton(this);
  _closeButton->setParentItem(this);
  _closeButton->setIcon(Dock::CloseIcon);
  _closeButton->setY(6);
  connect(_closeButton, &DockWidgetHeaderButton::clicked, this, &DockTabButton::closeButtonClicked);

  AbstractStyle::registerThemableItem(this);
}

void DockTabButton::paint(QPainter *painter) {
  dockStyle->paintTabButton(painter, this, _status);
}

QString DockTabButton::title() const {
  return _title;
}

void DockTabButton::setTitle(const QString &title) {
  //    setWidth(QFontMetrics(font()).horizontalAdvance(title) + 10);
  _title = title;
  update();
}

void DockTabButton::setChecked(bool checked) {
  _status = checked ? Dock::Checked : Dock::Normal;
  update();
}

void DockTabButton::mousePressEvent(QMouseEvent *event) {
  Q_UNUSED(event)
  if (_status == Dock::Checked)
    return;

  _status = Dock::Pressed;
  update();
}

void DockTabButton::mouseReleaseEvent(QMouseEvent *event) {
  if (_status == Dock::Checked)
    return;

  if (clipRect().contains(event->pos())) {
    _status = Dock::Hovered;
    Q_EMIT clicked();
  } else {
    _status = Dock::Normal;
  }
  update();
}

void DockTabButton::hoverEnterEvent(QHoverEvent *event) {
  Q_UNUSED(event)
  if (_status == Dock::Checked)
    return;

  if (_status == Dock::Normal) {
    _status = Dock::Hovered;
    update();
  }
}

void DockTabButton::hoverLeaveEvent(QHoverEvent *event) {
  Q_UNUSED(event)
  if (_status == Dock::Checked)
    return;

  if (_status == Dock::Hovered) {
    _status = Dock::Normal;
    update();
  }
}

void DockTabButton::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
  if (_showCloseButton) {
    _closeButton->setX(width() - 20);
  }
  QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
}
