#ifndef DOCKMOVEGUIDE_H
#define DOCKMOVEGUIDE_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockMoveGuide : public QQuickPaintedItem
{
    Q_OBJECT
    QPointF _mousePos;
    Dock::Area _area;

public:
    DockMoveGuide(QQuickItem *parent = nullptr);

signals:
    void dropped(Dock::Area area);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
    QPointF mousePos() const;
    void setMousePos(const QPointF &mousePos);

    Dock::Area area() const;

private:
    bool drawRect(QPainter *painter, const QRectF &rc);
};

#endif // DOCKMOVEGUIDE_H
