#include "HomeWidget.h"
#include "../../../common/Constants.h"
#include "../HomeConstatns.h"
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
    connectSignals();
    loadStyles();
}

void HomeWidget::loadStyles() {
    QFile styleFile(HomeConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void HomeWidget::setupTitleLabel() {
    mTitleLabel = new QLabel(Constants::APP_TITLE, this);
    mTitleLabel->setObjectName(HomeConstants::TITLE_LABEL_NAME);
    mTitleLabel->setAlignment(Qt::AlignCenter);
}

void HomeWidget::setupStartBtn() {
    mStartButton = new QPushButton(HomeConstants::START_BUTTON_TEXT, this);
    mStartButton->setObjectName(HomeConstants::START_BUTTON_NAME);
    mStartButton->setMinimumSize(HomeConstants::START_BUTTON_MIN_WIDTH_SIZE,
                                 HomeConstants::START_BUTTON_MIN_HEIGHT_SIZE);
    mStartButton->setCursor(Qt::PointingHandCursor);
}

void HomeWidget::setupUi() {
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setAlignment(Qt::AlignCenter);
    mMainLayout->setSpacing(20);

    setupTitleLabel();
    setupStartBtn();

    mMainLayout->addStretch();
    mMainLayout->addWidget(mTitleLabel);
    mMainLayout->addSpacing(30);
    mMainLayout->addWidget(mStartButton, 0, Qt::AlignCenter);
    mMainLayout->addStretch();

    setLayout(mMainLayout);
}

void HomeWidget::connectSignals() {
    connect(mStartButton, &QPushButton::clicked, this, &HomeWidget::onWorkSpaceClicked);
}

void HomeWidget::onWorkSpaceClicked() {
    emit navigateToWorkspace();
}
