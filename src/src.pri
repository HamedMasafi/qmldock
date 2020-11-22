QT += widgets quick
#quickcontrols2 quickcontrols2-private \
#quicktemplates2 quicktemplates2-private quick-private

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/abstractbutton.cpp \
    $$PWD/abstractmouseresponsiveitem.cpp \
    $$PWD/debugrect.cpp \
    $$PWD/dock.cpp \
    $$PWD/dockarea.cpp \
    $$PWD/dockcontainer.cpp \
    $$PWD/dockgroupresizehandler.cpp \
    $$PWD/dockmoveguide.cpp \
    $$PWD/dockplugin.cpp \
    $$PWD/docktabbar.cpp \
    $$PWD/docktabbararrorbutton.cpp \
    $$PWD/docktabbutton.cpp \
    $$PWD/dockwidget.cpp \
    $$PWD/dockwidgetbackground.cpp \
    $$PWD/dockwidgetheader.cpp \
    $$PWD/dockwidgetheaderbutton.cpp \
    $$PWD/dockwidgetmovehandler.cpp \
    $$PWD/dockwindow.cpp \
    $$PWD/movedropguide.cpp \
    $$PWD/style/abstractstyle.cpp \
    $$PWD/style/defaultstyle.cpp \
    $$PWD/style/vsdarkstyle.cpp \
    $$PWD/style/vslightstyle.cpp

RESOURCES += $$PWD/dock.qrc

HEADERS += \
    $$PWD/abstractbutton.h \
    $$PWD/abstractmouseresponsiveitem.h \
    $$PWD/debugrect.h \
    $$PWD/dock.h \
    $$PWD/dock_p.h \
    $$PWD/dockarea.h \
    $$PWD/dockarea_p.h \
    $$PWD/dockcontainer.h \
    $$PWD/dockcontainer_p.h \
    $$PWD/dockgroupresizehandler.h \
    $$PWD/dockmoveguide.h \
    $$PWD/dockplugin.h \
    $$PWD/docktabbar.h \
    $$PWD/docktabbararrorbutton.h \
    $$PWD/docktabbutton.h \
    $$PWD/dockwidget.h \
    $$PWD/dockwidget_p.h \
    $$PWD/dockwidgetbackground.h \
    $$PWD/dockwidgetheader.h \
    $$PWD/dockwidgetheaderbutton.h \
    $$PWD/dockwidgetmovehandler.h \
    $$PWD/dockwindow.h \
    $$PWD/movedropguide.h \
    $$PWD/style/abstractstyle.h \
    $$PWD/style/defaultstyle.h \
    $$PWD/style/vsdarkstyle.h \
    $$PWD/style/vslightstyle.h
