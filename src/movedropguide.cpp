#include "movedropguide.h"

#include <QPainter>

MoveDropGuide::MoveDropGuide(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

void MoveDropGuide::paint(QPainter *painter)
{
    painter->setOpacity(.4);
    painter->fillRect(clipRect(), Qt::blue);
    painter->setOpacity(1);
}
