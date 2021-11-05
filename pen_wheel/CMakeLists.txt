set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(TARGET_NAME ${PROJECT_NAME})
set(TARGET_SOURCES
        main.cpp
        windows/pen_wheel_window.cpp
        windows/pen_wheel_window.h
        windows/pen_wheel_window.ui
        widgets/wheel_widget.cpp
        widgets/wheel_widget.h
        widgets/wheel_widget.ui
        utils/mouse_events.cpp
        utils/mouse_events.h
        config/pen_wheel_config.cpp
        config/pen_wheel_config.h
        config/pen_wheel_action.cpp
        config/pen_wheel_action.h
        pen_wheel.cpp
        pen_wheel.h
        config/pen_wheel_color.cpp
        config/pen_wheel_color.h
        utils/processes.cpp
        utils/processes.h
        utils/processes_mac.mm
        utils/key_sender.cpp
        utils/key_sender.h
        config/pen_wheel_key_combination.cpp
        config/pen_wheel_key_combination.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${TARGET_SOURCES})
else ()
    add_executable(${TARGET_NAME} ${TARGET_SOURCES})
endif ()

if (APPLE)
    find_library(CARBON_LIBRARY Carbon)
    target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_LIBRARY})
endif()

target_link_libraries(${TARGET_NAME} PRIVATE yaml-cpp)
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${TARGET_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.eightbitforest.penwheel
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_NAME})
endif ()
