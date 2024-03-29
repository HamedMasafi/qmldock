#include "dockcontainer.h"
#include "dockcontainer_p.h"
#include "dockmoveguide.h"
#include "dockwidget.h"
#include "dockarea.h"
#include "dockgroupresizehandler.h"
#include "style/abstractstyle.h"
#include "dockwindow.h"
#include "dockarea.h"
#include "dockwidgetmovehandler.h"
#include "dockwidgetheader.h"

#include <QDebug>
#include <QPainter>
#include <QSettings>
#define Z_GROUP 100
#define Z_WIDGET 200
#define Z_WIDGET_FLOAT 300
#define Z_RESIZER 400
#define Z_GUIDE 500

DockContainerPrivate::DockContainerPrivate(DockContainer *parent)
    : q_ptr(parent), topLeftOwner{Qt::LeftEdge}, topRightOwner{Qt::RightEdge}, bottomLeftOwner{Qt::LeftEdge},
      bottomRightOwner{Qt::RightEdge}, activeDockWidget{nullptr}, defaultDisplayType{Dock::SplitView} {}

DockContainer::DockContainer(QQuickItem *parent) : QQuickPaintedItem(parent), d_ptr(new DockContainerPrivate(this)) {
  Q_D(DockContainer);
  d->dockMoveGuide = new DockMoveGuide(this);

  setFiltersChildMouseEvents(true);
  AbstractStyle::registerThemableItem(this);
}

DockContainer::~DockContainer() {
  Q_D(DockContainer);
  if (d->enableStateStoring)
    storeSettings();
  delete d;
}

void DockContainer::componentComplete() {
  Q_D(DockContainer);

  auto areas = findChildren<DockArea *>();
  for (auto &a : areas)
    d->dockAreas.insert(a->area(), a);

  if (!d->dockAreas.contains(Dock::Center)) {
    createGroup(Dock::Center);
    d->dockAreas[Dock::Center]->setDisplayType(Dock::TabbedView);
  }
  connect(window(), &QQuickWindow::activeFocusItemChanged, [this, d]() {
    //        auto dockWidget = Dock::findInParents<DockWidget>(
    //            window()->activeFocusItem());
    //        if (dockWidget) {
    //            if (d->activeDockWidget)
    //                d->activeDockWidget->setIsActive(false);
    //            d->activeDockWidget = dockWidget;
    //            d->activeDockWidget->setIsActive(true);
    //        }
  });
  createGroup(Dock::Left);
  createGroup(Dock::Right);
  createGroup(Dock::Top);
  createGroup(Dock::Bottom);

  for (auto &dw : d->initialWidgets)
    addDockWidget(dw);

  for (auto i = d->dockAreas.begin(); i != d->dockAreas.end(); i++) {
    const auto &dg = *i;
    connect(dg, &DockArea::panelSizeChanged, this, &DockContainer::reorderDockAreas);
    connect(dg, &DockArea::isOpenChanged, this, &DockContainer::reorderDockAreas);
  }

  d->dockAreas[Dock::Left]->polish();
  d->dockAreas[Dock::Top]->polish();
  d->dockAreas[Dock::Right]->polish();
  d->dockAreas[Dock::Bottom]->polish();
  d->dockAreas[Dock::Center]->polish();
  //    reorderDockAreas();
  //    geometryChange(QRectF(), QRectF());

  reorderDockAreas();

  QQuickItem::componentComplete();
}

void DockContainer::storeSettings() {
  Q_D(DockContainer);
  QSettings set;
  QMetaEnum e = QMetaEnum::fromType<Dock::Area>();

  for (auto &dw : d->dockWidgets) {
    set.beginGroup(dw->title());
    set.setValue("area", e.valueToKey(dw->area()));
    set.setValue("position", QStringLiteral("%1,%2").arg(dw->x()).arg(dw->y()));
    set.setValue("size", QStringLiteral("%1,%2").arg(dw->width()).arg(dw->height()));
    set.setValue("visible", dw->isVisible());
    set.endGroup();
  }

  set.sync();
}

