#include "ResumeWidget.h"
#include "../ResumeConstants.h"
#include "../service/ResumeService.h"

ResumeWidget::ResumeWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
    connectSignals();
    loadStyles();
}

void ResumeWidget::setupUi() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mSidebar = new ResumeSidebar(this);

    mStack = new QStackedWidget(this);
    mBasicInfoWidget = new BasicInfoWidget(mStack);
    mItemListWidget  = new ResumeItemListWidget(mStack);
    mStack->addWidget(mBasicInfoWidget); // index 0
    mStack->addWidget(mItemListWidget);  // index 1

    mainLayout->addWidget(mSidebar);
    mainLayout->addWidget(mStack, 1);

    setLayout(mainLayout);
}

void ResumeWidget::connectSignals() {
    connect(mSidebar, &ResumeSidebar::categorySelected,
            this, &ResumeWidget::onCategorySelected);
    mSidebar->selectFirst();
}

void ResumeWidget::onCategorySelected(int categoryId) {
    ResumeService &service = ResumeService::getInstance();
    ResumeCategory cat = service.getCategoryById(categoryId);

    if (cat.getType() == CategoryType::BasicInfo) {
        mBasicInfoWidget->activate();
        mStack->setCurrentIndex(0);
    } else {
        mItemListWidget->setCategory(categoryId);
        mStack->setCurrentIndex(1);
    }
}

void ResumeWidget::loadStyles() {
    QFile styleFile(ResumeConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }
}
