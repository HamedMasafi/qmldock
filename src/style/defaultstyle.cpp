#include "defaultstyle.h"

#include "dockcontainer.h"
#include "dockarea.h"
#include "dockgroupresizehandler.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "dockwidget.h"
#include "dockwidgetbackground.h"
#include "dockwidgetheader.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>
#include <docktabbararrorbutton.h>

#ifdef Q_OS_WIN
#   define STR(x) QString::fromUtf16(u ## x)
#else
#   define STR(x) QString::fromUtf8(x)
#endif

DefaultStyle::DefaultStyle(QObject *parent)
    : QObject(parent), AbstractStyle(), m_backgroundColor("#e4e4e4"),
      m_mainColor("#4fc1e9"), m_borderColor("#4fc1e9"),
      m_widgetColor(Qt::white),
      m_hoverColor(220, 220, 220), m_pressColor(200, 200, 200),
      m_tabAreaColor(Qt::white), m_textColor(Qt::black)
      , m_activeTextColor(Qt::black)
{
}

void DefaultStyle::paintDockContainer(QPainter *p, DockContainer *item)
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
    return 25;
}

qreal DefaultStyle::resizeHandleSize() const
{
    return 8;
}

qreal DefaultStyle::widgetPadding(DockWidget *widget) const
{
    if (widget->dockArea() && widget->dockArea()->displayType() == Dock::TabbedView)
        return 0;
    return 3;
}

void DefaultStyle::setMainColor(QColor mainColor)
{
    if (m_mainColor == mainColor)
        return;

    m_mainColor = mainColor;
    Q_EMIT mainColorChanged(m_mainColor);
}

void DefaultStyle::setBorderColor(QColor borderColor)
{
    if (m_borderColor == borderColor)
        return;

    m_borderColor = borderColor;
    Q_EMIT borderColorChanged(m_borderColor);
}

void DefaultStyle::setHoverColor(QColor hoverColor)
{
    if (m_hoverColor == hoverColor)
        return;

    m_hoverColor = hoverColor;
    Q_EMIT hoverColorChanged(m_hoverColor);
}

void DefaultStyle::setPressColor(QColor pressColor)
{
    if (m_pressColor == pressColor)
        return;

    m_pressColor = pressColor;
    Q_EMIT pressColorChanged(m_pressColor);
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

QColor DefaultStyle::widgetColor() const
{
    return m_widgetColor;
}

QColor DefaultStyle::activeTextColor() const
{
    return m_activeTextColor;
}

void DefaultStyle::setTabAreaColor(QColor tabAreaColor)
{
    if (m_tabAreaColor == tabAreaColor)
        return;

    m_tabAreaColor = tabAreaColor;
    Q_EMIT tabAreaColorChanged(m_tabAreaColor);
}

void DefaultStyle::setTextColor(QColor textColor)
{
    if (m_textColor == textColor)
        return;

    m_textColor = textColor;
    Q_EMIT textColorChanged(m_textColor);
}

void DefaultStyle::setBackgroundColor(QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    Q_EMIT backgroundColorChanged(m_backgroundColor);
}

void DefaultStyle::setWidgetColor(QColor widgetColor)
{
    if (m_widgetColor == widgetColor)
        return;

    m_widgetColor = widgetColor;
    Q_EMIT widgetColorChanged(m_widgetColor);
}

void DefaultStyle::setActiveTextColor(QColor activeTextColor)
{
    if (m_activeTextColor == activeTextColor)
        return;

    m_activeTextColor = activeTextColor;
    Q_EMIT activeTextColorChanged(m_activeTextColor);
}

QString DefaultStyle::iconToStr(Dock::Icon icon) const
{
    switch (icon) {
    case Dock::CloseIcon:
        return "c";
    case Dock::PinIcon:
        return "p";
    case Dock::PopoutIcon:
        return "o";

    case Dock::CenterDropIcon:
        return "C";
    case Dock::LeftDropIcon:
        return "l";
    case Dock::RightDropIcon:
        return "r";
    case Dock::UpDropIcon:
        return "u";
    case Dock::DownDropIcon:
        return "d";

    case Dock::LeftArrowIcon:
        return "L";
    case Dock::RightArrowIcon:
        return "R";
    case Dock::UpArrowIcon:
        return "U";
    case Dock::DownArrowIcon:
        return "D";
    };
    return QString();
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
        p->setPen(m_activeTextColor);
    } else {
        p->setPen(QColor(110, 110, 250));
        p->setOpacity(.8);
    }
    p->fillRect(rc, Qt::lightGray);

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
//    p->fillRect(item->clipRect(), Qt::red);
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
        rc.setBottom(rc.bottom());
        p->fillRect(rc, m_backgroundColor);
        if (item->parentTabBar()->edge() == Qt::BottomEdge)
            drawLineOnEdge(p, item, Qt::TopEdge);
        else
            drawLineOnEdge(p, item, Qt::BottomEdge);
        p->setPen(m_textColor);
        break;

    case Dock::Hovered:
        p->setPen(m_borderColor);
        rc.setBottom(rc.bottom());
        p->setBrush(hoverColor());
        p->fillRect(rc, hoverColor());
        if (item->parentTabBar()->edge() == Qt::BottomEdge)
            drawLineOnEdge(p, item, Qt::TopEdge);
        else
            drawLineOnEdge(p, item, Qt::BottomEdge);
        p->setPen(m_textColor);
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
        p->setPen(m_activeTextColor);
        break;
    }

    if (item->showCloseButton())
        rc.setWidth(rc.width() - 20);

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
        p->setBrush(m_hoverColor);
        p->drawEllipse(0, 0, 15, 15);
        break;

    case Dock::Pressed:
    case Dock::Checked:
        p->setBrush(m_pressColor);
        p->drawEllipse(0, 0, 15, 15);
        break;
    }
    p->setPen(m_textColor);
    p->setFont(QFont("dock_font_default"));
    p->drawText(item->clipRect(), Qt::AlignCenter, iconToStr(item->icon()));
}

