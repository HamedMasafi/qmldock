#ifndef DOCKWIDGETHEADER_H
#define DOCKWIDGETHEADER_H

#include <QImage>
#include <QQuickPaintedItem>

class DockWidget;
class DockWidgetHeaderButton;
class DockWidgetMoveHandler;

class DockWidgetHeader : public QQuickPaintedItem {
  Q_OBJECT
  DockWidget *_parentDock;
  QPointF _lastMousePos;
  QPointF _lastParentPos;
  bool _moveEmitted;
  QString _title;

  DockWidgetHeaderButton *pinButton;
  DockWidgetHeaderButton *closeButton;
  DockWidgetMoveHandler *_moveHandler;

  enum ButtonStatus { Normal, Hovered, Pressed };

  bool _enableMove;

public:
  DockWidgetHeader(DockWidget *parent = nullptr);

  // Q_SIGNALS:
  //     void moveStarted();
  //     void moving(const QPointF &windowPos, const QPointF &cursorPos);
  //     void moveEnded();

  // QQuickPaintedItem interface
public:
  void paint(QPainter *painter);

  //    // QQuickItem interface
  QString title() const;

  bool closeButtonVisible() const;
  void setCloseButtonVisible(bool closeButtonVisible);

  bool enableMove() const;
  void setEnableMove(bool enableMove);

  bool pinButtonVisible() const;
  void setPinButtonVisible(bool pinButtonVisible);

  DockWidget *parentDock() const;

  DockWidgetMoveHandler *moveHandler() const;

public Q_SLOTS:
  void setTitle(const QString &title);

  // protected:
  //     void mousePressEvent(QMouseEvent *event);
  //     void mouseMoveEvent(QMouseEvent *event);
  //     void mouseReleaseEvent(QMouseEvent *event);

protected:
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry);
};

#endif // DOCKWIDGETHEADER_H
