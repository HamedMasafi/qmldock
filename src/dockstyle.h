#ifndef DOCKSTYLE_H
#define DOCKSTYLE_H

#include <QObject>
#include "dock.h"

class QPainter;
class DockTabBar;
class DockTabButton;
class DockGroup;
class DockGroupResizeHandler;
class DockWidgetHeaderButton;
class DockGroupResizeHandler;
class DockStyle : public QObject
{
    Q_OBJECT
    void drawCircle(QPainter *painter,
                    const QPointF &center,
                    bool hover = false);

    void drawTab(QPainter *p, qreal *pos, const QString &title, int status);

public:
    enum ButtonStatus { Normal, Hovered, Pressed };

    static DockStyle *instance();
    explicit DockStyle(QObject *parent = nullptr);

    QFont defaultFont() const;
    qreal tabBarHeight() const;
    qreal tabBarButtonHeight() const;
    qreal tabBarButtonY() const;
    qreal tabMargin() const;
    qreal resizeHandleSize() const;

    QColor mainColor() const;
    QColor hoverColor() const;
    QColor pressColor() const;
    QColor borderColor() const;

    void paintDropButton(QPainter *p, Dock::Area area);
    void paintTabBar(QPainter *p, DockTabBar *item);
    void paintTabButton(QPainter *p, DockTabButton *item, Dock::ButtonStatus status);
    void paintWidgetButton(QPainter *p,
                           DockWidgetHeaderButton *item,
                           Dock::ButtonStatus status);
    void paintResizeHandler(QPainter *p, DockGroupResizeHandler *item, Dock::ButtonStatus status);
    void paintGroup(QPainter *p, DockGroup *item);
signals:

};

#endif // DOCKSTYLE_H
