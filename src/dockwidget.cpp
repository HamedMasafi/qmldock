#include "dockwidget.h"
#include "dockwidgetheader.h"
#include "debugrect.h"
#include "dockwidgetbackground.h"
#include "dockwindow.h"

#include <QDebug>
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

DockWidget::DockWidget(QQuickItem *parent) : QQuickPage(parent)
      , _dockGroup{nullptr}, _originalSize{200, 200}, isDetached{false}
{
    _header = new DockWidgetHeader(this);
    setHeader(_header);
    _header->setPosition(QPointF(0, 0));
    _header->setSize(QSizeF(width(), 30));
    _header->setVisible(true);
    _header->setZ(999);

    setBackground(new DockWidgetBackground(this));
//    setBackground(new DebugRect(Qt::green, this));
    contentItem()->setClip(true);

    setTopPadding(1);
    setRightPadding(1);
    setBottomPadding(1);
    setLeftPadding(1);

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

    connect(this,
            &QQuickPage::titleChanged,
            this,
            &DockWidget::this_titleChanged);
    setClip(true);
    setSize(QSizeF(200, 200));
    setFiltersChildMouseEvents(true);
//    setAcceptedMouseButtons(Qt::LeftButton);
}

void DockWidget::detach()
{
    dockWindow = new DockWindow(this);
    //    window->setFlag(Qt::Tool);
    dockWindow->setTitle(title());
    isDetached = true;
//    setSize(QSizeF(200, 200));

    //    for (auto& i: contentItem()->childItems()) {
    ////        if (i == header() || i->inherits("QQuickContentItem"))
    ////            continue;

    //        qDebug() << i << "moved to window";
    //        i->setParentItem(window->contentItem());
    //        i->setVisible(true);
    //    }
//    setParentItem(window->contentItem());
//    setPosition(QPointF(0, 0));
    dockWindow->show();
//    setVisible(false);
}

void DockWidget::close()
{
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

void DockWidget::this_titleChanged()
{
    _header->setTitle(title());
}

bool DockWidget::childMouseEventFilter(QQuickItem *item, QEvent *e)
{
    static QPointF _lastMousePos;
    static QPointF _lastChildPos;
    static bool _moveEmitted = false;

    if (item != header())
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

    QQuickPage::geometryChanged(newGeometry, oldGeometry);
}
