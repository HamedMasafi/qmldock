#include "dockdockableitem.h"
#include "dockdockableitem_p.h"
#include "dockwindow.h"

DockDockableItem::DockDockableItem(QQuickItem *parent) : QQuickItem(parent), d_ptr(new DockDockableItemPrivate(this)) {}

void DockDockableItem::componentComplete() {
  Q_D(DockDockableItem);
  if (children().size() > 1) {
    qWarning() << "DockableItem does not support children more than one";
  } else {
    d->item = childItems().first();
  }
  QQuickItem::componentComplete();
}

DockDockableItem::Status DockDockableItem::status() const {
  Q_D(const DockDockableItem);
  return d->isPopedOut ? PoppedOut : PoppedIn;
}

void DockDockableItem::popOut() {
  Q_D(DockDockableItem);

  if (d->isPopedOut)
    return;

  if (!d->window)
    d->window = new DockWindow{d->item};

  d->window->show();

  Q_EMIT statusChanged(PoppedOut);
}

void DockDockableItem::popIn() {
  Q_D(DockDockableItem);

  if (!d->isPopedOut)
    return;

  d->item->setParentItem(this);
  d->window->close();

  Q_EMIT statusChanged(PoppedIn);
}

void DockDockableItem::setStatus(DockDockableItem::Status status) {
  switch (status) {
  case PoppedIn:
    popIn();
    break;
  case PoppedOut:
    popOut();
    break;
  }
}

void DockDockableItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
  for (auto &ch : childItems()) {
    ch->setPosition({0, 0});
    ch->setSize(newGeometry.size());
  }
  QQuickItem::geometryChange(newGeometry, oldGeometry);
}

DockDockableItemPrivate::DockDockableItemPrivate(DockDockableItem *parent) : q_ptr(parent) {}
