#ifndef DOCKWIDGETHEADERBUTTON_H
#define DOCKWIDGETHEADERBUTTON_H

#include <QImage>
#include "abstractbutton.h"
#include "dock.h"

class DockWidgetHeaderButton : public AbstractButton {
  Q_OBJECT

public:
  DockWidgetHeaderButton(QQuickItem *parent = nullptr);
  void paint(QPainter *painter);
};

#endif // DOCKWIDGETHEADERBUTTON_H
