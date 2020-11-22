#ifndef DOCKWIDGETMOVEHANDLER_H
#define DOCKWIDGETMOVEHANDLER_H

#include <QQuickItem>

class DockWidget;
class DockWidgetMoveHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(DockWidget *dockWidget READ dockWidget WRITE setDockWidget NOTIFY dockWidgetChanged)

    DockWidget *m_dockWidget;
    QPointF _lastMousePos;
    QPointF _lastParentPos;
    bool _moveEmitted;
    QString _title;
    bool _enableMove;

public:
    DockWidgetMoveHandler(QQuickItem *parent = nullptr);

    DockWidget * dockWidget() const;

public slots:
    void setDockWidget(DockWidget * dockWidget);

signals:
    void dockWidgetChanged(DockWidget * dockWidget);

protected:
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DOCKWIDGETMOVEHANDLER_H
