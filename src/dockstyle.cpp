#include "dockgroupresizehandler.h"
#include "dockstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "dockwidgetheaderbutton.h"

#include <QPainter>

DockStyle::DockStyle(QObject *parent) : QObject(parent)
{

}

QFont DockStyle::defaultFont() const
{
    return QFont();
}

void DockStyle::paintDropButton(QPainter *p, Dock::Area area) {}

void DockStyle::paintTabBar(QPainter *p, DockTabBar *item)
{
    //    qreal pos{0};
    //    for (auto t : item->tabs())
    //        drawTab(p, &pos, t, 0);
    p->drawLine(0, item->height() - 2, item->width() - 1, item->height() - 2);
}

void DockStyle::paintTabButton(QPainter *p,
                               DockTabButton *item,
                               Dock::ButtonStatus status)
{
    QRectF rc;

    switch (status) {
    case Dock::Normal:
        rc = QRectF(1, 4, item->width() - 2, item->height() - 5);
        break;

    case Dock::Hovered:
        p->setBrush(Qt::yellow);
        rc = QRectF(1, 4, item->width() - 2, item->height() - 5);
        p->drawRect(rc);
        break;

    case Dock::Checked:
        case Dock::Pressed:
        p->setBrush(Qt::white);
        rc = QRectF(0, 0, item->width() - 1, item->height() - 1);
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
    p->drawText(item->clipRect(), Qt::AlignCenter, "B");
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

void DockStyle::drawCircle(QPainter *painter, const QPointF &center, bool hover)
{
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
//    p->setPen(Qt::black);
//    p->setBrush(Qt::yellow);
//    QRectF rc(*pos, 0, p->fontMetrics().horizontalAdvance(title) + 20, height());
//    p->drawRoundedRect(rc, 5, 5);
//    p->drawText(rc, Qt::AlignCenter, title);
//    *pos += rc.width() + 2;
}
