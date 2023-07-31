#include "dockcontainer.h"
#include "dockwidget.h"
#include "dockarea.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  QQuickStyle::setStyle("material");

  qmlRegisterType<DockContainer>("AI", 1, 0, "DockContainer");
  qmlRegisterType<DockWidget>("AI", 1, 0, "DockWidget");
  qmlRegisterType<DockArea>("AI", 1, 0, "DockArea");
  qmlRegisterUncreatableMetaObject(Dock::staticMetaObject, "AI", 1, 0, "Dock", "Error: only enums");

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
