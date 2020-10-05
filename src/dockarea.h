#ifndef DOCKAREA_H
#define DOCKAREA_H

#include <QQuickPaintedItem>
#include "dock.h"

class DockWidget;
class DockMoveGuide;
class DockGroup;
class DockGroupResizeHandler;
class DockArea : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QList<DockWidget *> dockWidgets READ dockWidgets NOTIFY dockWidgetsChanged)
    Q_PROPERTY(Qt::Edge topLeftOwner READ topLeftOwner WRITE setTopLeftOwner NOTIFY topLeftOwnerChanged)
    Q_PROPERTY(Qt::Edge topRightOwner READ topRightOwner WRITE setTopRightOwner NOTIFY topRightOwnerChanged)
    Q_PROPERTY(Qt::Edge bottomLeftOwner READ bottomLeftOwner WRITE setBottomLeftOwner NOTIFY bottomLeftOwnerChanged)
    Q_PROPERTY(Qt::Edge bottomRightOwner READ bottomRightOwner WRITE setBottomRightOwner NOTIFY bottomRightOwnerChanged)

    QList<DockWidget *> _initialWidgets;
    QList<DockWidget *> _dockWidgets;
    DockMoveGuide *_dockMoveGuide;
    QMap<Dock::Area, DockGroup *> _dockGroups;
public:
    DockArea(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QList<DockWidget *> dockWidgets() const;

signals:

    void dockWidgetsChanged(QList<DockWidget *> dockWidgets);

    void topLeftOwnerChanged(Qt::Edge topLeftOwner);

    void topRightOwnerChanged(Qt::Edge topRightOwner);

void bottomLeftOwnerChanged(Qt::Edge bottomLeftOwner);

void bottomRightOwnerChanged(Qt::Edge bottomRightOwner);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChange(ItemChange, const ItemChangeData &) override;
    void componentComplete() override;

public slots:
    void addDockWidget(DockWidget *widget);
    void reorderDockGroups();

    void setTopLeftOwner(Qt::Edge topLeftOwner);

    void setTopRightOwner(Qt::Edge topRightOwner);

    void setBottomLeftOwner(Qt::Edge bottomLeftOwner);

    void setBottomRightOwner(Qt::Edge bottomRightOwner);

private slots:
    void dockWidget_beginMove();
    void dockWidget_moving(const QPointF &pt);
    void dockWidget_moved();
    void dockWidget_visibleChanged();

protected:

private:
    inline int panelSize(Dock::Area area) const;
    DockGroup *createGroup(Dock::Area area, DockGroup *item = nullptr);


    // QQmlParserStatus interface
    Qt::Edge m_topLeftOwner;

    Qt::Edge m_topRightOwner;

    Qt::Edge m_bottomLeftOwner;

    Qt::Edge m_bottomRightOwner;

public:
Qt::Edge topLeftOwner() const;
Qt::Edge topRightOwner() const;
Qt::Edge bottomLeftOwner() const;
Qt::Edge bottomRightOwner() const;
};

#endif // DOCKAREA_H
