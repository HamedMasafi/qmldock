#ifndef DOCKGROUPRESIZEHANDLER_H
#define DOCKGROUPRESIZEHANDLER_H

#include <QQuickPaintedItem>

class DockAreaResizeHandler : public QQuickPaintedItem {
  Q_OBJECT
  Qt::Orientation _orientation;
  bool _hasHover;
  qreal _lastMousePos;
  qreal _lastHandlerPos;
  int _index;
  qreal _pos;

public:
  DockAreaResizeHandler(Qt::Orientation orientation, QQuickItem *parent = nullptr);

Q_SIGNALS:
  void moving(qreal pos, bool *ok);
  void moved();

  // QQuickPaintedItem interface
public:
  void paint(QPainter *painter);

  // QQuickItem interface
  int index() const;
  void setIndex(int index);

  qreal pos() const;
  void setPos(const qreal &pos);

  Qt::Orientation orientation() const;

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  // QQuickItem interface
protected:
  void hoverEnterEvent(QHoverEvent *event);
  void hoverLeaveEvent(QHoverEvent *event);

private:
  void drawCircle(QPainter *painter, const QPointF &center, bool hover = false);
};

#endif // DOCKGROUPRESIZEHANDLER_H
