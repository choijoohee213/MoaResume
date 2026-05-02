#include "ResumeItemListWidget.h"
#include "ResumeItemDialog.h"
#include "../ResumeConstants.h"
#include "../config/ResumeCategoryConfig.h"
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>

ResumeItemListWidget::ResumeItemListWidget(QWidget *parent)
    : QWidget(parent)
    , mCurrentCategoryId(-1)
    , mCurrentCategoryType(CategoryType::BasicInfo)
    , mService(ResumeService::getInstance()) {
    setupUi();
    connectSignals();
}

void ResumeItemListWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    mCategoryLabel = new QLabel(this);
    QFont labelFont = mCategoryLabel->font();
    labelFont.setPointSize(14);
    labelFont.setBold(true);
    mCategoryLabel->setFont(labelFont);

    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    mAddButton = new QPushButton(ResumeConstants::BTN_TEXT_ADD, this);
    mAddButton->setCursor(Qt::PointingHandCursor);

    mDeleteButton = new QPushButton(ResumeConstants::BTN_TEXT_DELETE, this);
    mDeleteButton->setCursor(Qt::PointingHandCursor);
    mDeleteButton->setEnabled(false);

    mCopyButton = new QPushButton(ResumeConstants::BTN_TEXT_COPY, this);
    mCopyButton->setCursor(Qt::PointingHandCursor);
    mCopyButton->setEnabled(false);
    mCopyButton->setToolTip("선택한 항목의 내용을 클립보드에 복사");

    toolbarLayout->addWidget(mAddButton);
    toolbarLayout->addWidget(mDeleteButton);
    toolbarLayout->addWidget(mCopyButton);
    toolbarLayout->addStretch();

    mListWidget = new QListWidget(this);
    mListWidget->setAlternatingRowColors(true);
    mListWidget->setSpacing(2);

    mainLayout->addWidget(mCategoryLabel);
    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(mListWidget);

    setLayout(mainLayout);
}

void ResumeItemListWidget::connectSignals() {
    connect(mAddButton,    &QPushButton::clicked, this, &ResumeItemListWidget::onAddClicked);
    connect(mDeleteButton, &QPushButton::clicked, this, &ResumeItemListWidget::onDeleteClicked);
    connect(mCopyButton,   &QPushButton::clicked, this, &ResumeItemListWidget::onCopyClicked);
    connect(mListWidget,   &QListWidget::itemSelectionChanged, this, &ResumeItemListWidget::onSelectionChanged);
    connect(mListWidget,   &QListWidget::itemDoubleClicked, this, &ResumeItemListWidget::onItemDoubleClicked);
    connect(&mService,     &ResumeService::dataChanged, this, &ResumeItemListWidget::onDataChanged);
}

void ResumeItemListWidget::setCategory(int categoryId) {
    mCurrentCategoryId = categoryId;
    ResumeCategory category = mService.getCategoryById(categoryId);
    mCurrentCategoryType = category.getType();
    mCategoryLabel->setText(category.getName());
    loadItems();
    updateButtonStates();
}

void ResumeItemListWidget::loadItems() {
    mListWidget->clear();
    if (mCurrentCategoryId < 0) return;

    ResumeCategory category = mService.getCategoryById(mCurrentCategoryId);
    for (const ResumeItem &item : category.getItems()) {
        QString summary = ResumeCategoryConfig::getSummary(mCurrentCategoryType, item.getFields());
        QListWidgetItem *listItem = new QListWidgetItem(summary.isEmpty() ? "(내용 없음)" : summary);
        listItem->setData(Qt::UserRole, item.getId());
        mListWidget->addItem(listItem);
    }
}

void ResumeItemListWidget::updateButtonStates() {
    bool hasSelection = !mListWidget->selectedItems().isEmpty();
    mDeleteButton->setEnabled(hasSelection);
    mCopyButton->setEnabled(hasSelection);
}

int ResumeItemListWidget::getSelectedItemId() const {
    QList<QListWidgetItem*> selected = mListWidget->selectedItems();
    if (selected.isEmpty()) return -1;
    return selected.first()->data(Qt::UserRole).toInt();
}

void ResumeItemListWidget::onAddClicked() {
    if (mCurrentCategoryId < 0) return;
    ResumeItem newItem;
    ResumeItemDialog dialog(newItem, mCurrentCategoryType, this);
    if (dialog.exec() == QDialog::Accepted) {
        mService.addItem(mCurrentCategoryId, dialog.getItem());
    }
}

void ResumeItemListWidget::onDeleteClicked() {
    int itemId = getSelectedItemId();
    if (itemId < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        ResumeConstants::MSG_TITLE_CONFIRM,
        ResumeConstants::MSG_DELETE_CONFIRM,
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        mService.deleteItem(mCurrentCategoryId, itemId);
    }
}

void ResumeItemListWidget::onCopyClicked() {
    int itemId = getSelectedItemId();
    if (itemId < 0) return;

    ResumeCategory category = mService.getCategoryById(mCurrentCategoryId);
    ResumeItem item = category.getItemById(itemId);

    // 전체 필드를 읽기 좋은 텍스트로 변환
    QList<FieldDefinition> fieldDefs = ResumeCategoryConfig::getFields(mCurrentCategoryType);
    QString text;
    for (const FieldDefinition &field : fieldDefs) {
        QString value = item.getField(field.key);
        if (!value.isEmpty()) {
            text += field.label + ": " + value + "\n";
        }
    }

    QApplication::clipboard()->setText(text.trimmed());
    QMessageBox::information(this, ResumeConstants::MSG_TITLE_CONFIRM, ResumeConstants::MSG_COPY_SUCCESS);
}

void ResumeItemListWidget::onSelectionChanged() {
    updateButtonStates();
}

void ResumeItemListWidget::onDataChanged(int categoryId) {
    if (categoryId == mCurrentCategoryId) loadItems();
}

void ResumeItemListWidget::onItemDoubleClicked(QListWidgetItem *listItem) {
    int itemId = listItem->data(Qt::UserRole).toInt();
    ResumeCategory category = mService.getCategoryById(mCurrentCategoryId);
    ResumeItem item = category.getItemById(itemId);

    ResumeItemDialog dialog(item, mCurrentCategoryType, this);
    if (dialog.exec() == QDialog::Accepted) {
        mService.updateItem(mCurrentCategoryId, dialog.getItem());
    }
}
