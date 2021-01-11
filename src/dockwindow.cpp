#include "dockwindow.h"
#include "dockwidget.h"
#include <QDebug>

DockWindow::DockWindow(DockWidget *dock, QWindow *parent) : QQuickWindow(parent)
      , _dockWidget(dock)
{
    setFlags(Qt::FramelessWindowHint | Qt::Tool);
}

void DockWindow::resizeEvent(QResizeEvent *)
{
    _dockWidget->setSize(size());
}
