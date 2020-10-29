#ifndef DefaultStyle_H
#define DefaultStyle_H

#include <QColor>
#include <QFont>
#include <QObject>
#include "abstractstyle.h"

class QQuickItem;
class DefaultStyle : public QObject, public AbstractStyle
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor widgetColor READ widgetColor WRITE setWidgetColor NOTIFY widgetColorChanged)
    Q_PROPERTY(QColor mainColor READ mainColor WRITE setMainColor NOTIFY mainColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor NOTIFY hoverColorChanged)
    Q_PROPERTY(QColor pressColor READ pressColor WRITE setPressColor NOTIFY pressColorChanged)
    Q_PROPERTY(QColor tabAreaColor READ tabAreaColor WRITE setTabAreaColor NOTIFY tabAreaColorChanged)
    Q_PROPERTY(QColor activeTextColor READ activeTextColor WRITE setActiveTextColor NOTIFY activeTextColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)


public:
    explicit DefaultStyle(QObject *parent = nullptr);


    void paintDockArea(QPainter *p, DockArea *item) override;
    void paintDropButton(QPainter *p, Dock::Area area, const QRectF &rc, bool hover) override;
    void paintTabBar(QPainter *p, DockTabBar *item) override;
    void paintTabButton(QPainter *p, DockTabButton *item, Dock::ButtonStatus status) override;
    void paintWidgetButton(QPainter *p,
                           DockWidgetHeaderButton *item,
                           Dock::ButtonStatus status) override;
    void paintResizeHandler(QPainter *p, DockGroupResizeHandler *item, Dock::ButtonStatus status) override;
    void paintDockGroup(QPainter *p, DockGroup *item) override;
    void paintDockWidget(QPainter *p, DockWidget *item) override;
    void paintDockWidgetHeader(QPainter *p, DockWidgetHeader *item) override;
    void paintDockWidgetBackground(QPainter *p, DockWidgetBackground *item) override;

    QFont font() const override;

    qreal resizeHandleSize() const override;
    qreal widgetPadding() const override;
    qreal widgetResizePadding() const override;
    qreal widgetTabPadding() const override;
    qreal tabBarSize() const override;
    qreal dropButtonSize() const override;
    qreal dropButtonSpace() const override;

    QColor mainColor() const;
    QColor borderColor() const;
    QColor hoverColor() const;
    QColor pressColor() const;
    QColor tabAreaColor() const;
    QColor textColor() const;
    QColor backgroundColor() const;

    QColor widgetColor() const;

    QColor activeTextColor() const;

signals:
    void mainColorChanged(QColor mainColor);
    void borderColorChanged(QColor borderColor);
    void hoverColorChanged(QColor hoverColor);
    void pressColorChanged(QColor pressColor);
    void tabAreaColorChanged(QColor tabAreaColor);
    void textColorChanged(QColor textColor);
    void backgroundColorChanged(QColor backgroundColor);

    void widgetColorChanged(QColor widgetColor);

    void activeTextColorChanged(QColor activeTextColor);

public slots:
    void setMainColor(QColor mainColor);
    void setBorderColor(QColor borderColor);
    void setHoverColor(QColor hoverColor);
    void setPressColor(QColor pressColor);
    void setTabAreaColor(QColor tabAreaColor);
    void setTextColor(QColor textColor);
    void setBackgroundColor(QColor backgroundColor);

    void setWidgetColor(QColor widgetColor);

    void setActiveTextColor(QColor activeTextColor);

private:
    void drawLineOnEdge(QPainter *p, QQuickItem *item, Qt::Edge edge) const;
    void drawCircle(QPainter *painter,
                    const QPointF &center,
                    bool hover = false);


    QColor m_backgroundColor;
    QColor m_mainColor;
    QColor m_borderColor;
    QColor m_hoverColor;
    QColor m_pressColor;
    QColor m_tabAreaColor;
    QColor m_textColor;
    QColor m_widgetColor;
    QColor m_activeTextColor;
};

#endif // DefaultStyle_H