void DockContainer::restoreSettings() {}

void DockContainer::paint(QPainter *painter) {
  dockStyle->paintDockContainer(painter, this);
}

QList<DockWidget *> DockContainer::dockWidgets() const {
  Q_D(const DockContainer);
  return d->dockWidgets;
}

void DockContainer::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data) {
  Q_D(DockContainer);
  if (change == QQuickItem::ItemChildAddedChange) {
    auto dw = qobject_cast<DockWidget *>(data.item);
    if (dw) {
      if (isComponentComplete()) {
        if (!d->dockWidgets.contains(dw))
          addDockWidget(dw);
      } else {
        d->initialWidgets.append(dw);
      }
    }

    auto dg = qobject_cast<DockArea *>(data.item);
    if (dg) {
      if (!d->dockAreas.contains(dg->area()))
        d->dockAreas.insert(dg->area(), dg);
    }
  }

  if (change == QQuickItem::ItemChildRemovedChange) {
    auto dw = qobject_cast<DockWidget *>(data.item);
    if (dw) {
      //            if (dw->visibility() == DockWidget::Closed)
      //                d->dockWidgets.removeOne(dw);

      if (dw->dockArea())
        dw->dockArea()->removeDockWidget(dw);
    }
  }

  QQuickItem::itemChange(change, data);
}

void DockContainer::addDockWidget(DockWidget *widget) {
  Q_D(DockContainer);

  widget->setZ(widget->area() == Dock::Float ? Z_WIDGET_FLOAT : Z_WIDGET);

  widget->setDockContainer(this);
  //    widget->setParentItem(this);
  d->dockWidgets.append(widget);

  connect(widget, &DockWidget::beginMove, this, &DockContainer::dockWidget_beginMove, Qt::QueuedConnection);

  connect(widget, &DockWidget::moving, this, &DockContainer::dockWidget_moving);
  connect(widget, &DockWidget::moved, this, &DockContainer::dockWidget_moved);
  connect(widget, &DockWidget::closed, this, &DockContainer::dockWidget_closed);
  connect(widget, &DockWidget::opened, this, &DockContainer::dockWidget_opened);
  //    connect(widget, &DockWidget::areaChanged, this, &DockContainer::dockWidget_areaChanged);
  connect(widget, &QQuickItem::visibleChanged, this, &DockContainer::dockWidget_visibleChanged);

  if (d->enableStateStoring) {
    QSettings set;
    QMetaEnum e = QMetaEnum::fromType<Dock::Area>();
    set.beginGroup(widget->title());
    if (set.contains("area"))
      widget->setArea((Dock::Area)e.keyToValue(set.value("area", widget->area()).toString().toLocal8Bit().data()));
  }
  switch (widget->area()) {
  case Dock::Left:
  case Dock::Right:
  case Dock::Top:
  case Dock::Bottom:
  case Dock::Center:
    d->dockAreas[widget->area()]->addDockWidget(widget);
    d->dockAreas[widget->area()]->polish();
    break;

    // TODO: remove this or keep!
  case Dock::NoArea:
    d->dockAreas[Dock::Center]->addDockWidget(widget);
    d->dockAreas[Dock::Center]->polish();
    break;
  default:
    qWarning() << "dock has no area " << widget->title();
    break;
  }

  if (isComponentComplete())
    reorderDockAreas();

  Q_EMIT dockWidgetsChanged(d->dockWidgets);
}

void DockContainer::removeDockWidget(DockWidget *widget) {
  Q_D(DockContainer);
  if (widget->dockArea())
    widget->dockArea()->removeDockWidget(widget);
  widget->setParentItem(nullptr);
  d->removedDockWidgets.append(widget);
}

