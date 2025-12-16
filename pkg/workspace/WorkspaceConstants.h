#ifndef WORKSPACECONSTANTS_H
#define WORKSPACECONSTANTS_H

namespace WorkspaceConstants {
    // Resource paths
    constexpr const char *QSS_PATH = ":/workspace/styles/workspace.qss";

    // Sidebar
    constexpr const char *SIDEBAR_OBJECT_NAME = "workspaceSidebar";
    constexpr int SIDEBAR_MIN_WIDTH = 200;
    constexpr int SIDEBAR_MAX_WIDTH = 250;

    // Menu button names
    constexpr const char *MENU_BTN_APPLICATION_LIST = "menuBtnApplicationList";
    constexpr const char *MENU_BTN_RESUME = "menuBtnResume";

    // Menu button texts
    constexpr const char *MENU_TEXT_APPLICATION_LIST = "지원서 목록";
    constexpr const char *MENU_TEXT_RESUME = "이력서 관리";

    // Menu indices (for QStackedWidget)
    enum class MENU_INDEX {
        APPLICATION_LIST = 0,
        RESUME
    };
}


#endif
