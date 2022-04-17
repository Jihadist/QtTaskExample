cmake_minimum_required(VERSION 3.5)

project(QtTask VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

add_library(${PROJECT_NAME}Lib
    src/lib/common.h
    src/lib/include/delegate.h
    src/lib/delegate.cpp
    src/lib/delegate_p.h
    src/lib/include/itemmodel.h
    src/lib/itemmodel.cpp

)

target_link_libraries(${PROJECT_NAME}Lib PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(${PROJECT_NAME}Lib PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/include)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}Lib)


set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER qt.task.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(tests)
