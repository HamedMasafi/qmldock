#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H

#include "dock.h"

#include <QQuickPaintedItem>

class DockWidget;
class DockMoveGuide;
class DockArea;
class DockAreaResizeHandler;
class DockContainerPrivate;

class DockContainer : public QQuickPaintedItem {
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockContainer);
  DockContainerPrivate *d_ptr;

  Q_PROPERTY(QList<DockWidget *> dockWidgets READ dockWidgets NOTIFY dockWidgetsChanged)
  Q_PROPERTY(Qt::Edge topLeftOwner READ topLeftOwner WRITE setTopLeftOwner NOTIFY topLeftOwnerChanged)
  Q_PROPERTY(Qt::Edge topRightOwner READ topRightOwner WRITE setTopRightOwner NOTIFY topRightOwnerChanged)
  Q_PROPERTY(Qt::Edge bottomLeftOwner READ bottomLeftOwner WRITE setBottomLeftOwner NOTIFY bottomLeftOwnerChanged)
  Q_PROPERTY(Qt::Edge bottomRightOwner READ bottomRightOwner WRITE setBottomRightOwner NOTIFY bottomRightOwnerChanged)
  Q_PROPERTY(bool enableStateStoring READ enableStateStoring WRITE setEnableStateStoring NOTIFY enableStateStoringChanged)
  Q_PROPERTY(Dock::DockWidgetDisplayType defaultDisplayType READ defaultDisplayType WRITE setDefaultDisplayType NOTIFY
                 defaultDisplayTypeChanged)

public:
  DockContainer(QQuickItem *parent = nullptr);
  virtual ~DockContainer();

  void paint(QPainter *painter) override;

  QList<DockWidget *> dockWidgets() const;

Q_SIGNALS:
  void dockWidgetsChanged(QList<DockWidget *> dockWidgets);
  void topLeftOwnerChanged(Qt::Edge topLeftOwner);
  void topRightOwnerChanged(Qt::Edge topRightOwner);
  void bottomLeftOwnerChanged(Qt::Edge bottomLeftOwner);
  void bottomRightOwnerChanged(Qt::Edge bottomRightOwner);

  void enableStateStoringChanged(bool enableStateStoring);

  void defaultDisplayTypeChanged(Dock::DockWidgetDisplayType defaultDisplayType);

protected:
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
  void itemChange(ItemChange, const ItemChangeData &) override;
  void componentComplete() override;

public Q_SLOTS:
  void storeSettings();
  void restoreSettings();

  void addDockWidget(DockWidget *widget);
  void removeDockWidget(DockWidget *widget);
  void reorderDockAreas();

  void setTopLeftOwner(Qt::Edge topLeftOwner);
  void setTopRightOwner(Qt::Edge topRightOwner);
  void setBottomLeftOwner(Qt::Edge bottomLeftOwner);
  void setBottomRightOwner(Qt::Edge bottomRightOwner);

  void setEnableStateStoring(bool enableStateStoring);

  void setDefaultDisplayType(Dock::DockWidgetDisplayType defaultDisplayType);

private Q_SLOTS:
  void dockWidget_beginMove();
  void dockWidget_moving(const QPointF &pt);
  void dockWidget_moved();
  void dockWidget_opened();
  void dockWidget_closed();
  void dockWidget_visibleChanged();
  void dockWidget_areaChanged(Dock::Area area);

private:
  inline int panelSize(Dock::Area area) const;
  DockArea *createGroup(Dock::Area area, DockArea *item = nullptr);
  QRectF panelRect(Dock::Area area) const;

public:
  Qt::Edge topLeftOwner() const;
  Qt::Edge topRightOwner() const;
  Qt::Edge bottomLeftOwner() const;
  Qt::Edge bottomRightOwner() const;

  friend class DockMoveGuide;
  bool enableStateStoring() const;
  Dock::DockWidgetDisplayType defaultDisplayType() const;

  // QQuickItem interface
protected:
  bool childMouseEventFilter(QQuickItem *, QEvent *) override;
};

#endif // DOCKCONTAINER_H
