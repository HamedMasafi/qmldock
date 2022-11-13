FILE(GLOB SOURCE_LIST "*.cpp" "*.h")

qt_add_library(DockWidget
        SHARED
        ${SOURCE_LIST}
        )

target_link_libraries(DockWidget
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        )

target_include_directories(DockWidget
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )