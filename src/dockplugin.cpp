#include "dockplugin.h"

#include "dockarea.h"
#include "dockwidget.h"
#include "dockgroup.h"

#include <QQmlApplicationEngine>
#include <QFontDatabase>

DockPlugin::DockPlugin(QObject *parent) : QObject(parent)
{

}

void DockPlugin::registerDockPlugin()
{
    qmlRegisterType<DockArea>("Kaj.Dock", 1, 0, "DockArea");
    qmlRegisterType<DockWidget>("Kaj.Dock", 1, 0, "DockWidget");
    qmlRegisterType<DockGroup>("Kaj.Dock", 1, 0, "DockGroup");
    qmlRegisterUncreatableMetaObject(Dock::staticMetaObject,
                                     "Kaj.Dock",
                                     1,
                                     0,
                                     "Dock",
                                     "Error: only enums");

    QFontDatabase::addApplicationFont(":/icons.ttf");
}
