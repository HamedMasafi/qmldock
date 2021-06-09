#ifndef DOCKACTIVEWIDGETDIM_H
#define DOCKACTIVEWIDGETDIM_H

#include <QQuickPaintedItem>

class DockActiveWidgetDim : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit DockActiveWidgetDim(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

Q_SIGNALS:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DOCKACTIVEWIDGETDIM_H
