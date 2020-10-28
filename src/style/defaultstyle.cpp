#include "defaultstyle.h"

#include "dockarea.h"
#include "dockgroup.h"
#include "dockgroupresizehandler.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "dockwidget.h"
#include "dockwidgetbackground.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>

#ifdef Q_OS_WIN
#   define STR(x) QString::fromUtf16(u ## x)
#else
#   define STR(x) QString::fromUtf8(x)
#endif

DefaultStyle::DefaultStyle(QObject *parent)
    : QObject(parent), AbstractStyle(), m_backgroundColor("#e4e4e4"),
      m_mainColor("#4fc1e9"), m_borderColor(30, 30, 30),
      m_hoverColor(220, 220, 220), m_pressColor(200, 200, 200),
      m_tabAreaColor(Qt::white), m_textColor(Qt::black)
{
}

void DefaultStyle::paintDockArea(QPainter *p, DockArea *item)
{
    Q_UNUSED(p)
    Q_UNUSED(item)
    p->fillRect(item->clipRect(), m_backgroundColor);
}

QFont DefaultStyle::font() const
{
    return QFont();
}

qreal DefaultStyle::tabBarSize() const
{
    return 29;
}

qreal DefaultStyle::dropButtonSize() const
{
    return 30;
}

qreal DefaultStyle::dropButtonSpace() const
{
    return 15;
}

qreal DefaultStyle::resizeHandleSize() const
{
    return 8;
}

qreal DefaultStyle::widgetPadding() const
{
    return 3;
}

void DefaultStyle::setMainColor(QColor mainColor)
{
    if (m_mainColor == mainColor)
        return;

    m_mainColor = mainColor;
    emit mainColorChanged(m_mainColor);
}

void DefaultStyle::setBorderColor(QColor borderColor)
{
    if (m_borderColor == borderColor)
        return;

    m_borderColor = borderColor;
    emit borderColorChanged(m_borderColor);
}

void DefaultStyle::setHoverColor(QColor hoverColor)
{
    if (m_hoverColor == hoverColor)
        return;

    m_hoverColor = hoverColor;
    emit hoverColorChanged(m_hoverColor);
}

void DefaultStyle::setPressColor(QColor pressColor)
{
    if (m_pressColor == pressColor)
        return;

    m_pressColor = pressColor;
    emit pressColorChanged(m_pressColor);
}

qreal DefaultStyle::widgetResizePadding() const
{
    return 6;
}

qreal DefaultStyle::widgetTabPadding() const
{
    return 5;
}

QColor DefaultStyle::tabAreaColor() const
{
    return m_tabAreaColor;
}

QColor DefaultStyle::textColor() const
{
    return m_textColor;
}

QColor DefaultStyle::backgroundColor() const
{
    return m_backgroundColor;
}

void DefaultStyle::setTabAreaColor(QColor tabAreaColor)
{
    if (m_tabAreaColor == tabAreaColor)
        return;

    m_tabAreaColor = tabAreaColor;
    emit tabAreaColorChanged(m_tabAreaColor);
}

void DefaultStyle::setTextColor(QColor textColor)
{
    if (m_textColor == textColor)
        return;

    m_textColor = textColor;
    emit textColorChanged(m_textColor);
}

void DefaultStyle::setBackgroundColor(QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(m_backgroundColor);
}

void DefaultStyle::drawLineOnEdge(QPainter *p,
                                  QQuickItem *item,
                                  Qt::Edge edge) const
{
    switch (edge) {
    case Qt::TopEdge:
        p->drawLine(0, 0, item->width() - 1, 0);
        break;
    case Qt::RightEdge:
        p->drawLine(item->width() - 1, 0, item->width() - 1, item->height() - 1);
        break;
    case Qt::LeftEdge:
        p->drawLine(0, 0, 0, item->height() - 1);
        break;
    case Qt::BottomEdge:
        p->drawLine(0, item->height() - 1, item->width() - 1, item->height() - 1);
        break;
    }
}

QColor DefaultStyle::mainColor() const
{
    return m_mainColor;
}

QColor DefaultStyle::borderColor() const
{
    return m_borderColor;
}

QColor DefaultStyle::hoverColor() const
{
    return m_hoverColor;
}

QColor DefaultStyle::pressColor() const
{
    return m_pressColor;
}

void DefaultStyle::paintDropButton(QPainter *p, Dock::Area area, const QRectF &rc, bool hover)
{
    p->setFont(QFont("dock_font_default", 24));
    if (hover) {
        p->setPen(Qt::blue);
    } else {
        p->setPen(Qt::black);
        p->setOpacity(.2);
        p->fillRect(rc, Qt::blue);
    }

    switch (area) {
    case Dock::Left:
        p->drawText(rc, Qt::AlignCenter, "l");
        break;
    case Dock::Right:
        p->drawText(rc, Qt::AlignCenter, "r");
        break;
    case Dock::Top:
        p->drawText(rc, Qt::AlignCenter, "u");
        break;
    case Dock::Bottom:
        p->drawText(rc, Qt::AlignCenter, "d");
        break;
    case Dock::Center:
        p->drawText(rc, Qt::AlignCenter, "C");
        break;
    default:
        break;
    }
    p->setOpacity(1);
}

void DefaultStyle::paintTabBar(QPainter *p, DockTabBar *item)
{
    Q_UNUSED(p)
    Q_UNUSED(item)
}

