set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(TARGET_NAME ${PROJECT_NAME})
set(TARGET_SOURCES
        main.cpp
        windows/main_window.cpp
        windows/main_window.h
        windows/main_window.ui
        windows/wheel.cpp
        windows/wheel.h
        windows/wheel.ui widgets/wheel_widget.cpp widgets/wheel_widget.h widgets/wheel_widget.ui utils/mouse_events.cpp utils/mouse_events.h config/pen_wheel_config.cpp config/pen_wheel_config.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${TARGET_SOURCES})
else ()
    add_executable(${TARGET_NAME} ${TARGET_SOURCES})
endif ()

target_link_libraries(${TARGET_NAME} PRIVATE yaml-cpp)
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${TARGET_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.eightbitforest.penwheel
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_NAME})
endif ()
