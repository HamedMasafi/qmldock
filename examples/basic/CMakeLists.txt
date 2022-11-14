qt_add_executable(example
        main.cpp
        )

# add qml files
qt_add_resources(example
        "configuration"
        PREFIX "/"
        FILES
        main.qml
        CornerOwnerSettings.qml
        CusomTitleBar.qml
        TabPosition.qml
        )

target_link_libraries(example
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        DockWidget
        )