void DefaultStyle::paintTabButton(QPainter *p,
                                  DockTabButton *item,
                                  Dock::ButtonStatus status)
{
    QRectF rc(0, 0, item->width() - 1, item->height() - 1);
    p->setPen(m_borderColor);

    switch (status) {
    case Dock::Normal:
        p->setPen(m_borderColor);
        rc.setBottom(rc.bottom() - 1);
        p->fillRect(rc, m_backgroundColor);
        if (item->parentTabBar()->edge() == Qt::BottomEdge)
            drawLineOnEdge(p, item, Qt::TopEdge);
        else
            drawLineOnEdge(p, item, Qt::BottomEdge);
        break;

    case Dock::Hovered:
        p->setPen(m_borderColor);
        rc.setBottom(rc.bottom() - 1);
        p->setBrush(hoverColor());
        p->fillRect(rc, hoverColor());
        if (item->parentTabBar()->edge() == Qt::BottomEdge)
            drawLineOnEdge(p, item, Qt::TopEdge);
        else
            drawLineOnEdge(p, item, Qt::BottomEdge);
        break;

    case Dock::Checked:
    case Dock::Pressed:
        p->fillRect(rc, m_tabAreaColor);

        switch (item->parentTabBar()->edge()) {
        case Qt::TopEdge:
        case Qt::LeftEdge:
        case Qt::RightEdge:
            p->setPen(m_tabAreaColor);
            drawLineOnEdge(p, item, Qt::BottomEdge);

            p->setPen(m_borderColor);
            drawLineOnEdge(p, item, Qt::TopEdge);
            drawLineOnEdge(p, item, Qt::LeftEdge);
            drawLineOnEdge(p, item, Qt::RightEdge);
            break;
        case Qt::BottomEdge:
            p->setPen(m_tabAreaColor);
            drawLineOnEdge(p, item, Qt::TopEdge);

            p->setPen(m_borderColor);
            drawLineOnEdge(p, item, Qt::LeftEdge);
            drawLineOnEdge(p, item, Qt::RightEdge);
            drawLineOnEdge(p, item, Qt::BottomEdge);
            break;
        }
        break;
    }

    p->setPen(m_textColor);
    p->drawText(rc, Qt::AlignCenter, item->title());
}

void DefaultStyle::paintWidgetButton(QPainter *p,
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

void DefaultStyle::paintResizeHandler(QPainter *p,
                                      DockGroupResizeHandler *item,
                                      Dock::ButtonStatus status)
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

void DefaultStyle::paintDockGroup(QPainter *p, DockGroup *item)
{
    Q_UNUSED(p)
    Q_UNUSED(item)
    if (item->displayType() == Dock::TabbedView && item->widgets().count()) {
        QRectF rc;
        rc.setTop(item->tabPosition() == Qt::TopEdge ? tabBarSize() - 1 : 0);
        rc.setLeft(item->tabPosition() == Qt::LeftEdge ? tabBarSize() - 1 : 0);
        rc.setRight(item->width()
                    - (item->tabPosition() == Qt::RightEdge ? tabBarSize() : 1));
        rc.setBottom(
            item->height()
            - (item->tabPosition() == Qt::BottomEdge ? tabBarSize() : 1));

        switch (item->area()) {
        case Dock::Left:
            rc.setRight(rc.right() - resizeHandleSize());
            break;
        case Dock::Top:
            rc.setBottom(rc.bottom() - resizeHandleSize());
            break;
        case Dock::Right:
            rc.setX(rc.x() - resizeHandleSize());
            break;
        case Dock::Bottom:
            rc.setY(rc.y() - resizeHandleSize());
            break;
        default:
            break;
        }
        p->fillRect(rc, m_tabAreaColor);
        p->setPen(m_borderColor);
        p->drawRect(rc);
        //        p->setPen(borderColor());
        //        p->fillRect(0, 29, item->width() - 1, item->height() - 30, Qt::white);
        //        p->drawLine(0, 29, 0, item->height() - 1);
        //        p->drawLine(item->width() - 1, 29, item->width() - 1, item->height() - 1);
        //        p->drawLine(0, item->height() - 1, item->width() - 1, item->height() - 1);
    }
}

void DefaultStyle::paintDockWidget(QPainter *p, DockWidget *item)
{
    p->setBrush(Qt::white);
    /*QPen pn(mainColor());
    pn.setWidth(widgetPadding() * 2);
    p->setPen(pn);
    p->drawRect(0, 0, item->width() - 1, item->height() - 1);*/
    p->fillRect(item->clipRect(), mainColor());
    qreal a;
    if (item->area() == Dock::Float || item->area() == Dock::Detached)
        a = 10;
    else
        a = 1;
    p->fillRect(item->clipRect().adjusted(a, a, -a, -a), Qt::white);
}

void DefaultStyle::paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item)
{
    //    p->fillRect(item->clipRect(), mainColor());
    QBrush b(m_mainColor);
    b.setStyle(Qt::Dense6Pattern);
    auto tw = QFontMetrics(font()).horizontalAdvance(item->title());
    p->fillRect(10 + tw, 10, item->width() - 30 - tw, item->height() - 20, b);
    p->setPen(m_textColor);
    p->drawText(8,
                0,
                item->width() - 60,
                item->height(),
                Qt::AlignVCenter,
                item->title());
    //    p->drawLine(5, item->height() - 1, item->width() - 5, item->height() - 1);
}

void DefaultStyle::paintDockWidgetBackground(QPainter *p,
                                             DockWidgetBackground *item)
{
    p->setPen(Qt::gray);
    p->setBrush(Qt::white);
    p->drawRect(0, 0, item->width() - 1, item->height() - 1);
}

void DefaultStyle::drawCircle(QPainter *painter,
                              const QPointF &center,
                              bool hover)
{
    Q_UNUSED(hover)
    painter->drawEllipse(center, 2, 2);
}

void DefaultStyle::drawTab(QPainter *p,
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
