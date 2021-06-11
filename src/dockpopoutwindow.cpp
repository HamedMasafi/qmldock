#include "dockpopoutwindow.h"

#include <QQuickItem>


DockPopoutWindow::DockPopoutWindow(QQuickItem *contentItem, QWindow *parent)
: QQuickWindow(parent), _contentItem{contentItem}
{
    if (_contentItem) {
        _contentItem->setParentItem(this->contentItem());
        _contentItem->setPosition(QPointF(0, 0));
    }
    setFlags(Qt::Tool);
}

void DockPopoutWindow::resizeEvent(QResizeEvent *)
{
    _contentItem->setSize(size());
}
