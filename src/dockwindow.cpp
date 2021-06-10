#include "dockwindow.h"
#include "dockwidget.h"
#include <QDebug>

DockWindow::DockWindow(QQuickItem *dockWidget, QWindow *parent)
    : QQuickWindow(parent), _dockWidget{dockWidget}
{
    if (_dockWidget) {
        _dockWidget->setParentItem(contentItem());
        _dockWidget->setPosition(QPointF(0, 0));
    }
    setFlags(Qt::FramelessWindowHint | Qt::Tool);
}

QQuickItem *DockWindow::dockWidget() const
{
    return _dockWidget;
}

void DockWindow::setDockWidget(QQuickItem *dockWidget)
{
    _dockWidget = dockWidget;
    _dockWidget->setParentItem(contentItem());
    _dockWidget->setPosition(QPointF(0, 0));
    _dockWidget->setSize(size());
}

void DockWindow::resizeEvent(QResizeEvent *)
{
    _dockWidget->setSize(size());
}
