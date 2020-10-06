#include "dockstyle.h"
#include "dockwidgetbackground.h"

#include <QPainter>

DockWidgetBackground::DockWidgetBackground(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

}

void DockWidgetBackground::paint(QPainter *painter)
{
    DockStyle::instance()->paintDockWidgetBackground(painter, this);
}
