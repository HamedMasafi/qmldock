#include "dockactivewidgetdim.h"

#include <QPainter>

DockActiveWidgetDim::DockActiveWidgetDim(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton);
}

void DockActiveWidgetDim::paint(QPainter *painter)
{
//    painter->setOpacity(.1);
//    painter->fillRect(clipRect(), Qt::black);
}

void DockActiveWidgetDim::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked();
    event->ignore();
}
