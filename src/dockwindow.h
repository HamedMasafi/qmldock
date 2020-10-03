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

//    // QWindow interface
//protected:
//    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
//    void mouseDoubleClickEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);

//    // QWindow interface
//protected:
//    void moveEvent(QMoveEvent *);

    // QWindow interface
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // DOCKWINDOW_H
