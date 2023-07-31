#ifndef DEBUGRECT_H
#define DEBUGRECT_H

#include <QQuickPaintedItem>

class DebugRect : public QQuickPaintedItem {
  Q_OBJECT
  QColor _color;

public:
  DebugRect(QColor color, QQuickItem *parent);

Q_SIGNALS:

  // QQuickPaintedItem interface
public:
  void paint(QPainter *painter);
};

#endif // DEBUGRECT_H
