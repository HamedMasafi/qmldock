#ifndef DOCKSTYLE_H
#define DOCKSTYLE_H

#include <QColor>
#include <QFont>
#include <QObject>
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
class DockStyle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor mainColor READ mainColor WRITE setMainColor NOTIFY mainColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor NOTIFY hoverColorChanged)
    Q_PROPERTY(QColor pressColor READ pressColor WRITE setPressColor NOTIFY pressColorChanged)

    void drawCircle(QPainter *painter,
                    const QPointF &center,
                    bool hover = false);

    void drawTab(QPainter *p, qreal *pos, const QString &title, int status);

    QColor m_mainColor;

    QColor m_borderColor;

    QColor m_hoverColor;

    QColor m_pressColor;

public:
    enum ButtonStatus { Normal, Hovered, Pressed };

    static DockStyle *instance();
    explicit DockStyle(QObject *parent = nullptr);


    void paintDockArea(QPainter *p, DockArea *item);
    void paintDropButton(QPainter *p, Dock::Area area);
    void paintTabBar(QPainter *p, DockTabBar *item);
    void paintTabButton(QPainter *p, DockTabButton *item, Dock::ButtonStatus status);
    void paintWidgetButton(QPainter *p,
                           DockWidgetHeaderButton *item,
                           Dock::ButtonStatus status);
    void paintResizeHandler(QPainter *p, DockGroupResizeHandler *item, Dock::ButtonStatus status);
    void paintGroup(QPainter *p, DockGroup *item);
    void paintDockWidget(QPainter *p, DockWidget *item);
    void paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item);
    void paintDockWidgetBackground(QPainter *p, DockWidgetBackground *item);

    QColor mainColor() const;
    QColor borderColor() const;

    QColor hoverColor() const;

    QColor pressColor() const;

public slots:
    QFont defaultFont() const;
    qreal tabBarHeight() const;
    qreal tabBarButtonHeight() const;
    qreal tabBarButtonY() const;
    qreal tabMargin() const;
    qreal resizeHandleSize() const;
    qreal widgetPadding();

    void setMainColor(QColor mainColor);
    void setBorderColor(QColor borderColor);

    void setHoverColor(QColor hoverColor);

    void setPressColor(QColor pressColor);

signals:
    void mainColorChanged(QColor mainColor);
    void borderColorChanged(QColor borderColor);
    void hoverColorChanged(QColor hoverColor);
    void pressColorChanged(QColor pressColor);
};

#endif // DOCKSTYLE_H
