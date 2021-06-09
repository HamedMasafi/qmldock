#include "style/abstractstyle.h"
#include "dockwidget.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"
#include "dockwidgetmovehandler.h"
#include "dockwindow.h"

#include <QDebug>
#include <QPainter>

QString DockWidgetHeader::title() const
{
    return _title;
}

void DockWidgetHeader::setTitle(const QString &title)
{
    _title = title;
    update();
}

bool DockWidgetHeader::closeButtonVisible() const
{
    return closeButton->isVisible();
}

void DockWidgetHeader::setCloseButtonVisible(bool closeButtonVisible)
{
    closeButton->setVisible(closeButtonVisible);
}

bool DockWidgetHeader::enableMove() const
{
    return _enableMove;
}

void DockWidgetHeader::setEnableMove(bool enableMove)
{
    _enableMove = enableMove;
}

bool DockWidgetHeader::pinButtonVisible() const
{
    return pinButton->isVisible();
}

void DockWidgetHeader::setPinButtonVisible(bool pinButtonVisible)
{
    pinButton->setVisible(pinButtonVisible);
}

DockWidget *DockWidgetHeader::parentDock() const
{
    return _parentDock;
}

DockWidgetMoveHandler *DockWidgetHeader::moveHandler() const
{
    return _moveHandler;
}

DockWidgetHeader::DockWidgetHeader(DockWidget *parent)
    : QQuickPaintedItem(parent),
      _parentDock(parent)
      , pinButton{new DockWidgetHeaderButton(this)}
      , closeButton{new DockWidgetHeaderButton(this)}
      , _moveHandler{new DockWidgetMoveHandler(this)}
      , _enableMove(true)

{
    setHeight(20);
//    setAcceptedMouseButtons(Qt::LeftButton);
    setClip(true);
//    setAcceptHoverEvents(true);

    pinButton->setIcon(Dock::PinIcon);
    pinButton->setY(5);
//    pinButton->setVisible(true);

    closeButton->setIcon(Dock::CloseIcon);
    closeButton->setY(5);

    _buttons.append(closeButton);
//    _buttons.append(pinButton);

    connect(closeButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::close);

    connect(pinButton,
            &DockWidgetHeaderButton::clicked,
            parent,
            &DockWidget::detach);

    _moveHandler->setParentItem(this);
    _moveHandler->setDockWidget(parent);

    AbstractStyle::registerThemableItem(this);
}

void DockWidgetHeader::paint(QPainter *painter)
{
    dockStyle->paintDockWidgetHeader(painter, this);
}

void DockWidgetHeader::geometryChanged(const QRectF &newGeometry,
                                       const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);

    if (!isComponentComplete())
        return;

    int buttonMargin{20};
    for (auto &btn: _buttons) {
        btn->setX(width() - buttonMargin);
        buttonMargin += 20;
    }

    _moveHandler->setHeight(height());
    _moveHandler->setWidth(width() - 40);
}