void DefaultStyle::paintResizeHandler(QPainter *p,
                                      DockAreaResizeHandler *item,
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

void DefaultStyle::paintDockArea(QPainter *p, DockArea *item)
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
            rc.setX(rc.x() + resizeHandleSize());
            break;
        case Dock::Bottom:
            rc.setY(rc.y() + resizeHandleSize());
            break;
        default:
            break;
        }
        p->fillRect(rc, m_tabAreaColor);
        p->setPen(m_borderColor);
        p->drawRect(rc);
    }
}

void DefaultStyle::paintDockWidget(QPainter *p, DockWidget *item)
{
    //    if (item->isActive())
    //        p->fillRect(item->clipRect(), Qt::red);
    //    else

    qreal a{0};
    if ((item->dockArea() && item->dockArea()->displayType() != Dock::TabbedView)
        || item->area() == Dock::Float
        || item->area() == Dock::Detached) {
        p->fillRect(item->clipRect(), m_borderColor);
        a = 1;
    }
    p->fillRect(item->clipRect().adjusted(a, a, -a, -a), m_widgetColor);
}

void DefaultStyle::paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item)
{
    //    p->fillRect(item->clipRect(), mainColor());
    QBrush b(m_mainColor);
    b.setStyle(Qt::Dense6Pattern);
    auto tw = QFontMetrics(font()).horizontalAdvance(item->title());
    int padding{20};
    if (item->closeButtonVisible())
        padding += 20;
    if (item->pinButtonVisible())
        padding += 20;
    p->fillRect(10 + tw, 10, item->width() - padding - tw, item->height() - 20, b);
    p->setPen(m_textColor);
    p->drawText(8,
                0,
                item->width() - padding,
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

void DefaultStyle::paintDockTabBarArrowButton(QPainter *p,
                                              DockTabBarArrorButton *item, Dock::ButtonStatus status)
{
    p->setFont(QFont("dock_font_default"));

    switch (status) {
    case Dock::Normal:
        p->fillRect(item->clipRect(), m_backgroundColor);
        break;

    case Dock::Hovered:
        p->setBrush(m_hoverColor);
        p->fillRect(item->clipRect(), m_hoverColor);
        break;

    case Dock::Pressed:
    case Dock::Checked:
        p->setBrush(m_pressColor);
        p->fillRect(item->clipRect(), m_pressColor);
        break;
    }
    p->setPen(m_textColor);
    p->setFont(QFont("dock_font_default"));
    p->drawText(item->clipRect(), Qt::AlignCenter, iconToStr(item->icon()));
}

void DefaultStyle::drawCircle(QPainter *painter,
                              const QPointF &center,
                              bool hover)
{
    Q_UNUSED(hover)
    painter->drawEllipse(center, 2, 2);
}
