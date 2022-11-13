#ifndef DOCKTABBAR_H
#define DOCKTABBAR_H

#include <QQuickPaintedItem>

class DockWidget;
class DockTabButton;
class DockTabBarArrorButton;

class DockTabBar : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

  Qt::Edge _edge;
  int m_currentIndex;
  qreal _tabsStartPos;
  qreal _tabsSize;

  QList<DockTabButton *> _tabs;
  DockTabBarArrorButton *_prevButton;
  DockTabBarArrorButton *_nextButton;

public:
  DockTabBar(QQuickItem *parent = nullptr);
  void paint(QPainter *painter);

  QStringList tabs() const;
  int currentIndex() const;

  Qt::Edge edge() const;
  void setEdge(const Qt::Edge &edge);

private Q_SLOTS:
  void prevButton_clicked();
  void nextButton_clicked();
  void tabButton_clicked();
  void tabButton_closeButtonClicked();

public Q_SLOTS:
  int addTab(DockWidget *widget);
  void removeTab(int index);
  void setCurrentIndex(int currentIndex);

Q_SIGNALS:
  void tabClicked(int index);
  void closeButtonClicked(int index);
  void currentIndexChanged(int currentIndex);

private:
  void reorderTabs();
  void calculateTabsSize();

protected:
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry);
  void updatePolish();
};

#endif // DOCKTABBAR_H
