#ifndef DOCKTABBAR_H
#define DOCKTABBAR_H

#include <QQuickPaintedItem>

class DockTabButton;
class DockTabBarArrorButton;
class DockTabBar : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

    QList<DockTabButton *> _tabs;
    Qt::Edge _edge;
    DockTabBarArrorButton *_prevButton;
    DockTabBarArrorButton *_nextButton;
    qreal _tabsStartPos;
    qreal _tabsSize;

public:
    DockTabBar(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    QStringList tabs() const;


    int currentIndex() const;

    Qt::Edge edge() const;
    void setEdge(const Qt::Edge &edge);
private slots:
    void prevButton_clicked();
    void nextButton_clicked();
    void tabButton_clicked();

public slots:
    int addTab(const QString &name);
    void removeTab(int index);
    void setCurrentIndex(int currentIndex);

signals:
    void tabClicked(int index);

    void currentIndexChanged(int currentIndex);

private:
    void reorderTabs();
    void calculateTabsSize();
    int m_currentIndex;

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void updatePolish();

};

#endif // DOCKTABBAR_H
