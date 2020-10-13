#include "dockarea.h"
#include "dockgroup.h"
#include "dockgroupresizehandler.h"
#include "dockstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "dockwidget.h"
#include "dockwidgetbackground.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>

DockStyle::DockStyle(QObject *parent) : QObject(parent)
    , m_mainColor("#4fc1e9")
    , m_borderColor(30, 30, 30)
    , m_hoverColor(220, 220, 220)
    , m_pressColor(200, 200, 200)
{

}

void DockStyle::paintDockArea(QPainter *p, DockArea *item)
{
    Q_UNUSED(p)
    Q_UNUSED(item)
//    p->fillRect(item->clipRect(), mainColor());
}

QFont DockStyle::defaultFont() const
{
    return QFont();
}

qreal DockStyle::tabBarHeight() const
{
    return 30;
}

qreal DockStyle::tabBarButtonHeight() const
{
    return 25;
}

qreal DockStyle::tabBarButtonY() const
{
    return 4;
}

qreal DockStyle::tabMargin() const
{
    return 6;
}

qreal DockStyle::resizeHandleSize() const
{
    return 8;
}

qreal DockStyle::widgetPadding()
{
    return 3;
}

void DockStyle::setMainColor(QColor mainColor)
{
    if (m_mainColor == mainColor)
            return;

        m_mainColor = mainColor;
        emit mainColorChanged(m_mainColor);
}

void DockStyle::setBorderColor(QColor borderColor)
{
    if (m_borderColor == borderColor)
            return;

        m_borderColor = borderColor;
        emit borderColorChanged(m_borderColor);
}

void DockStyle::setHoverColor(QColor hoverColor)
{
    if (m_hoverColor == hoverColor)
            return;

        m_hoverColor = hoverColor;
        emit hoverColorChanged(m_hoverColor);
}

void DockStyle::setPressColor(QColor pressColor)
{
    if (m_pressColor == pressColor)
            return;

        m_pressColor = pressColor;
        emit pressColorChanged(m_pressColor);
}

QColor DockStyle::mainColor() const
{
    return m_mainColor;
}

QColor DockStyle::borderColor() const
{
    return m_borderColor;
}

QColor DockStyle::hoverColor() const
{
    return m_hoverColor;
}

QColor DockStyle::pressColor() const
{
    return m_pressColor;
}


void DockStyle::paintDropButton(QPainter *p, Dock::Area area)
{
    Q_UNUSED(p)
    Q_UNUSED(area)
}

void DockStyle::paintTabBar(QPainter *p, DockTabBar *item)
{
//    p->fillRect(item->clipRect(), Qt::magenta);

//    qreal pos{0};
//    for (auto t : item->tabs())
//        drawTab(p, &pos, t, 0);
    p->drawLine(0, 29, item->width() - 1, 29);
}

void DockStyle::paintTabButton(QPainter *p,
                               DockTabButton *item,
                               Dock::ButtonStatus status)
{
    QRectF rc;

//    QTransform t;
//    t.rotate(90);
//    p->setTransform(t);
    switch (status) {
    case Dock::Normal:
        p->setBrush(pressColor());
        rc = QRectF(1, 4, item->width() - 2, item->height() - 5);
        p->drawRect(rc);
        break;

    case Dock::Hovered:
        p->setBrush(hoverColor());
        rc = QRectF(1, 4, item->width() - 2, item->height() - 5);
        p->drawRect(rc);
        break;

    case Dock::Checked:
    case Dock::Pressed:
        p->setPen(borderColor());
        p->setBrush(mainColor());
        rc = QRectF(0, 0, item->width() - 1, item->height());
        p->fillRect(rc, Qt::white);
        p->drawLine(rc.topLeft(), rc.topRight());
        p->drawLine(rc.topLeft(), rc.bottomLeft());
        p->drawLine(rc.topRight(), rc.bottomRight());
        break;
    }

    p->drawText(rc, Qt::AlignCenter, item->title());
}

