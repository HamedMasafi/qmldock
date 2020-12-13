#ifndef DOCKMOVEGUIDE_H
#define DOCKMOVEGUIDE_H

#include <QQuickPaintedItem>
#include <QPair>
#include "dock.h"

class QQuickWindow;
class DockContainer;
class MoveDropGuide;
class DockMoveGuide : public QQuickPaintedItem
{
    Q_OBJECT
    QPointF _mousePos;
    DockContainer *_parentDockContainer;
    Dock::Area _area;
    Dock::Areas _allowedAreas;
    QQuickWindow *_window;
    using LocalAndGlobalRect = QPair<QRectF, QRectF>;

    QMultiMap<Dock::Area, LocalAndGlobalRect> _areas;
//    QMultiMap<Dock::Area, QRectF> _globalAreas;
    MoveDropGuide *_dropArea;

    void insertToAreas(Dock::Area, const QRectF &);
public:
    DockMoveGuide(DockContainer *parent = nullptr);

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
