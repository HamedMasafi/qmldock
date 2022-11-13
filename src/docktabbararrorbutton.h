#ifndef DOCKTABBARARRORBUTTON_H
#define DOCKTABBARARRORBUTTON_H

#include "abstractbutton.h"

class DockTabBarArrorButton : public AbstractButton {
public:
  DockTabBarArrorButton(QQuickItem *parent = nullptr);
  void paint(QPainter *painter);
};

#endif // DOCKTABBARARRORBUTTON_H
