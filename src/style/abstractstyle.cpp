#include "abstractstyle.h"
#include "defaultstyle.h"

AbstractStyle *AbstractStyle::_style{nullptr};

AbstractStyle *AbstractStyle::style()
{
    if (Q_UNLIKELY(!_style))
        _style = new DefaultStyle;
    return _style;
}

void AbstractStyle::setStyle(AbstractStyle *style)
{
    if (Q_UNLIKELY(_style))
        delete _style;
    _style = style;
}
