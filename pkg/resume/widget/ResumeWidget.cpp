#include "ResumeWidget.h"
#include "../ResumeConstants.h"
#include "../service/ResumeService.h"
#include <QVBoxLayout>
#include <QPushButton>

ResumeWidget::ResumeWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
    connectSignals();
    loadStyles();
}

void ResumeWidget::setupUi() {
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // 상단 툴바 - 미리보기/PDF 버튼
    QWidget *toolbar = new QWidget(this);
    toolbar->setFixedHeight(44);
    toolbar->setStyleSheet("background-color: #DBEAFE; border-bottom: 1px solid #BFDBFE;");
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(16, 0, 16, 0);
    QPushButton *previewBtn = new QPushButton("미리보기 / PDF 저장", toolbar);
    previewBtn->setMinimumSize(150, 30);
    connect(previewBtn, &QPushButton::clicked, this, &ResumeWidget::onPreviewClicked);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(previewBtn);
    outerLayout->addWidget(toolbar);

    // 본문 (사이드바 + 콘텐츠)
    QWidget *body = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(body);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mSidebar = new ResumeSidebar(body);

    mStack = new QStackedWidget(body);
    mBasicInfoWidget = new BasicInfoWidget(mStack);
    mItemListWidget  = new ResumeItemListWidget(mStack);
    mStack->addWidget(mBasicInfoWidget);
    mStack->addWidget(mItemListWidget);

    mainLayout->addWidget(mSidebar);
    mainLayout->addWidget(mStack, 1);
    outerLayout->addWidget(body, 1);

    setLayout(outerLayout);
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

void ResumeWidget::onPreviewClicked() {
    ResumePreviewDialog dialog(this);
    dialog.exec();
}

void ResumeWidget::loadStyles() {
    QFile styleFile(ResumeConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }
}
