#ifndef DOCKPLUGIN_H
#define DOCKPLUGIN_H

#include <QObject>

class DockPlugin : public QObject
{
    Q_OBJECT
public:
    explicit DockPlugin(QObject *parent = nullptr);

    static void registerDockPlugin();
Q_SIGNALS:

};

#endif // DOCKPLUGIN_H
