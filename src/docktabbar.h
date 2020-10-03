#ifndef DOCKTABBAR_H
#define DOCKTABBAR_H

#include <QQuickPaintedItem>

class DockTabButton;
class DockTabBar : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

    QList<DockTabButton *> _tabs;


public:
    DockTabBar(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    QStringList tabs() const;


    int currentIndex() const;

public slots:
    int addTab(const QString &name);
    void removeTab(int index);
    void setCurrentIndex(int currentIndex);

signals:
    void tabClicked(int index);

    void currentIndexChanged(int currentIndex);

private:
    void drawTab(QPainter *p, qreal *pos, const QString &title, int status);

    int m_currentIndex;

private slots:
    void tabButton_clicked();
protected:

    // QQuickItem interface
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
};

#endif // DOCKTABBAR_H
