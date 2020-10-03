#ifndef DOCKTABBUTTON_H
#define DOCKTABBUTTON_H

#include <QQuickPaintedItem>
#include "dock.h"
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

class DockTabButton : public QQuickPaintedItem
{
    Q_OBJECT
    QString _title;
    Dock::ButtonStatus _status;

public:
    DockTabButton(QString title, QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

signals:
    void clicked();

    // QQuickPaintedItem interface
public:
    QString title() const;
    void setTitle(const QString &title);
    void setChecked(bool checked);

    // QQuickItem interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
};

#endif // DOCKTABBUTTON_H
