#include "vsdarkstyle.h"

#include <QPainter>
#include "dockwidget.h"
#include "dockwidgetbackground.h"

VSDarkStyle::VSDarkStyle(QObject *parent) : DefaultStyle(parent) {
  setBackgroundColor(QColor("#323234"));
  setBorderColor(QColor("#595E63"));
  setTextColor(Qt::white);
  setMainColor(QColor("#393E43"));
  setTabAreaColor(QColor("#1E1E1C"));
  setWidgetColor(QColor("#1E1E1C"));
  setActiveTextColor(QColor("#0179C2"));
  setHoverColor(QColor("#525254"));
}
