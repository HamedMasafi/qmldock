#include "abstractstyle.h"
#include "defaultstyle.h"

#include <dockcontainer.h>

AbstractStyle *AbstractStyle::_style{nullptr};
QList<QQuickItem *> AbstractStyle::_items;

AbstractStyle *AbstractStyle::style() {
  if (Q_UNLIKELY(!_style))
    _style = new DefaultStyle;
  return _style;
}

void AbstractStyle::setStyle(AbstractStyle *style, bool deleteOldStyle) {
  if (deleteOldStyle && Q_UNLIKELY(_style))
    delete _style;
  _style = style;

  for (auto i = _items.begin(); i != _items.end(); ++i)
    (*i)->update();
}

void AbstractStyle::registerThemableItem(QQuickItem *item) {
  _items.append(item);
  QObject::connect(item, &QObject::destroyed, [item]() { _items.removeOne(item); });
}
