#include "debugrect.h"

#include <QPainter>

DebugRect::DebugRect(QColor color, QQuickItem *parent) : QQuickPaintedItem(parent), _color(color) {}

void DebugRect::paint(QPainter *painter) {
  painter->fillRect(clipRect(), _color);
}
