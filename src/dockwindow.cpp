#include "dockwindow.h"
#include "dockwidget.h"
#include <QDebug>

DockWindow::DockWindow(DockWidget *dock, QWindow *parent) : QQuickWindow(parent)
      , _dockWidget(dock)
{
    qDebug() << "Dock window created";
    _dockWidget->setParentItem(contentItem());
    _dockWidget->setPosition(QPointF(0, 0));
    setFlags(Qt::FramelessWindowHint | Qt::Tool);
}

//void DockWindow::mousePressEvent(QMouseEvent *e)
//{
//    qDebug() << Q_FUNC_INFO;
//    QQuickWindow::mousePressEvent(e);
//}

//void DockWindow::mouseReleaseEvent(QMouseEvent *) {}

//void DockWindow::mouseDoubleClickEvent(QMouseEvent *) {}

//void DockWindow::mouseMoveEvent(QMouseEvent *) {}

//void DockWindow::moveEvent(QMoveEvent *)
//{
//    qDebug() << Q_FUNC_INFO;
//}

void DockWindow::resizeEvent(QResizeEvent *)
{
    qDebug() << "resize" << size();
    _dockWidget->setSize(size());
}
