project(QtTaskTests)

find_package(
  QT NAMES Qt6 Qt5
  COMPONENTS Widgets Test
  REQUIRED)
find_package(
  Qt${QT_VERSION_MAJOR}
  COMPONENTS Widgets Test
  REQUIRED)


add_executable(${PROJECT_NAME}
    testitemmodel.h
    testitemmodel.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QtTaskLib)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
