#ifndef DOCKMOVEGUIDE_H
#define DOCKMOVEGUIDE_H

#include <QQuickPaintedItem>
#include "dock.h"

class QQuickWindow;
class DockMoveGuide : public QQuickPaintedItem
{
    Q_OBJECT
    QPointF _mousePos;
    Dock::Area _area;
    Dock::Areas _allowedAreas;
    QQuickWindow *_window;
    QMultiMap<Dock::Area, QRectF> _areas;

public:
    DockMoveGuide(QQuickItem *parent = nullptr);

public:
    void begin(const QPointF &pos, const QSizeF &size);
    void end();
    void paint(QPainter *painter);
    QPointF mousePos() const;
    void setMousePos(const QPointF &mousePos);

    Dock::Area area() const;

    Dock::Areas allowedAreas() const;
    void setAllowedAreas(const Dock::Areas &allowedAreas);

signals:
    void dropped(Dock::Area area);
};

#endif // DOCKMOVEGUIDE_H
