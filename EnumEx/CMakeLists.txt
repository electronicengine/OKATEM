cmake_minimum_required(VERSION 3.1)

project(XEnum)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/usr/local/Qt5.10.1/5.10.1/clang_64")

find_package(Qt5)
find_package(Qt5Widgets) # Find the QtWidgets library
#set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES cxx_decltype)


add_executable(${PROJECT_NAME} "main.cpp"
                                "monad.h"
                                "enum.h"
                                "cint.h"
                                "mainwindow.cpp"
                                "fileops.cpp"
                                "mainwindow.h"
                                "widgetypes.h") # Tell CMake to create the helloworld executable

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets) # Use the Widgets module from Qt 5.
