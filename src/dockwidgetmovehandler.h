#ifndef DOCKWIDGETMOVEHANDLER_H
#define DOCKWIDGETMOVEHANDLER_H

#include <QQuickItem>

class DockWidget;

class DockWidgetMoveHandler : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(DockWidget *dockWidget READ dockWidget WRITE setDockWidget NOTIFY dockWidgetChanged)

  DockWidget *m_dockWidget;
  QPointF _lastMousePos;
  QPointF _lastParentPos;
  bool _moveEmitted;
  QString _title;
  bool _enableMove;

public:
  DockWidgetMoveHandler(QQuickItem *parent = nullptr);

  DockWidget *dockWidget() const;

public Q_SLOTS:
  void setDockWidget(DockWidget *dockWidget);

Q_SIGNALS:
  void dockWidgetChanged(DockWidget *dockWidget);
  void moveStarted();
  void moving(const QPointF &windowPos, const QPointF &cursorPos);
  void moveEnded();

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  friend class DockContainer;
};

#endif // DOCKWIDGETMOVEHANDLER_H
