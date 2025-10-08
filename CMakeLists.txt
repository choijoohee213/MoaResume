cmake_minimum_required(VERSION 3.29)
project(MoaResume)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(MoaResume main.cpp)
target_link_libraries(MoaResume
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