void DockStyle::paintWidgetButton(QPainter *p,
                                 DockWidgetHeaderButton *item,
                                 Dock::ButtonStatus status)
{
    switch (status) {
    case Dock::Normal:
        break;

    case Dock::Hovered:
        p->setBrush(QColor(220, 220, 220));
        p->drawEllipse(0, 0, 15, 15);
        break;

    case Dock::Pressed:
    case Dock::Checked:
        p->setBrush(QColor(200, 200, 200));
        p->drawEllipse(0, 0, 15, 15);
        break;
    }
    p->setFont(QFont("icons"));
    p->drawText(item->clipRect(), Qt::AlignCenter, item->icon());
}

void DockStyle::paintResizeHandler(QPainter *p, DockGroupResizeHandler *item, Dock::ButtonStatus status)
{
//    p->fillRect(item->clipRect(), Qt::darkYellow);
    p->setBrush(status == Dock::Hovered ? Qt::gray : Qt::lightGray);

    auto center = item->clipRect().center();
    drawCircle(p, center);

    switch (item->orientation()) {
    case Qt::Horizontal:
        center.setX(center.x() - 10);
        drawCircle(p, center);

        center.setX(center.x() + 20);
        drawCircle(p, center);
        break;

    case Qt::Vertical:
        center.setY(center.y() - 10);
        drawCircle(p, center);

        center.setY(center.y() + 20);
        drawCircle(p, center);
        break;
    }
}

void DockStyle::paintGroup(QPainter *p, DockGroup *item)
{
    Q_UNUSED(p)
    Q_UNUSED(item)
//    if (item->displayType() == Dock::TabbedView && item->widgets().count() && item->showTabBar()) {
//        p->setPen(borderColor());
//        p->fillRect(0, 29, item->width() - 1, item->height() - 30, Qt::white);
//        p->drawLine(0, 29, 0, item->height() - 1);
//        p->drawLine(item->width() - 1, 29, item->width() - 1, item->height() - 1);
//        p->drawLine(0, item->height() - 1, item->width() - 1, item->height() - 1);
//    }
}

void DockStyle::paintDockWidget(QPainter *p, DockWidget *item)
{
    p->setBrush(Qt::white);
    /*QPen pn(mainColor());
    pn.setWidth(widgetPadding() * 2);
    p->setPen(pn);
    p->drawRect(0, 0, item->width() - 1, item->height() - 1);*/
    p->fillRect(item->clipRect(), mainColor());
    if (item->area() == Dock::Float || item->area() == Dock::Detached)
        p->fillRect(item->clipRect().adjusted(10, 10, -10, -10), Qt::white);
    else
        p->fillRect(item->clipRect().adjusted(3, 3, -3, -3), Qt::white);
}

void DockStyle::paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item)
{
    p->fillRect(item->clipRect(), mainColor());
    p->drawText(8, 0, item->width() - 60, item->height(), Qt::AlignVCenter, item->title());
//    p->drawLine(5, item->height() - 1, item->width() - 5, item->height() - 1);
}

void DockStyle::paintDockWidgetBackground(QPainter *p, DockWidgetBackground *item)
{
    p->setPen(Qt::gray);
    p->setBrush(Qt::white);
    p->drawRect(0, 0, item->width() - 1, item->height() - 1);
}

void DockStyle::drawCircle(QPainter *painter, const QPointF &center, bool hover)
{
    Q_UNUSED(hover)
    painter->drawEllipse(center, 2, 2);
}

DockStyle *DockStyle::instance()
{
    static DockStyle *i = new DockStyle;
    return i;
}

void DockStyle::drawTab(QPainter *p,
                         qreal *pos,
                         const QString &title,
                         int status)
{
    Q_UNUSED(p)
    Q_UNUSED(pos)
    Q_UNUSED(title)
    Q_UNUSED(status)
//    p->setPen(Qt::black);
//    p->setBrush(Qt::yellow);
//    QRectF rc(*pos, 0, p->fontMetrics().horizontalAdvance(title) + 20, height());
//    p->drawRoundedRect(rc, 5, 5);
//    p->drawText(rc, Qt::AlignCenter, title);
//    *pos += rc.width() + 2;
}
