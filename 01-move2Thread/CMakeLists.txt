cmake_minimum_required(VERSION 3.14)

project(timer_in_thread01 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Concurrent Widgets Quick)
qt5_add_resources(RESOURCES "qml.qrc")
aux_source_directory(. CPP_FILES)

file (GLOB H_Files "./*.h")

add_executable(timer_in_thread01
  main.cpp ${CPP_FILES} ${RESOURCES}
)
target_link_libraries(timer_in_thread01 Qt${QT_VERSION_MAJOR}::Core Qt5::Widgets
    Qt${QT_VERSION_MAJOR}::Quick)

install(TARGETS timer_in_thread01
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
