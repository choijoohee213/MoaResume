#include "ResumeItemDialog.h"
#include "../ResumeConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>

ResumeItemDialog::ResumeItemDialog(const ResumeItem &item, CategoryType categoryType, QWidget *parent)
    : QDialog(parent)
    , mItemId(item.getId())
    , mCategoryType(categoryType) {
    mFieldDefs = ResumeCategoryConfig::getFields(categoryType);
    setupUi();
    loadData(item);
    connectSignals();
    setWindowTitle(item.getId() == -1
        ? ResumeConstants::DIALOG_TITLE_ADD
        : ResumeConstants::DIALOG_TITLE_EDIT);
}

void ResumeItemDialog::setupUi() {
    setMinimumSize(700, 500);
    resize(750, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(12);

    QWidget *formContainer = new QWidget(this);

    mFormLayout = new QFormLayout();
    mFormLayout->setLabelAlignment(Qt::AlignLeft);
    mFormLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    mFormLayout->setHorizontalSpacing(16);
    mFormLayout->setVerticalSpacing(12);
    mFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    for (const FieldDefinition &field : mFieldDefs) {
        switch (field.type) {
        case FieldDefinition::Text: {
            QLineEdit *edit = new QLineEdit(formContainer);
            edit->setMinimumHeight(32);
            mLineEdits[field.key] = edit;
            mFormLayout->addRow(field.label + ":", edit);
            break;
        }
        case FieldDefinition::Date: {
            QDateEdit *edit = new QDateEdit(formContainer);
            edit->setCalendarPopup(true);
            edit->setDisplayFormat("yyyy-MM");
            edit->setDate(QDate::currentDate());
            edit->setSpecialValueText(" ");
            edit->setMinimumDate(QDate(1900, 1, 1));
            edit->setMinimumHeight(32);
            mDateEdits[field.key] = edit;
            mFormLayout->addRow(field.label + ":", edit);
            break;
        }
        case FieldDefinition::Combo: {
            QComboBox *combo = new QComboBox(formContainer);
            combo->setMinimumHeight(32);
            combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            combo->addItem("");
            for (const QString &opt : field.options) {
                combo->addItem(opt);
            }
            mComboBoxes[field.key] = combo;
            mFormLayout->addRow(field.label + ":", combo);
            break;
        }
        case FieldDefinition::Multiline:
        case FieldDefinition::Markdown: {
            QTextEdit *edit = new QTextEdit(formContainer);
            edit->setAcceptRichText(false);
            edit->setMinimumHeight(120);
            mTextEdits[field.key] = edit;
            mFormLayout->addRow(field.label + ":", edit);
            break;
        }
        }
    }
    formContainer->setLayout(mFormLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(formContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addWidget(scrollArea, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mSaveButton   = new QPushButton(ResumeConstants::BTN_TEXT_SAVE, this);
    mCancelButton = new QPushButton(ResumeConstants::BTN_TEXT_CANCEL, this);
    mSaveButton->setMinimumSize(80, 34);
    mCancelButton->setMinimumSize(80, 34);
    buttonLayout->addStretch();
    buttonLayout->addWidget(mSaveButton);
    buttonLayout->addWidget(mCancelButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void ResumeItemDialog::connectSignals() {
    connect(mSaveButton,   &QPushButton::clicked, this, &ResumeItemDialog::onSaveClicked);
    connect(mCancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ResumeItemDialog::loadData(const ResumeItem &item) {
    for (const FieldDefinition &field : mFieldDefs) {
        QString value = item.getField(field.key);
        if (value.isEmpty()) continue;

        switch (field.type) {
        case FieldDefinition::Text:
            if (mLineEdits.contains(field.key))
                mLineEdits[field.key]->setText(value);
            break;
        case FieldDefinition::Date:
            if (mDateEdits.contains(field.key)) {
                QDate date = QDate::fromString(value, "yyyy-MM");
                if (date.isValid()) mDateEdits[field.key]->setDate(date);
            }
            break;
        case FieldDefinition::Combo:
            if (mComboBoxes.contains(field.key)) {
                int idx = mComboBoxes[field.key]->findText(value);
                if (idx >= 0) mComboBoxes[field.key]->setCurrentIndex(idx);
            }
            break;
        case FieldDefinition::Multiline:
        case FieldDefinition::Markdown:
            if (mTextEdits.contains(field.key))
                mTextEdits[field.key]->setPlainText(value);
            break;
        }
    }
}

bool ResumeItemDialog::validateInput() {
    for (const FieldDefinition &field : mFieldDefs) {
        if (!field.required) continue;
        if (getFieldValue(field).trimmed().isEmpty()) {
            QMessageBox::warning(this,
                ResumeConstants::MSG_TITLE_ERROR,
                ResumeConstants::MSG_VALIDATION_EMPTY);
            return false;
        }
    }
    return true;
}

QString ResumeItemDialog::getFieldValue(const FieldDefinition &field) const {
    switch (field.type) {
    case FieldDefinition::Text:
        return mLineEdits.value(field.key) ? mLineEdits[field.key]->text() : "";
    case FieldDefinition::Date:
        if (mDateEdits.contains(field.key)) {
            QDate d = mDateEdits[field.key]->date();
            return d.isValid() ? d.toString("yyyy-MM") : "";
        }
        return "";
    case FieldDefinition::Combo:
        return mComboBoxes.value(field.key) ? mComboBoxes[field.key]->currentText() : "";
    case FieldDefinition::Multiline:
    case FieldDefinition::Markdown:
        return mTextEdits.value(field.key) ? mTextEdits[field.key]->toPlainText() : "";
    }
    return "";
}

ResumeItem ResumeItemDialog::getItem() const {
    ResumeItem item;
    item.setId(mItemId);
    for (const FieldDefinition &field : mFieldDefs) {
        item.setField(field.key, getFieldValue(field));
    }
    return item;
}

void ResumeItemDialog::onSaveClicked() {
    if (validateInput()) accept();
}
