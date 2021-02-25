#ifndef DOCKDOCKABLEITEM_H
#define DOCKDOCKABLEITEM_H

#include <QQuickItem>

class DockDockableItemPrivate;
class DockDockableItem : public QQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DockDockableItem);
    DockDockableItemPrivate *d_ptr;
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)

public:
    enum Status {
        PoppedIn,
        PoppedOut
    };

    DockDockableItem(QQuickItem *parent = nullptr);
    void componentComplete() override;

    Status status() const;

public Q_SLOTS:
    void popOut();
    void popIn();

    void setStatus(Status status);

Q_SIGNALS:
    void statusChanged(Status status);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
};

#endif // DOCKDOCKABLEITEM_H
