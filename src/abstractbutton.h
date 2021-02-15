#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

#include <QQuickPaintedItem>
#include "dock.h"

class AbstractButton : public QQuickPaintedItem
{
    Q_OBJECT

protected:
    Dock::ButtonStatus _status;
    Dock::Icon _icon;

public:
    AbstractButton(QQuickItem *parent = nullptr);

    Dock::Icon icon() const;
    void setIcon(const Dock::Icon &icon);

Q_SIGNALS:
    void clicked();

protected:
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // ABSTRACTBUTTON_H
