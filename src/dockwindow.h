#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

#include <QQuickWindow>

class DockWidget;
class DockWindow : public QQuickWindow
{
    Q_OBJECT
    DockWidget *_dockWidget;

public:
    DockWindow(DockWidget *dockWidget = nullptr, QWindow *parent = nullptr);

    DockWidget *dockWidget() const;
    void setDockWidget(DockWidget *dockWidget);

private Q_SLOTS:
    void dockWidget_closed();

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // DOCKWINDOW_H
