#include "BasicInfoWidget.h"
#include "../ResumeConstants.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFont>
#include <QFrame>

BasicInfoWidget::BasicInfoWidget(QWidget *parent)
    : QWidget(parent)
    , mService(ResumeService::getInstance()) {
    mFieldDefs = ResumeCategoryConfig::getFields(CategoryType::BasicInfo);
    setupUi();
    loadData();
    connectSignals();
}

void BasicInfoWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    mTitleLabel = new QLabel("기본정보", this);
    QFont titleFont = mTitleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    mTitleLabel->setFont(titleFont);
    mainLayout->addWidget(mTitleLabel);

    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    divider->setStyleSheet("color: #BFDBFE;");
    mainLayout->addWidget(divider);

    QWidget *formContainer = new QWidget(this);
    mFormLayout = new QFormLayout(formContainer);
    mFormLayout->setLabelAlignment(Qt::AlignLeft);
    mFormLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    mFormLayout->setHorizontalSpacing(16);
    mFormLayout->setVerticalSpacing(12);
    mFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    for (const FieldDefinition &field : mFieldDefs) {
        if (field.type == FieldDefinition::Text) {
            QLineEdit *edit = new QLineEdit(formContainer);
            edit->setMinimumHeight(32);
            mLineEdits[field.key] = edit;
            mFormLayout->addRow(field.label + ":", edit);
        } else if (field.type == FieldDefinition::Date) {
            QDateEdit *edit = new QDateEdit(formContainer);
            edit->setCalendarPopup(true);
            edit->setDisplayFormat("yyyy-MM-dd");
            edit->setDate(QDate::currentDate());
            edit->setSpecialValueText(" ");
            edit->setMinimumDate(QDate(1900, 1, 1));
            edit->setMinimumHeight(32);
            mDateEdits[field.key] = edit;
            mFormLayout->addRow(field.label + ":", edit);
        }
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(formContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    mSaveButton = new QPushButton("저장", this);
    mSaveButton->setMinimumSize(100, 36);
    btnLayout->addStretch();
    btnLayout->addWidget(mSaveButton);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}

void BasicInfoWidget::loadData() {
    // BasicInfo 카테고리 찾기
    for (const ResumeCategory &cat : mService.getCategories()) {
        if (cat.getType() == CategoryType::BasicInfo) {
            mCategoryId = cat.getId();
            break;
        }
    }
    if (mCategoryId < 0) return;

    ResumeCategory cat = mService.getCategoryById(mCategoryId);
    ResumeItem item;
    if (cat.getItems().isEmpty()) {
        // 첫 항목 자동 생성
        mService.addItem(mCategoryId, ResumeItem());
        cat    = mService.getCategoryById(mCategoryId);
    }
    item    = cat.getItems().first();
    mItemId = item.getId();

    for (const FieldDefinition &field : mFieldDefs) {
        QString value = item.getField(field.key);
        if (value.isEmpty()) continue;
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key)) {
            mLineEdits[field.key]->setText(value);
        } else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate date = QDate::fromString(value, "yyyy-MM-dd");
            if (date.isValid()) mDateEdits[field.key]->setDate(date);
        }
    }
}

void BasicInfoWidget::activate() {
    // 다른 화면에서 돌아올 때 최신 데이터 재로드
    for (auto it = mLineEdits.begin(); it != mLineEdits.end(); ++it)
        it.value()->clear();
    loadData();
}

void BasicInfoWidget::connectSignals() {
    connect(mSaveButton, &QPushButton::clicked, this, &BasicInfoWidget::onSaveClicked);
}

void BasicInfoWidget::onSaveClicked() {
    if (mCategoryId < 0) return;

    ResumeItem item;
    item.setId(mItemId);
    for (const FieldDefinition &field : mFieldDefs) {
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key)) {
            item.setField(field.key, mLineEdits[field.key]->text());
        } else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate d = mDateEdits[field.key]->date();
            item.setField(field.key, d.isValid() ? d.toString("yyyy-MM-dd") : "");
        }
    }

    if (mItemId < 0) {
        mService.addItem(mCategoryId, item);
        ResumeCategory cat = mService.getCategoryById(mCategoryId);
        if (!cat.getItems().isEmpty())
            mItemId = cat.getItems().first().getId();
    } else {
        mService.updateItem(mCategoryId, item);
    }

    QMessageBox::information(this, "저장 완료", "기본정보가 저장되었습니다.");
}
