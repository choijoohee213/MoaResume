#include "WorkspaceWidget.h"
#include "../WorkspaceConstants.h"
#include <QFile>

WorkspaceWidget::WorkspaceWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
    connectSignals();
    loadStyles();
}

void WorkspaceWidget::setupUi() {
    mMainLayout = new QHBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    mSidebar = new WorkspaceSidebar(this);
    mStackedWidget = new QStackedWidget(this);

    // Create content widgets
    mApplicationListWidget = new ApplicationListWidget(this);
    mResumeWidget = new ResumeWidget(this);

    // Add widgets to stacked widget (order must match MENU_INDEX enum)
    mStackedWidget->addWidget(mApplicationListWidget);  // index 0: APPLICATION_LIST
    mStackedWidget->addWidget(mResumeWidget);           // index 1: RESUME

    mMainLayout->addWidget(mSidebar);
    mMainLayout->addWidget(mStackedWidget, 1);

    setLayout(mMainLayout);
}

void WorkspaceWidget::loadStyles() {
    QFile styleFile(WorkspaceConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void WorkspaceWidget::connectSignals() {
    connect(mSidebar, &WorkspaceSidebar::menuChanged, this, &WorkspaceWidget::onMenuChanged);
}

void WorkspaceWidget::onMenuChanged(int menuIndex) {
    mStackedWidget->setCurrentIndex(menuIndex);
}
