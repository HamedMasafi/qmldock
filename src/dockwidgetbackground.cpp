#include "style/abstractstyle.h"
#include "dockwidgetbackground.h"

#include <QPainter>

DockWidgetBackground::DockWidgetBackground(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

}

void DockWidgetBackground::paint(QPainter *painter)
{
    dockStyle->paintDockWidgetBackground(painter, this);
}
