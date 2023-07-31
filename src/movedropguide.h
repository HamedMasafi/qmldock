#ifndef MOVEDROPGUIDE_H
#define MOVEDROPGUIDE_H

#include <QQuickPaintedItem>

class MoveDropGuide : public QQuickPaintedItem {
  Q_OBJECT
public:
  MoveDropGuide(QQuickItem *parent = nullptr);
  void paint(QPainter *painter);
};

#endif // MOVEDROPGUIDE_H
