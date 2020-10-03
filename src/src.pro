QT += widgets quick quickcontrols2 quickcontrols2-private \
quicktemplates2 quicktemplates2-private quick-private

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

SOURCES += \
        abstractmouseresponsiveitem.cpp \
        debugrect.cpp \
        dock.cpp \
        dockarea.cpp \
        dockgroup.cpp \
        dockgroupresizehandler.cpp \
        dockmoveguide.cpp \
        dockstyle.cpp \
        docktabbar.cpp \
        docktabbutton.cpp \
        dockwidget.cpp \
        dockwidgetbackground.cpp \
        dockwidgetheader.cpp \
        dockwidgetheaderbutton.cpp \
        dockwindow.cpp \
        main.cpp
#        dockwindow.cpp \

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    abstractmouseresponsiveitem.h \
    debugrect.h \
    dock.h \
    dock_p.h \
    dockarea.h \
    dockgroup.h \
    dockgroup_p.h \
    dockgroupresizehandler.h \
    dockmoveguide.h \
    dockstyle.h \
    docktabbar.h \
    docktabbutton.h \
    dockwidget.h \
    dockwidgetbackground.h \
    dockwidgetheader.h \
    dockwidgetheaderbutton.h \
    dockwindow.h
 #    dockwindow.h
