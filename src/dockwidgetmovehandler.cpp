#include "dockwidgetmovehandler.h"
#include "dockwidget.h"
#include "dockwindow.h"

DockWidgetMoveHandler::DockWidgetMoveHandler(QQuickItem *parent)
    : QQuickItem(parent), _enableMove{true}
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::ArrowCursor);
}

DockWidget *DockWidgetMoveHandler::dockWidget() const
{
    return m_dockWidget;
}

void DockWidgetMoveHandler::setDockWidget(DockWidget *dockWidget)
{
    if (m_dockWidget == dockWidget)
        return;

    m_dockWidget = dockWidget;
    Q_EMIT dockWidgetChanged(m_dockWidget);
}


void DockWidgetMoveHandler::mousePressEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    _moveEmitted = false;
    if (m_dockWidget->getIsDetached()) {
        _lastParentPos = m_dockWidget->dockWindow()->position();
        _lastMousePos = event->globalPos();
    } else {
        _lastMousePos = event->windowPos();
        _lastParentPos = m_dockWidget->position();
    }
//    grabMouse();
}

void DockWidgetMoveHandler::mouseMoveEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    if (_moveEmitted) {
        if (m_dockWidget->getIsDetached())
            Q_EMIT moving(_lastParentPos + (event->globalPos() - _lastMousePos),
                        event->pos() + m_dockWidget->dockWindow()->position());
        else
            Q_EMIT moving(_lastParentPos + (event->windowPos() - _lastMousePos),
                        event->pos() + m_dockWidget->position());
    } else {
        Q_EMIT moveStarted();
        _moveEmitted = true;
    }
}

void DockWidgetMoveHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if (!_enableMove) {
        event->ignore();
        return;
    }
    Q_UNUSED(event)

    if (_moveEmitted)
        Q_EMIT moveEnded();

    _moveEmitted = false;
    ungrabMouse();
}
