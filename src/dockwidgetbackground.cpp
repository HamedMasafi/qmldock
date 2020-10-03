#include "dockwidgetbackground.h"

#include <QPainter>

DockWidgetBackground::DockWidgetBackground(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

}

void DockWidgetBackground::paint(QPainter *painter)
{
    painter->setOpacity(1);
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::white);
    painter->drawRect(0, 0, width() - 1, height() - 1);
}
