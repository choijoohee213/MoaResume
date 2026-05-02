#include "ResumeWidget.h"
#include "../ResumeConstants.h"

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
    mItemListWidget = new ResumeItemListWidget(this);

    mainLayout->addWidget(mSidebar);
    mainLayout->addWidget(mItemListWidget, 1);

    setLayout(mainLayout);
}

void ResumeWidget::connectSignals() {
    connect(mSidebar, &ResumeSidebar::categorySelected,
            mItemListWidget, &ResumeItemListWidget::setCategory);
}

void ResumeWidget::loadStyles() {
    QFile styleFile(ResumeConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }
}
