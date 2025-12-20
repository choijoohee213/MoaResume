cmake_minimum_required(VERSION 3.29)
project(MoaResume)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(MoaResume
        main.cpp
        ui/MainWindow.cpp ui/MainWindow.h
        pkg/home/widget/HomeWidget.cpp pkg/home/widget/HomeWidget.h
        pkg/workspace/widget/WorkspaceWidget.cpp pkg/workspace/widget/WorkspaceWidget.h
        pkg/workspace/widget/WorkspaceSidebar.cpp pkg/workspace/widget/WorkspaceSidebar.h
        pkg/home/home.qrc
        pkg/workspace/workspace.qrc
        pkg/applicationList/widget/ApplicationListWidget.cpp
        pkg/applicationList/widget/ApplicationListWidget.h
        pkg/applicationList/model/Application.cpp
        pkg/applicationList/model/Application.h
        pkg/applicationList/service/ApplicationListService.cpp
        pkg/applicationList/service/ApplicationListService.h
        pkg/resume/widget/ResumeWidget.cpp
        pkg/resume/widget/ResumeWidget.h
)

target_link_libraries(MoaResume
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

