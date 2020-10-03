#ifndef DOCKWIDGETBACKGROUND_H
#define DOCKWIDGETBACKGROUND_H

#include <QQuickPaintedItem>

class DockWidgetBackground : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DockWidgetBackground(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
};

#endif // DOCKWIDGETBACKGROUND_H
