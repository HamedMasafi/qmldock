#ifndef DOCK_P_H
#define DOCK_P_H

namespace Dock { namespace Private {

namespace Z {
constexpr static int Group{1};
constexpr static int Widget{2};
constexpr static int ActiveWidgetDim{3};
constexpr static int ActiveWidget{4};
constexpr static int FloatWidget{5};
constexpr static int TabBar{1};
constexpr static int Resizer{1};
constexpr static int Guide{1};
constexpr static int TitleBar{1};
 }

#ifdef QQUICKITEM_H
template<class T>
T *findInParents(QQuickItem *item)
{
    auto i = item;

    while (i) {
        auto p = qobject_cast<T *>(i);
        if (p)
            return p;

        i = i->parentItem();
    }
    return nullptr;
}
#else
template<class T, class QQuickItem>
T *findInParents(QQuickItem *item)
{
    static_assert (sizeof (QQuickItem) == 0, "The header <QQuickItem> is not included");
    return nullptr;
}
#endif // QQUICKITEM_H

} }

#endif // DOCK_P_H
