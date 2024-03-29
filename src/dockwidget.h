#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QJSValue>
#include <QQuickPaintedItem>
#include "dock.h"

class DockArea;
class DockWidgetHeader;
class DockWindow;
class DockContainer;
class DockWidgetPrivate;

class DockWidget : public QQuickPaintedItem {
  Q_OBJECT

  Q_DECLARE_PRIVATE(DockWidget)
  DockWidgetPrivate *d_ptr;

  Q_PROPERTY(Dock::Area area READ area WRITE setArea NOTIFY areaChanged)
  Q_PROPERTY(bool closable READ closable WRITE setClosable NOTIFY closableChanged)
  Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
  Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
  Q_PROPERTY(bool showHeader READ showHeader WRITE setShowHeader NOTIFY showHeaderChanged)
  Q_PROPERTY(bool detachable READ detachable WRITE setDetachable NOTIFY detachableChanged)
  Q_PROPERTY(QQuickItem *contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged FINAL)
  Q_PROPERTY(QQuickItem *titleBar READ titleBar WRITE setTitleBar NOTIFY titleBarChanged)
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(Dock::Areas allowedAreas READ allowedAreas WRITE setAllowedAreas NOTIFY allowedAreasChanged)
  Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
  Q_PROPERTY(QJSValue closeEvent READ closeEvent WRITE setCloseEvent NOTIFY closeEventChanged)
  Q_PROPERTY(DockWidgetVisibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)

  //    Q_PROPERTY(DockWidgetFlags flags READ flags WRITE setFlags NOTIFY flagsChanged)

public:
  enum DockWidgetVisibility { Openned, Active, Hidden, Closed };
  Q_ENUM(DockWidgetVisibility)

  enum DockWidgetFlag { Movable, Detachable, Resizable };
  Q_DECLARE_FLAGS(DockWidgetFlags, DockWidgetFlag)
  Q_FLAG(DockWidgetFlags)

  DockWidget(QQuickItem *parent = nullptr);
  virtual ~DockWidget();

  DockArea *dockArea() const;
  Dock::Area area() const;
  bool closable() const;
  bool resizable() const;
  bool movable() const;
  bool showHeader() const;
  bool detachable() const;
  void paint(QPainter *painter) override;
  QQuickItem *contentItem() const;

  QString title() const;

  DockContainer *dockContainer() const;
  void setDockContainer(DockContainer *dockContainer);

  bool getIsDetached() const;

  Dock::Areas allowedAreas() const;
  DockWindow *dockWindow() const;

  QQuickItem *titleBar() const;
  void componentComplete() override;

  bool isActive() const;

  QJSValue closeEvent() const;

  DockWidgetVisibility visibility() const;
  void setVisibility(DockWidgetVisibility newVisibility);

public Q_SLOTS:
  Q_DECL_DEPRECATED
  void detach();

  void beginDetach();
  void open();
  void close();
  void restoreSize();

  void setDockArea(DockArea *dockArea);
  void setArea(Dock::Area area);
  void setClosable(bool closable);
  void setResizable(bool resizable);
  void setMovable(bool movable);
  void setShowHeader(bool showHeader);
  void setDetachable(bool detachable);

  void setContentItem(QQuickItem *contentItem);

  void setTitle(QString title);

  void setAllowedAreas(Dock::Areas allowedAreas);

  void setTitleBar(QQuickItem *titleBar);

  void setCloseEvent(QJSValue closeEvent);

private Q_SLOTS:
  void header_moveStarted();
  void header_moving(const QPointF &windowPos, const QPointF &cursorPos);
  void header_moveEnded();

protected:
  bool childMouseEventFilter(QQuickItem *, QEvent *) override;
  void itemChange(ItemChange, const ItemChangeData &) override;
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
  void hoverMoveEvent(QHoverEvent *event) override;
  void hoverLeaveEvent(QHoverEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

Q_SIGNALS:
  void beginMove();
  void moving(const QPointF &pt);
  void moved();
  void closed();
  void opened();
  void areaChanged(Dock::Area area);
  void closableChanged(bool closable);
  void resizableChanged(bool resizable);
  void movableChanged(bool movable);
  void showHeaderChanged(bool showHeader);
  void detachableChanged(bool detachable);
  void contentItemChanged(QQuickItem *contentItem);
  void titleChanged(QString title);
  void allowedAreasChanged(Dock::Areas allowedAreas);
  void titleBarChanged(QQuickItem *titleBar);
  void isActiveChanged(bool isActive);
  void closeEventChanged(QJSValue closeEvent);
  void visibilityChanged();

private:
  void setIsActive(bool isActive);

  friend class DockContainer;
  friend class DockArea;
  QJSValue m_closeEvent;

  friend class DockWidgetHeader;
};

#endif // DOCKWIDGET_H