void DockContainer::reorderDockAreas() {
  Q_D(DockContainer);

  QRectF rc;

  rc.setLeft(panelSize(Dock::Left));
  rc.setTop(panelSize(Dock::Top));
  rc.setWidth(width() - panelSize(Dock::Right) - panelSize(Dock::Left));
  rc.setHeight(height() - panelSize(Dock::Top) - panelSize(Dock::Bottom));

  qreal leftStart, leftEnd;
  qreal topStart, topEnd;
  qreal rightStart, rightEnd;
  qreal bottomStart, bottomEnd;

  if (d->topLeftOwner == Qt::LeftEdge) {
    leftStart = 0;
    topStart = rc.left();
  } else {
    leftStart = rc.top();
    topStart = 0;
  }

  if (d->topRightOwner == Qt::RightEdge) {
    topEnd = rc.right();
    rightStart = 0;
  } else {
    topEnd = width();
    rightStart = rc.top();
  }

  if (d->bottomLeftOwner == Qt::LeftEdge) {
    leftEnd = height();
    bottomStart = rc.left();
  } else {
    leftEnd = rc.bottom();
    bottomStart = 0;
  }

  if (d->bottomRightOwner == Qt::RightEdge) {
    bottomEnd = rc.right();
    rightEnd = height();
  } else {
    bottomEnd = width();
    rightEnd = rc.bottom();
  }
  d->dockAreas[Dock::Left]->setPosition(QPointF(0, leftStart));
  d->dockAreas[Dock::Left]->setSize(QSizeF(rc.left(), leftEnd - leftStart));

  d->dockAreas[Dock::Top]->setPosition(QPointF(topStart, 0));
  d->dockAreas[Dock::Top]->setSize(QSizeF(topEnd - topStart, rc.top()));

  d->dockAreas[Dock::Right]->setPosition(QPointF(rc.right(), rightStart));
  d->dockAreas[Dock::Right]->setSize(QSizeF(width() - rc.right(), rightEnd - rightStart));

  d->dockAreas[Dock::Bottom]->setPosition(QPointF(bottomStart, rc.bottom()));
  d->dockAreas[Dock::Bottom]->setSize(QSizeF(bottomEnd - bottomStart, height() - rc.bottom()));

  d->dockAreas[Dock::Center]->setPosition(rc.topLeft());
  d->dockAreas[Dock::Center]->setSize(rc.size());
}

void DockContainer::setTopLeftOwner(Qt::Edge topLeftOwner) {
  Q_D(DockContainer);

  if (topLeftOwner != Qt::LeftEdge && topLeftOwner != Qt::TopEdge) {
    qWarning() << "Invalid value for topLeftOwner: " << topLeftOwner;
    return;
  }
  if (d->topLeftOwner == topLeftOwner)
    return;

  d->topLeftOwner = topLeftOwner;
  if (isComponentComplete())
    reorderDockAreas();
  Q_EMIT topLeftOwnerChanged(d->topLeftOwner);
}

void DockContainer::setTopRightOwner(Qt::Edge topRightOwner) {
  Q_D(DockContainer);

  if (topRightOwner != Qt::TopEdge && topRightOwner != Qt::RightEdge) {
    qWarning() << "Invalid value for topRightOwner: " << topRightOwner;
    return;
  }

  if (d->topRightOwner == topRightOwner)
    return;

  d->topRightOwner = topRightOwner;
  if (isComponentComplete())
    reorderDockAreas();
  Q_EMIT topRightOwnerChanged(d->topRightOwner);
}

void DockContainer::setBottomLeftOwner(Qt::Edge bottomLeftOwner) {
  Q_D(DockContainer);

  if (bottomLeftOwner != Qt::BottomEdge && bottomLeftOwner != Qt::LeftEdge) {
    qWarning() << "Invalid value for bottomLeftOwner: " << bottomLeftOwner;
    return;
  }

  if (d->bottomLeftOwner == bottomLeftOwner)
    return;

  d->bottomLeftOwner = bottomLeftOwner;
  if (isComponentComplete())
    reorderDockAreas();
  Q_EMIT bottomLeftOwnerChanged(d->bottomLeftOwner);
}

