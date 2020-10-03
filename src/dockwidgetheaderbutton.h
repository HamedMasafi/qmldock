#ifndef DOCKWIDGETHEADERBUTTON_H
#define DOCKWIDGETHEADERBUTTON_H

#include <QImage>
#include <QQuickPaintedItem>
#include "dock.h"

class DockWidgetHeaderButton : public QQuickPaintedItem
{
    Q_OBJECT
    Dock::ButtonStatus _status;
    QString _icon;

public:
    DockWidgetHeaderButton(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    QString icon() const;
    void setIcon(const QString &icon);

signals:
    void clicked();

protected:
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DOCKWIDGETHEADERBUTTON_H
