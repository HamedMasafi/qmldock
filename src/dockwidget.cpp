#include "dockwidget.h"
#include "dockwidgetheader.h"
#include "debugrect.h"
#include "dockwidgetbackground.h"
#include "dockwindow.h"
#include "dockstyle.h"

#include <QDebug>
#include <QPainter>
#include <QQuickWindow>

DockGroup *DockWidget::dockGroup() const
{
    return _dockGroup;
}

void DockWidget::setDockGroup(DockGroup *dockGroup)
{
    _dockGroup = dockGroup;
}

Dock::Area DockWidget::area() const
{
    return m_area;
}

bool DockWidget::closable() const
{
    return m_closable;
}

bool DockWidget::resizable() const
{
    return m_resizable;
}

bool DockWidget::movable() const
{
    return m_movable;
}

bool DockWidget::showHeader() const
{
    return m_showHeader;
}

bool DockWidget::detachable() const
{
    return m_detachable;
}

void DockWidget::paint(QPainter *painter)
{
    DockStyle::instance()->paintDockWidget(painter, this);
//    QQuickPaintedItem::paint(painter);
}

DockWidget::DockWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent), _dockGroup{nullptr}, _originalSize{200, 200}
      , isDetached{false}, m_closable{true}, m_resizable{true}, m_movable{true}
    , m_showHeader{true}, m_detachable{false}, m_contentItem{nullptr}
{
    _header = new DockWidgetHeader(this);
    _header->setPosition(QPointF(DockStyle::instance()->widgetPadding(), DockStyle::instance()->widgetPadding()));
    _header->setSize(QSizeF(width(), 30));
    _header->setVisible(true);
    _header->setZ(999);

    connect(_header,
            &DockWidgetHeader::moveStarted,
            this,
            &DockWidget::header_moveStarted);
    connect(_header,
            &DockWidgetHeader::moving,
            this,
            &DockWidget::header_moving);
    connect(_header,
            &DockWidgetHeader::moveEnded,
            this,
            &DockWidget::header_moveEnded);

//    connect(this,
//            &QQuickPage::titleChanged,
//            this,
//            &DockWidget::this_titleChanged);
    setClip(true);
    setSize(QSizeF(200, 200));
    setFiltersChildMouseEvents(true);
//    setAcceptedMouseButtons(Qt::LeftButton);
}

void DockWidget::detach()
{
    dockWindow = new DockWindow(this);
    dockWindow->setTitle(title());
    isDetached = true;
    dockWindow->show();
}

void DockWidget::close()
{
    if (isDetached && dockWindow)
        dockWindow->setVisible(false);
    else
        setVisible(false);
}

void DockWidget::restoreSize()
{
    setSize(_originalSize);
}

void DockWidget::setArea(Dock::Area area)
{
    if (m_area == area)
        return;

    m_area = area;
    emit areaChanged(m_area);
}

void DockWidget::setClosable(bool closable)
{
    if (m_closable == closable)
        return;

    _header->setCloseButtonVisible(closable);
    m_closable = closable;
    emit closableChanged(m_closable);
}

void DockWidget::setResizable(bool resizable)
{
    if (m_resizable == resizable)
        return;

    m_resizable = resizable;
    emit resizableChanged(m_resizable);
}

void DockWidget::setMovable(bool movable)
{
    if (m_movable == movable)
        return;

    m_movable = movable;
    _header->setEnableMove(movable);
    emit movableChanged(m_movable);
}

void DockWidget::setShowHeader(bool showHeader)
{
    if (m_showHeader == showHeader)
        return;

    _header->setVisible(showHeader);
    m_showHeader = showHeader;
    emit showHeaderChanged(m_showHeader);
}

void DockWidget::setDetachable(bool detachable)
{
    if (m_detachable == detachable)
        return;

    m_detachable = detachable;
    _header->setPinButtonVisible(detachable);
    emit detachableChanged(m_detachable);
}

void DockWidget::setContentItem(QQuickItem *contentItem)
{
    if (m_contentItem == contentItem)
        return;

    m_contentItem = contentItem;

    m_contentItem->setParentItem(this);
    m_contentItem->setPosition(QPointF(
                                   DockStyle::instance()->widgetPadding(),
                                   (m_showHeader ? _header->height() : 0)
                                    +DockStyle::instance()->widgetPadding()
                                   ));
    geometryChanged(QRectF(), QRectF());
    emit contentItemChanged(m_contentItem);
}

void DockWidget::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    _header->setTitle(title);
    emit titleChanged(m_title);
}

void DockWidget::header_moveStarted()
{
    emit beginMove();
}

void DockWidget::header_moving(const QPointF &hotspot)
{
    emit moving(hotspot);
}

void DockWidget::header_moveEnded()
{
    emit moved();
}

bool DockWidget::childMouseEventFilter(QQuickItem *item, QEvent *e)
{
    static QPointF _lastMousePos;
    static QPointF _lastChildPos;
    static bool _moveEmitted = false;

    if (item != _header || !m_movable)
        return false;

    auto me = static_cast<QMouseEvent *>(e);
    if (isDetached) {
        if (me->button() == Qt::LeftButton)
            dockWindow->startSystemMove();
        return true;
    }

    switch (e->type()) {
    case QEvent::MouseButtonPress:
        _lastMousePos = me->windowPos();
        _lastChildPos = position();
        emit beginMove();
//        detach();
        _moveEmitted = false;
        break;

    case QEvent::MouseMove: {
        if (_moveEmitted) {
            emit moving(me->localPos());
        } else {
            emit beginMove();
            _moveEmitted = true;
        }
        auto pt = _lastChildPos + (me->windowPos() - _lastMousePos);

        setPosition(pt);
        emit moving(pt + me->pos());
        break;
    }

    case QEvent::MouseButtonRelease:
        emit moved();
        break;;

    default:
        break;
    }

    return true;
}

void DockWidget::itemChange(QQuickItem::ItemChange change,
                            const QQuickItem::ItemChangeData &data)
{
    QQuickItem::itemChange(change, data);

}

void DockWidget::geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry)
{
    Q_UNUSED(oldGeometry)
    if (m_area == Dock::Float) {
        _originalSize = newGeometry.size();
    }


    _header->setWidth(width() - 2 * DockStyle::instance()->widgetPadding());
    _header->setPosition(QPointF(DockStyle::instance()->widgetPadding(), DockStyle::instance()->widgetPadding()));

    if (m_contentItem) {
        m_contentItem->setPosition(QPointF(
                                       DockStyle::instance()->widgetPadding(),
                                       (m_showHeader ? _header->height() : 0)
                                        +DockStyle::instance()->widgetPadding()
                                       ));
        m_contentItem->setWidth(width() - 2 * DockStyle::instance()->widgetPadding());
        m_contentItem->setHeight(height()
                                 - (2 * DockStyle::instance()->widgetPadding())
                                 - (m_showHeader ? _header->height() : 0));
    }

    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

QQuickItem *DockWidget::contentItem() const
{
    return m_contentItem;
}

QString DockWidget::title() const
{
    return m_title;
}