void DockContainer::setBottomRightOwner(Qt::Edge bottomRightOwner) {
  Q_D(DockContainer);

  if (bottomRightOwner != Qt::BottomEdge && bottomRightOwner != Qt::RightEdge) {
    qWarning() << "Invalid value for bottomRightOwner: " << bottomRightOwner;
    return;
  }

  if (d->bottomRightOwner == bottomRightOwner)
    return;

  d->bottomRightOwner = bottomRightOwner;
  if (isComponentComplete())
    reorderDockAreas();
  Q_EMIT bottomRightOwnerChanged(d->bottomRightOwner);
}

void DockContainer::setEnableStateStoring(bool enableStateStoring) {
  Q_D(DockContainer);

  if (d->enableStateStoring == enableStateStoring)
    return;

  d->enableStateStoring = enableStateStoring;
  Q_EMIT enableStateStoringChanged(d->enableStateStoring);
}

void DockContainer::setDefaultDisplayType(Dock::DockWidgetDisplayType defaultDisplayType) {
  Q_D(DockContainer);

  if (d->defaultDisplayType == defaultDisplayType)
    return;

  d->defaultDisplayType = defaultDisplayType;
  Q_EMIT defaultDisplayTypeChanged(d->defaultDisplayType);
}

void DockContainer::dockWidget_beginMove() {
  Q_D(DockContainer);

  auto dw = qobject_cast<DockWidget *>(sender());

  for (auto d : d->dockWidgets)
    d->setZ(d->z() - 1);
  dw->setZ(Z_WIDGET_FLOAT);

  if (dw->dockArea()) {
    //        dw->beginDetach();
    dw->setArea(Dock::Float);
    dw->dockArea()->removeDockWidget(dw);
    dw->restoreSize();
  }

  d->dockMoveGuide->setAllowedAreas(dw->allowedAreas());
  d->dockMoveGuide->begin(mapToGlobal(QPoint(0, 0)), size());
  //    d->dockMoveGuide->setSize(size());
  //    d->dockMoveGuide->setVisible(true);
}

void DockContainer::dockWidget_moving(const QPointF &pt) {
  Q_D(const DockContainer);
  d->dockMoveGuide->setMousePos(pt);
}

void DockContainer::dockWidget_moved() {
  Q_D(DockContainer);
  d->dockMoveGuide->end();

  auto dw = qobject_cast<DockWidget *>(sender());
  if (!dw)
    return;

  switch (d->dockMoveGuide->area()) {
  case Dock::Left:
  case Dock::Right:
  case Dock::Top:
  case Dock::Bottom:
  case Dock::Center:
    if (dw->dockArea() != d->dockAreas[d->dockMoveGuide->area()]) {
      d->dockAreas[d->dockMoveGuide->area()]->addDockWidget(dw);
    }
    dw->setZ(Z_WIDGET);
    reorderDockAreas();
    break;
  case Dock::Float:
  case Dock::Detached:
    dw->setArea(d->dockMoveGuide->area());
    break;

  default:
    break;
  }
}

void DockContainer::dockWidget_opened() {
  Q_D(DockContainer);
  auto widget = qobject_cast<DockWidget *>(sender());

  widget->setParentItem(this);
  switch (widget->area()) {
  case Dock::Left:
  case Dock::Right:
  case Dock::Top:
  case Dock::Bottom:
  case Dock::Center:
    d->dockAreas[widget->area()]->addDockWidget(widget);
    d->dockAreas[widget->area()]->polish();
    break;
  // TODO: remove this or keep!
  case Dock::NoArea:
    d->dockAreas[Dock::Center]->addDockWidget(widget);
    d->dockAreas[Dock::Center]->polish();
    break;
  default:
    qWarning() << "dock has no area " << widget->title();
    break;
  }
  widget->setVisible(true);
}

void DockContainer::dockWidget_closed() {
  auto w = qobject_cast<DockWidget *>(sender());
  if (w)
    w->setParentItem(nullptr);

  //        removeDockWidget(w);
}

