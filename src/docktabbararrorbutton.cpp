#include "docktabbararrorbutton.h"
#include "style/abstractstyle.h"

#include <QCursor>

DockTabBarArrorButton::DockTabBarArrorButton(QQuickItem *parent)
: AbstractButton(parent)
{
//    setCursor(Qt::WaitCursor);
    setActiveFocusOnTab(true);
}

void DockTabBarArrorButton::paint(QPainter *painter)
{
    if (hasFocus())
        _status = Dock::Hovered;
    dockStyle->paintDockTabBarArrowButton(painter, this, _status);
}
