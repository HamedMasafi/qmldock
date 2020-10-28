#include "dockplugin.h"

#include "dockarea.h"
#include "dockwidget.h"
#include "dockgroup.h"
#include "style/abstractstyle.h"

#include <QQmlApplicationEngine>
#include <QFontDatabase>

//QObject *createDockStyle(QQmlEngine *, QJSEngine *)
//{
//    return dockStyle;
//}
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

//    qmlRegisterSingletonType<DockStyle>("Kaj.Dock", 1, 0, "DockStyle", createDockStyle);

    qRegisterMetaType<QList<DockWidget *>>();

    QFontDatabase::addApplicationFont(":/icons.ttf");
    QFontDatabase::addApplicationFont(":/icons/dock_font_default.ttf");
}
