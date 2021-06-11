#ifndef DOCKPOPOUTWINDOW_H
#define DOCKPOPOUTWINDOW_H

#include <QQuickWindow>

class DockPopoutWindow : public QQuickWindow
{
    Q_OBJECT
    QQuickItem *_contentItem;

public:
    DockPopoutWindow(QQuickItem *contentItem = nullptr, QWindow *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // DOCKPOPOUTWINDOW_H
