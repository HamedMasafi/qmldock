#include "style/abstractstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>

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

    if (currentIndex > _tabs.count() - 1) {
        qDebug() << "Invalid index";
        return;
    }
    if (currentIndex == -1) {
        for (auto &t : _tabs)
            t->setChecked(false);
        m_currentIndex = -1;
        emit currentIndexChanged(m_currentIndex);
        return;
    }

    if (m_currentIndex != -1 && m_currentIndex <= _tabs.count() - 1)
        _tabs.at(m_currentIndex)->setChecked(false);

        m_currentIndex = currentIndex;
    _tabs.at(m_currentIndex)->setChecked(true);

    emit currentIndexChanged(m_currentIndex);
}

DockTabBar::DockTabBar(QQuickItem *parent) : QQuickPaintedItem(parent)
      , m_currentIndex{-1}, _edge{Qt::TopEdge}
{
}

int DockTabBar::addTab(const QString &name)
{
    auto t = new DockTabButton{name, this};
    t->setFitSize(QFontMetrics(dockStyle->font()).horizontalAdvance(name) + 15);
    t->setY(0);
    connect(t, &DockTabButton::clicked, this, &DockTabBar::tabButton_clicked);
    _tabs.append(t);
    reorderTabs();
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
        setCurrentIndex(qBound(0, m_currentIndex, _tabs.count() - 1));

    reorderTabs();
}

void DockTabBar::paint(QPainter *painter)
{
    dockStyle->paintTabBar(painter, this);
}

void DockTabBar::reorderTabs()
{
    qreal xx = _edge == Qt::LeftEdge ? width() - 1 : 0;
    for (auto btn : _tabs) {
        btn->setY(0);
        btn->setHeight(height());
        btn->setWidth(btn->fitSize());
        if (_edge == Qt::LeftEdge) {
            btn->setX(xx - btn->width());
            xx -= btn->width();
        } else {
            btn->setX(xx);
            xx += btn->width();
        }
    }
}

void DockTabBar::tabButton_clicked()
{
    auto btn = qobject_cast<DockTabButton *>(sender());
    if (!btn)
        return;

    auto index = _tabs.indexOf(btn);
    if (m_currentIndex == index)
        return;
    setCurrentIndex(index);
}

void DockTabBar::geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry)
{
    reorderTabs();
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

Qt::Edge DockTabBar::edge() const
{
    return _edge;
}

void DockTabBar::setEdge(const Qt::Edge &edge)
{
    _edge = edge;
    reorderTabs();
    update();
}
