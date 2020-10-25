#ifndef DOCKTABBUTTON_H
#define DOCKTABBUTTON_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockTabBar;
class DockTabButton : public QQuickPaintedItem
{
    Q_OBJECT
    QString _title;
    Dock::ButtonStatus _status;
    DockTabBar *_parentTabBar;
    qreal _fitSize;

public:
    DockTabButton(QString title, DockTabBar *parent = nullptr);
    void paint(QPainter *painter);

signals:
    void clicked();

    // QQuickPaintedItem interface
public:
    QString title() const;
    void setTitle(const QString &title);
    void setChecked(bool checked);

    // QQuickItem interface
    DockTabBar *parentTabBar() const;

    qreal fitSize() const;
    void setFitSize(const qreal &fitSize);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
};

#endif // DOCKTABBUTTON_H
