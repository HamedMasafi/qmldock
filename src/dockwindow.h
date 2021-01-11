#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

#include <QQuickWindow>

class DockWidget;
class DockWindow : public QQuickWindow
{
    Q_OBJECT
    DockWidget *_dockWidget;
public:
    DockWindow(DockWidget *dock, QWindow *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // DOCKWINDOW_H
