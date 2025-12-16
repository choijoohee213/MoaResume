#include "WorkspaceSidebar.h"
#include "../WorkspaceConstants.h"

WorkspaceSidebar::WorkspaceSidebar(QWidget *parent) : QWidget(parent) {
    setupUi();
    setupMenuButtons();
    connectSignals();
}

void WorkspaceSidebar::setupUi() {
    setObjectName(WorkspaceConstants::SIDEBAR_OBJECT_NAME);
    setMinimumWidth(WorkspaceConstants::SIDEBAR_MIN_WIDTH);
    setMaximumWidth(WorkspaceConstants::SIDEBAR_MAX_WIDTH);

    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    mMenuButtonGroup = new QButtonGroup(this);
    mMenuButtonGroup->setExclusive(true);
}

void WorkspaceSidebar::setupMenuButtons() {
    mApplicationBtn = new QPushButton(WorkspaceConstants::MENU_TEXT_APPLICATION_LIST, this);
    mApplicationBtn->setObjectName(WorkspaceConstants::MENU_BTN_APPLICATION_LIST);
    mApplicationBtn->setCheckable(true);
    mApplicationBtn->setChecked(true);
    mMenuButtonGroup->addButton(mApplicationBtn,
        static_cast<int>(WorkspaceConstants::MENU_INDEX::APPLICATION_LIST));

    mResumeBtn = new QPushButton(WorkspaceConstants::MENU_TEXT_RESUME, this);
    mResumeBtn->setObjectName(WorkspaceConstants::MENU_BTN_RESUME);
    mResumeBtn->setCheckable(true);
    mMenuButtonGroup->addButton(mResumeBtn,
        static_cast<int>(WorkspaceConstants::MENU_INDEX::RESUME));

    mMainLayout->addWidget(mApplicationBtn);
    mMainLayout->addWidget(mResumeBtn);
    mMainLayout->addStretch();

    setLayout(mMainLayout);
}

void WorkspaceSidebar::connectSignals() {
    connect(mMenuButtonGroup, &QButtonGroup::idClicked, this,
            &WorkspaceSidebar::onMenuButtonClicked);
}

void WorkspaceSidebar::onMenuButtonClicked(int id) {
    emit menuChanged(id);
}
