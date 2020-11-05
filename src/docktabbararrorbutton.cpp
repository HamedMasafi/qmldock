#include "docktabbararrorbutton.h"
#include "style/abstractstyle.h"

DockTabBarArrorButton::DockTabBarArrorButton(QQuickItem *parent)
: AbstractButton(parent)
{

}

void DockTabBarArrorButton::paint(QPainter *painter)
{
    dockStyle->paintDockTabBarArrowButton(painter, this, _status);
}
