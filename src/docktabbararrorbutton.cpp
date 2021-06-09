#include "docktabbararrorbutton.h"
#include "style/abstractstyle.h"

#include <QCursor>

DockTabBarArrorButton::DockTabBarArrorButton(QQuickItem *parent)
: AbstractButton(parent)
{
//    setCursor(Qt::WaitCursor);
}

void DockTabBarArrorButton::paint(QPainter *painter)
{
    dockStyle->paintDockTabBarArrowButton(painter, this, _status);
}
