#include "style/abstractstyle.h"
#include "docktabbar.h"
#include "docktabbutton.h"
#include "docktabbararrorbutton.h"
#include "dockwidget.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>

#define tabsAreaSize (width() - _prevButton->width() - _nextButton->width())
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
    //allways do this even if m_currentIndex == currentIndex
    for (int i = 0; i < _tabs.size(); ++i)
        _tabs.at(i)->setChecked(i == currentIndex);

    if (m_currentIndex == currentIndex)
        return;

    m_currentIndex = currentIndex;

    Q_EMIT currentIndexChanged(m_currentIndex);
}

DockTabBar::DockTabBar(QQuickItem *parent)
    : QQuickPaintedItem(parent)
      , _edge{Qt::TopEdge}
      , m_currentIndex{-1}
      , _tabsStartPos{0.}
{
    AbstractStyle::registerThemableItem(this);
    setClip(true);

    _prevButton = new DockTabBarArrorButton(this);
    _prevButton->setParentItem(this);
    _prevButton->setZ(1000);
    _prevButton->setIcon(Dock::LeftArrowIcon);
    _prevButton->setY(0);
    _prevButton->setWidth(20);

    _nextButton = new DockTabBarArrorButton(this);
    _nextButton->setParentItem(this);
    _nextButton->setZ(1000);
    _nextButton->setIcon(Dock::RightArrowIcon);
    _nextButton->setY(0);
    _nextButton->setWidth(20);

    connect(_prevButton,
            &AbstractButton::clicked,
            this,
            &DockTabBar::prevButton_clicked);
    connect(_nextButton,
            &AbstractButton::clicked,
            this,
            &DockTabBar::nextButton_clicked);
}

int DockTabBar::addTab(DockWidget *widget)
{
    auto t = new DockTabButton{widget->title(), this};
    t->setFitSize(
        QFontMetrics(dockStyle->font()).horizontalAdvance(widget->title()) + 15);
    t->setY(0);
    t->setShowCloseButton(widget->closable());
    _tabsSize += t->width();
    connect(t, &DockTabButton::clicked, this, &DockTabBar::tabButton_clicked);
    connect(t, &DockTabButton::closeButtonClicked, this, &DockTabBar::tabButton_closeButtonClicked);
    connect(widget, &DockWidget::titleChanged, t, &DockTabButton::setTitle);
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
    _tabsSize -= tab->width();

    if (index >= _tabs.count())
        setCurrentIndex(qBound(0, m_currentIndex, _tabs.count() - 1));
    qDebug() << tab->title() << "removed";
    reorderTabs();
}

void DockTabBar::paint(QPainter *painter)
{
    dockStyle->paintTabBar(painter, this);
}

void DockTabBar::reorderTabs()
{
    qreal xx = (_edge == Qt::LeftEdge ? width() : 0)
        + _tabsStartPos;
    for (auto &btn : _tabs) {
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
        btn->update();
    }

    if (_tabsSize >= width() - _prevButton->width() - _nextButton->width()) {
        _nextButton->setX(width() - _nextButton->width());
        _prevButton->setX(width() - _prevButton->width() - _nextButton->width());
        _prevButton->setHeight(height() - 1);
        _nextButton->setHeight(height() - 1);
        _prevButton->setVisible(true);
        _nextButton->setVisible(true);
    } else {
        _prevButton->setVisible(false);
        _nextButton->setVisible(false);
    }
}

void DockTabBar::calculateTabsSize()
{
    _tabsSize = 0;
    for (auto btn : _tabs)
        _tabsSize += btn->width();
}

void DockTabBar::tabButton_clicked()
{
    auto btn = qobject_cast<DockTabButton *>(sender());
    if (!btn)
        return;

    auto index = _tabs.indexOf(btn);
    Q_EMIT tabClicked(index);
}

void DockTabBar::tabButton_closeButtonClicked()
{
    auto btn = qobject_cast<DockTabButton *>(sender());
    if (!btn)
        return;

    auto index = _tabs.indexOf(btn);
    Q_EMIT closeButtonClicked(index);
}

void DockTabBar::geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry)
{
    calculateTabsSize();
    if (_tabsSize + _tabsStartPos < tabsAreaSize)
        _tabsStartPos = tabsAreaSize - _tabsSize;

    if (tabsAreaSize > _tabsSize)
        _tabsStartPos = 0;

    reorderTabs();
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void DockTabBar::updatePolish()
{
    reorderTabs();
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

void DockTabBar::prevButton_clicked()
{
    _tabsStartPos = qMin(_tabsStartPos + 10, 0.);
    polish();
}

void DockTabBar::nextButton_clicked()
{
    _tabsStartPos = qMax(_tabsStartPos - 10, width() - _prevButton->width() - _nextButton->width() - _tabsSize);
    polish();
}
