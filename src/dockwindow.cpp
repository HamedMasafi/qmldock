#include "dockwindow.h"
#include "dockwidget.h"
#include <QDebug>

DockWindow::DockWindow(DockWidget *dockWidget, QWindow *parent)
    : QQuickWindow(parent), _dockWidget{dockWidget}
{
    if (_dockWidget) {
        _dockWidget->setParentItem(contentItem());
        _dockWidget->setPosition(QPointF(0, 0));
    }
    setFlags(Qt::FramelessWindowHint | Qt::Tool);
}

DockWidget *DockWindow::dockWidget() const
{
    return _dockWidget;
}

void DockWindow::setDockWidget(DockWidget *dockWidget)
{
    _dockWidget = dockWidget;
    _dockWidget->setParentItem(contentItem());
    _dockWidget->setPosition(QPointF(0, 0));
    _dockWidget->setSize(size());
    auto widget = qobject_cast<DockWidget*>(dockWidget);
    if (widget)
        connect(widget, &DockWidget::closed, this, &DockWindow::dockWidget_closed);
}

void DockWindow::dockWidget_closed()
{
    close();
}

void DockWindow::resizeEvent(QResizeEvent *)
{
    _dockWidget->setSize(size());
}
