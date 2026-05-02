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
        common/util/JsonFileUtils.cpp
        common/util/JsonFileUtils.h
        common/common.qrc
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
        pkg/applicationList/widget/ApplicationDetailDialog.cpp
        pkg/applicationList/widget/ApplicationDetailDialog.h
        pkg/applicationList/applicationList.qrc
        pkg/resume/widget/ResumeWidget.cpp
        pkg/resume/widget/ResumeWidget.h
        pkg/resume/widget/ResumeSidebar.cpp
        pkg/resume/widget/ResumeSidebar.h
        pkg/resume/widget/ResumeItemListWidget.cpp
        pkg/resume/widget/ResumeItemListWidget.h
        pkg/resume/widget/ResumeItemDialog.cpp
        pkg/resume/widget/ResumeItemDialog.h
        pkg/resume/model/ResumeItem.cpp
        pkg/resume/model/ResumeItem.h
        pkg/resume/model/ResumeCategory.cpp
        pkg/resume/model/ResumeCategory.h
        pkg/resume/service/ResumeService.cpp
        pkg/resume/service/ResumeService.h
        pkg/resume/ResumeConstants.h
        pkg/resume/config/ResumeCategoryConfig.h
        pkg/resume/config/ResumeCategoryConfig.cpp
        pkg/resume/resume.qrc
        pkg/applicationList/ApplicationListConstants.h
)

target_link_libraries(MoaResume
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

