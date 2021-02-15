#ifndef ABSTRACTMOUSERESPONSIVEITEM_H
#define ABSTRACTMOUSERESPONSIVEITEM_H

#include <QQuickPaintedItem>
#include "dock.h"

class AbstractMouseResponsiveItem : public QQuickPaintedItem
{
    Q_OBJECT
    Dock::ButtonStatus _status;

public:
    AbstractMouseResponsiveItem(QQuickItem *parent = nullptr);

Q_SIGNALS:


    // QQuickItem interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);

protected:
    Dock::ButtonStatus status() const;
};

#endif // ABSTRACTMOUSERESPONSIVEITEM_H
