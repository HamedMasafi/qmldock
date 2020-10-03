#include "dockarea.h"
#include "dockwidget.h"
#include "dockgroup.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("material");

    qmlRegisterType<DockArea>("AI", 1, 0, "DockArea");
    qmlRegisterType<DockWidget>("AI", 1, 0, "DockWidget");
    qmlRegisterType<DockGroup>("AI", 1, 0, "DockGroup");
    qmlRegisterUncreatableMetaObject(Dock::staticMetaObject,
                                     "AI",
                                     1,
                                     0,
                                     "Dock",
                                     "Error: only enums");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
