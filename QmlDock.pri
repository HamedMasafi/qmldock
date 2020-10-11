QT += widgets quick quickcontrols2 quickcontrols2-private \
quicktemplates2 quicktemplates2-private quick-private

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/abstractmouseresponsiveitem.cpp \
    $$PWD/debugrect.cpp \
    $$PWD/dock.cpp \
    $$PWD/dockarea.cpp \
    $$PWD/dockgroup.cpp \
    $$PWD/dockgroupresizehandler.cpp \
    $$PWD/dockmoveguide.cpp \
    $$PWD/dockstyle.cpp \
    $$PWD/docktabbar.cpp \
    $$PWD/docktabbutton.cpp \
    $$PWD/dockwidget.cpp \
    $$PWD/dockwidgetbackground.cpp \
    $$PWD/dockwidgetheader.cpp \
    $$PWD/dockwidgetheaderbutton.cpp \
    $$PWD/dockwindow.cpp

RESOURCES += $$PWD/qml.qrc

HEADERS += \
    $$PWD/abstractmouseresponsiveitem.h \
    $$PWD/debugrect.h \
    $$PWD/dock.h \
    $$PWD/dock_p.h \
    $$PWD/dockarea.h \
    $$PWD/dockgroup.h \
    $$PWD/dockgroup_p.h \
    $$PWD/dockgroupresizehandler.h \
    $$PWD/dockmoveguide.h \
    $$PWD/dockstyle.h \
    $$PWD/docktabbar.h \
    $$PWD/docktabbutton.h \
    $$PWD/dockwidget.h \
    $$PWD/dockwidgetbackground.h \
    $$PWD/dockwidgetheader.h \
    $$PWD/dockwidgetheaderbutton.h \
    $$PWD/dockwindow.h