void DockContainer::dockWidget_visibleChanged() {
  Q_D(DockContainer);

  auto dw = qobject_cast<DockWidget *>(sender());
  if (!dw)
    return;

  if (dw->area() == Dock::Float)
    return;

  if (dw->dockArea()) {
    auto dt = dw->dockArea()->displayType();
    if (dt == Dock::TabbedView)
      return;
  }

  if (dw->isVisible()) {
    d->dockAreas[dw->area()]->addDockWidget(dw);
  } else {
    if (dw->dockArea()) {
      dw->dockArea()->removeDockWidget(dw);
    }
  }
}

void DockContainer::dockWidget_areaChanged(Dock::Area area) {
  Q_D(DockContainer);

  auto dw = qobject_cast<DockWidget *>(sender());
  if (!dw)
    return;

  if (dw->dockArea())
    dw->dockArea()->removeDockWidget(dw);
  if (area < 16 && area)
    d->dockAreas[area]->addDockWidget(dw);
}

void DockContainer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
  if (isComponentComplete())
    reorderDockAreas();
  QQuickItem::geometryChange(newGeometry, oldGeometry);
}

int DockContainer::panelSize(Dock::Area area) const {
  Q_D(const DockContainer);
  return d->dockAreas[area]->isOpen() ? d->dockAreas[area]->panelSize() : 0.;
}

DockArea *DockContainer::createGroup(Dock::Area area, DockArea *item) {
  Q_D(DockContainer);
  if (d->dockAreas.contains(area)) {
    return d->dockAreas.value(area);
  }

  if (!item)
    item = new DockArea(this);
  item->setArea(area);
  item->setVisible(true);
  item->setZ(Z_GROUP);
  item->setPanelSize(120);
  item->setDisplayType(d->defaultDisplayType);

  switch (area) {
  case Dock::Left:
    item->setTabPosition(Qt::LeftEdge);
    break;
  case Dock::Right:
    item->setTabPosition(Qt::RightEdge);
    break;

  case Dock::Bottom:
    item->setTabPosition(Qt::BottomEdge);
    break;

  default:
    item->setTabPosition(Qt::TopEdge);
    break;
  }
  d->dockAreas.insert(area, item);

  return item;
}

QRectF DockContainer::panelRect(Dock::Area area) const {
  Q_D(const DockContainer);
  auto da = d->dockAreas.value(area);

  if (da->isOpen())
    return QRectF(da->position(), da->size());

  constexpr qreal s{50};

  switch (area) {
  case Dock::Left:
    return QRectF(0, 0, s, height() - 1);

  case Dock::Top:
    return QRectF(0, 0, width() - 1, s);

  case Dock::Right:
    return QRectF(width() - s, 0, s, height() - 1);

  case Dock::Bottom:
    return QRectF(0, height() - s, width() - 1, s);

  default:
    return QRectF();
  }
}

Qt::Edge DockContainer::topLeftOwner() const {
  Q_D(const DockContainer);
  return d->topLeftOwner;
}

Qt::Edge DockContainer::topRightOwner() const {
  Q_D(const DockContainer);
  return d->topRightOwner;
}

Qt::Edge DockContainer::bottomLeftOwner() const {
  Q_D(const DockContainer);
  return d->bottomLeftOwner;
}

Qt::Edge DockContainer::bottomRightOwner() const {
  Q_D(const DockContainer);
  return d->bottomRightOwner;
}

bool DockContainer::enableStateStoring() const {
  Q_D(const DockContainer);
  return d->enableStateStoring;
}

Dock::DockWidgetDisplayType DockContainer::defaultDisplayType() const {
  Q_D(const DockContainer);
  return d->defaultDisplayType;
}

bool DockContainer::childMouseEventFilter(QQuickItem *item, QEvent *event) {

  auto handler = qobject_cast<DockWidgetMoveHandler *>(item);
  if (handler) {
    if (event->type() == QEvent::MouseButtonPress) {
      auto me = static_cast<QMouseEvent *>(event);

      handler->mousePressEvent(me);
      handler->dockWidget()->setArea(Dock::Detached);
      return true;
    }
  }

  return false;
}
