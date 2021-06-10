#ifndef DOCKWIDGETHEADER_H
#define DOCKWIDGETHEADER_H

#include <QImage>
#include <QQuickPaintedItem>

class DockWidget;
class DockWidgetHeaderButton;
class DockWidgetMoveHandler;
class DockWidgetHeader : public QQuickPaintedItem
{
    Q_OBJECT
    DockWidget *_parentDock;
    QPointF _lastMousePos;
    QPointF _lastParentPos;
    bool _moveEmitted;
    QString _title;

    DockWidgetHeaderButton *pinButton;
    DockWidgetHeaderButton *closeButton;

    DockWidgetMoveHandler *_moveHandler;
    enum ButtonStatus {
        Normal,
        Hovered,
        Pressed
    };
    bool _enableMove;
public:
    DockWidgetHeader(DockWidget *parent = nullptr);

    void paint(QPainter *painter) override;

    QString title() const;

    bool closeButtonVisible() const;
    void setCloseButtonVisible(bool closeButtonVisible);

    bool enableMove() const;
    void setEnableMove(bool enableMove);

    bool pinButtonVisible() const;
    void setPinButtonVisible(bool pinButtonVisible);

    DockWidget *parentDock() const;

    DockWidgetMoveHandler *moveHandler() const;

public Q_SLOTS:
    void setTitle(const QString &title);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
};

#endif // DOCKWIDGETHEADER_H
