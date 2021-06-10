#include "dock_p.h"
#include "dockarea.h"
#include "dockwidget.h"
#include <QObject>

namespace Dock {
namespace Private {

QString printDebug(QQuickItem *o) {
    auto dockWidget = qobject_cast<DockWidget*>(o);
    if (dockWidget) {
        return QStringLiteral("DockWidget (title=%1)")
            .arg(dockWidget->title());
    }

    auto dockArea = qobject_cast<DockArea*>(o);
    if (dockArea) {
        return QStringLiteral("DockWidget (area=%1)")
            .arg(dockArea->area());
    }
    return QString();
}

}
} // namespace Dock
