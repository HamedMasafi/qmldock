#include "style/abstractstyle.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>

DockWidgetHeaderButton::DockWidgetHeaderButton(QQuickItem *parent)
    : AbstractButton(parent)
{
    setSize(QSizeF(16, 16));
}
void DockWidgetHeaderButton::paint(QPainter *painter)
{
    dockStyle->paintWidgetButton(painter, this, _status);
}
