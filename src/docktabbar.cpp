#include "dockstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"

#include <QPainter>
#include <QApplication>
#include <QStyle>

QStringList DockTabBar::tabs() const
{
    QStringList l;
    for (auto t : _tabs)
        l.append(t->title());
    return l;
}

int DockTabBar::currentIndex() const
{
    return m_currentIndex;
}

void DockTabBar::setCurrentIndex(int currentIndex)
{
    if (m_currentIndex == currentIndex)
        return;

    if (!_tabs.count()) {
        m_currentIndex = -1;
        emit currentIndexChanged(m_currentIndex);
        return;
    }

    if (m_currentIndex >= 0 && m_currentIndex < _tabs.count() - 1)
        _tabs.at(m_currentIndex)->setChecked(false);

    m_currentIndex = qBound(0, currentIndex, _tabs.count() - 1);
    _tabs.at(m_currentIndex)->setChecked(true);

    emit currentIndexChanged(m_currentIndex);
}

DockTabBar::DockTabBar(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_currentIndex{-1}
{
}

int DockTabBar::addTab(const QString &name)
{
    auto t = new DockTabButton{name, this};
    t->setWidth(QFontMetrics(DockStyle::instance()->defaultFont()).horizontalAdvance(name) + 15);
    connect(t, &DockTabButton::clicked, this, &DockTabBar::tabButton_clicked);
    _tabs.append(t);
    return _tabs.count() - 1;
}

void DockTabBar::removeTab(int index)
{
    if (index < 0 || index >= _tabs.count())
        return;

    auto tab = _tabs.at(index);
    tab->setParentItem(nullptr);
    tab->deleteLater();
    _tabs.removeAt(index);

    if (index >= _tabs.count())
        setCurrentIndex(_tabs.count() - 1);

    geometryChanged(QRect(), QRect());
}

void DockTabBar::paint(QPainter *painter)
{
    //    painter->fillRect(clipRect(), Qt::green);
    //    qreal p{0};
    //    for (auto t : _tabs)
    //        drawTab(painter, &p, t->title, 0);
    DockStyle::instance()->paintTabBar(painter, this);
}

void DockTabBar::drawTab(QPainter *p,
                         qreal *pos,
                         const QString &title,
                         int status)
{
    p->setPen(Qt::black);
    p->setBrush(Qt::yellow);
    QRectF rc(*pos, 0, p->fontMetrics().horizontalAdvance(title) + 20, height());
    p->drawRoundedRect(rc, 5, 5);
    p->drawText(rc, Qt::AlignCenter, title);
    *pos += rc.width() + 2;
}

void DockTabBar::tabButton_clicked()
{
    auto btn = qobject_cast<DockTabButton *>(sender());
    if (!btn)
        return;

    setCurrentIndex(_tabs.indexOf(btn));
}

void DockTabBar::geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry)
{
    qreal xx{0};
    for (auto btn : _tabs) {
        btn->setHeight(height());
        btn->setY(0);
        btn->setX(xx);
        xx += btn->width();
    }
}
