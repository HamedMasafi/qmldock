#include "vslightstyle.h"

VSLightStyle::VSLightStyle(QObject *parent) : DefaultStyle(parent)
{
    setBackgroundColor(QColor("#F8F7F9"));
    setBorderColor(QColor("#CBCEDB"));
    setTextColor(Qt::black);
    setMainColor(QColor("#393E43"));
    setTabAreaColor(QColor("#CBCEDB"));
    setWidgetColor(QColor("#F8F7F9"));
    setActiveTextColor(QColor("#0179C2"));
    setHoverColor(QColor("#dBdEeB"));
}
