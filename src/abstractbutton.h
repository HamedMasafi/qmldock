#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

#include <QQuickPaintedItem>
#include "dock.h"

class AbstractButton : public QQuickPaintedItem
{
    Q_OBJECT

protected:
    Dock::ButtonStatus _status;
    bool _hasFocus{false};
    Dock::Icon _icon;

public:
    AbstractButton(QQuickItem *parent = nullptr);

    Dock::Icon icon() const;
    void setIcon(const Dock::Icon &icon);

Q_SIGNALS:
    void clicked();

protected:
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;

public:
    void paint(QPainter *painter) override;
};

#endif // ABSTRACTBUTTON_H
