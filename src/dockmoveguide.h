#ifndef DOCKMOVEGUIDE_H
#define DOCKMOVEGUIDE_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockMoveGuide : public QQuickPaintedItem
{
    Q_OBJECT
    QPointF _mousePos;
    Dock::Area _area;
    Dock::Areas _allowedAreas;

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

    Dock::Areas allowedAreas() const;
    void setAllowedAreas(const Dock::Areas &allowedAreas);

private:
    bool drawRect(QPainter *painter, const QRectF &rc);
};

#endif // DOCKMOVEGUIDE_H
