#ifndef ABSTRACTSTYLE_H
#define ABSTRACTSTYLE_H

#define dockStyle AbstractStyle::style()
#include <QtGlobal>
#include <QFont>
#include "dock.h"

class QPainter;

class DockArea;
class DockTabBar;
class DockTabButton;
class DockGroup;
class DockWidget;
class DockWidgetHeader;
class DockWidgetBackground;
class DockGroupResizeHandler;
class DockWidgetHeaderButton;
class DockGroupResizeHandler;
class AbstractStyle
{
    static AbstractStyle *_style;
public:
    AbstractStyle() = default;
    virtual ~AbstractStyle() = default;
    static AbstractStyle *style();
    static void setStyle(AbstractStyle *style);

    virtual QFont font() const = 0;

    //sizes
    virtual qreal tabBarSize() const = 0;
    virtual qreal resizeHandleSize() const = 0;
    virtual qreal widgetPadding() const = 0;
    virtual qreal widgetResizePadding() const = 0;
    virtual qreal widgetTabPadding() const = 0;

    //colors
    /*virtual QColor areaColor() const = 0;
    virtual QColor widgetColor() const = 0;
    virtual QColor widgetHeaderColor() const = 0;
    virtual QColor tabAreaColor() const = 0;*/

    //draw methods
    virtual void paintDockGroup(QPainter *p, DockGroup *item) = 0;
    virtual void paintDockArea(QPainter *p, DockArea *item) = 0;
    virtual void paintDropButton(QPainter *p, Dock::Area area) = 0;
    virtual void paintTabBar(QPainter *p, DockTabBar *item) = 0;
    virtual void paintTabButton(QPainter *p, DockTabButton *item, Dock::ButtonStatus status) = 0;
    virtual void paintWidgetButton(QPainter *p,
                           DockWidgetHeaderButton *item,
                           Dock::ButtonStatus status) = 0;
    virtual void paintResizeHandler(QPainter *p, DockGroupResizeHandler *item, Dock::ButtonStatus status) = 0;
    virtual void paintDockWidget(QPainter *p, DockWidget *item) = 0;
    virtual void paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item) = 0;
    virtual void paintDockWidgetBackground(QPainter *p, DockWidgetBackground *item) = 0;

};

#endif // ABSTRACTSTYLE_H
