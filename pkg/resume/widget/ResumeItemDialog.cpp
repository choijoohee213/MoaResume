#include "ResumeItemDialog.h"
#include "../ResumeConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QDateTime>

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

    mFormContainer = new QWidget(this);

    mFormLayout = new QFormLayout();
    mFormLayout->setLabelAlignment(Qt::AlignLeft);
    mFormLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    mFormLayout->setHorizontalSpacing(16);
    mFormLayout->setVerticalSpacing(12);
    mFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    if (mCategoryType == CategoryType::Education) {
        setupEducationUi();
    } else {
        for (const FieldDefinition &field : mFieldDefs) {
            addFieldRow(field);
        }
    }

    mFormContainer->setLayout(mFormLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(mFormContainer);
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

void ResumeItemDialog::setupEducationUi() {
    mEduTypeCombo = new QComboBox(mFormContainer);
    mEduTypeCombo->setMinimumHeight(32);
    mEduTypeCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mEduTypeCombo->addItem("");
    mEduTypeCombo->addItem("고등학교");
    mEduTypeCombo->addItem("대학교");
    mEduTypeCombo->addItem("대학원");
    mComboBoxes["educationType"] = mEduTypeCombo;
    mFormLayout->addRow("학력 구분:", mEduTypeCombo);
    mDynamicRowStart = mFormLayout->rowCount();
}

void ResumeItemDialog::rebuildEducationFields(const QString &eduType) {
    clearDynamicRows();
    if (eduType.isEmpty()) return;

    QList<FieldDefinition> fields = ResumeCategoryConfig::getEducationFields(eduType);
    for (const FieldDefinition &field : fields) {
        addFieldRow(field);
    }

    // 부/복수전공 조건부 전공명 행 추가 (대학교/대학원만, 숨김 상태로)
    if (eduType != "고등학교" && mComboBoxes.contains("minorType")) {
        mMinorMajorLabel = new QLabel("부/복수전공 전공명:", mFormContainer);
        mMinorMajorEdit  = new QLineEdit(mFormContainer);
        mMinorMajorEdit->setMinimumHeight(32);
        mFormLayout->addRow(mMinorMajorLabel, mMinorMajorEdit);
        mLineEdits["minorMajor"]  = mMinorMajorEdit;
        mDynamicKeys.append("minorMajor");

        QString minorType = mComboBoxes["minorType"]->currentText();
        bool show = (!minorType.isEmpty() && minorType != "없음");
        mMinorMajorLabel->setVisible(show);
        mMinorMajorEdit->setVisible(show);

        connect(mComboBoxes["minorType"], &QComboBox::currentTextChanged,
                this, &ResumeItemDialog::onMinorTypeChanged);
    }
}

void ResumeItemDialog::clearDynamicRows() {
    while (mFormLayout->rowCount() > mDynamicRowStart) {
        mFormLayout->removeRow(mFormLayout->rowCount() - 1);
    }
    for (const QString &key : mDynamicKeys) {
        mLineEdits.remove(key);
        mDateEdits.remove(key);
        mComboBoxes.remove(key);
        mTextEdits.remove(key);
    }
    mDynamicKeys.clear();
    mAttachLabel     = nullptr;
    mMinorMajorLabel = nullptr;
    mMinorMajorEdit  = nullptr;
}

void ResumeItemDialog::addFieldRow(const FieldDefinition &field) {
    switch (field.type) {
    case FieldDefinition::Text: {
        QLineEdit *edit = new QLineEdit(mFormContainer);
        edit->setMinimumHeight(32);
        mLineEdits[field.key] = edit;
        mFormLayout->addRow(field.label + ":", edit);
        break;
    }
    case FieldDefinition::Date: {
        QDateEdit *edit = new QDateEdit(mFormContainer);
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
        QComboBox *combo = new QComboBox(mFormContainer);
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
        QTextEdit *edit = new QTextEdit(mFormContainer);
        edit->setAcceptRichText(false);
        edit->setMinimumHeight(120);
        mTextEdits[field.key] = edit;

        if (mCategoryType == CategoryType::Introduction) {
            QWidget *wrapper = new QWidget(mFormContainer);
            QVBoxLayout *vlay = new QVBoxLayout(wrapper);
            vlay->setContentsMargins(0, 0, 0, 0);
            vlay->setSpacing(4);
            QLabel *charCount = new QLabel("0자", wrapper);
            charCount->setAlignment(Qt::AlignRight);
            charCount->setStyleSheet("color: #93C5FD; font-size: 12px;");
            vlay->addWidget(edit);
            vlay->addWidget(charCount);
            connect(edit, &QTextEdit::textChanged, [edit, charCount]() {
                QString text = edit->toPlainText();
                if (text.endsWith('\n')) text.chop(1);
                charCount->setText(QString("%1자").arg(text.length()));
            });
            mFormLayout->addRow(field.label + ":", wrapper);
        } else {
            mFormLayout->addRow(field.label + ":", edit);
        }
        break;
    }
    case FieldDefinition::FileAttach: {
        QWidget *attachWidget = new QWidget(mFormContainer);
        QHBoxLayout *hlay = new QHBoxLayout(attachWidget);
        hlay->setContentsMargins(0, 0, 0, 0);
        hlay->setSpacing(8);
        mAttachLabel = new QLabel(mAttachmentPath.isEmpty()
            ? "파일을 선택하세요"
            : QFileInfo(mAttachmentPath).fileName(), attachWidget);
        mAttachLabel->setObjectName("attachLabel");
        QPushButton *btn = new QPushButton("파일 선택", attachWidget);
        btn->setMinimumHeight(32);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, &ResumeItemDialog::onAttachClicked);
        hlay->addWidget(mAttachLabel, 1);
        hlay->addWidget(btn);
        mFormLayout->addRow(field.label + ":", attachWidget);
        break;
    }
    }

    // Education 동적 행 추적 (educationType 고정 행 제외)
    if (mCategoryType == CategoryType::Education && field.key != "educationType") {
        mDynamicKeys.append(field.key);
    }
}

void ResumeItemDialog::connectSignals() {
    connect(mSaveButton,   &QPushButton::clicked, this, &ResumeItemDialog::onSaveClicked);
    connect(mCancelButton, &QPushButton::clicked, this, &QDialog::reject);

    if (mCategoryType == CategoryType::Education && mEduTypeCombo) {
        connect(mEduTypeCombo, &QComboBox::currentTextChanged,
                this, &ResumeItemDialog::onEduTypeChanged);
    }
}

void ResumeItemDialog::loadData(const ResumeItem &item) {
    // attachmentPath 먼저 세팅 (FileAttach 위젯 생성 시 표시에 필요)
    mAttachmentPath = item.getField("attachmentPath");

    if (mCategoryType == CategoryType::Education) {
        // 학력 구분 설정 (connectSignals 전이므로 onEduTypeChanged 미발생)
        QString eduType = item.getField("educationType");
        if (!eduType.isEmpty()) {
            int idx = mEduTypeCombo->findText(eduType);
            if (idx >= 0) mEduTypeCombo->setCurrentIndex(idx);
        }
        rebuildEducationFields(mEduTypeCombo->currentText());

        // 각 필드 값 로드
        for (const QString &key : mLineEdits.keys()) {
            QString value = item.getField(key);
            if (!value.isEmpty()) mLineEdits[key]->setText(value);
        }
        for (const QString &key : mDateEdits.keys()) {
            QString value = item.getField(key);
            if (!value.isEmpty()) {
                QDate date = QDate::fromString(value, "yyyy-MM");
                if (date.isValid()) mDateEdits[key]->setDate(date);
            }
        }
        for (const QString &key : mComboBoxes.keys()) {
            if (key == "educationType") continue;
            QString value = item.getField(key);
            if (!value.isEmpty()) {
                int idx = mComboBoxes[key]->findText(value);
                if (idx >= 0) mComboBoxes[key]->setCurrentIndex(idx);
            }
        }

        // minorType 값에 따라 minorMajor 가시성 업데이트
        if (mComboBoxes.contains("minorType") && mMinorMajorLabel) {
            QString minorType = mComboBoxes["minorType"]->currentText();
            bool show = (!minorType.isEmpty() && minorType != "없음");
            mMinorMajorLabel->setVisible(show);
            if (mMinorMajorEdit) mMinorMajorEdit->setVisible(show);
        }
        return;
    }

    // 비-Education 타입: 기존 로직
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
        case FieldDefinition::FileAttach:
            break;
        }
    }
}

void ResumeItemDialog::onEduTypeChanged(const QString &type) {
    rebuildEducationFields(type);
}

void ResumeItemDialog::onMinorTypeChanged(const QString &type) {
    if (!mMinorMajorLabel || !mMinorMajorEdit) return;
    bool show = (!type.isEmpty() && type != "없음");
    mMinorMajorLabel->setVisible(show);
    mMinorMajorEdit->setVisible(show);
}

void ResumeItemDialog::onAttachClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "성적증명서 선택", QString(),
        "PDF 파일 (*.pdf);;모든 파일 (*)");
    if (filePath.isEmpty()) return;

    // 앱 데이터 디렉토리 하위 attachments 폴더에 복사
    QString dataDir   = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString attachDir = dataDir + "/attachments";
    QDir().mkpath(attachDir);

    QFileInfo fi(filePath);
    QString destPath = attachDir + "/" + fi.fileName();
    if (QFile::exists(destPath) && destPath != filePath) {
        QString ts = QString::number(QDateTime::currentMSecsSinceEpoch());
        destPath = attachDir + "/" + fi.completeBaseName() + "_" + ts
                   + (fi.suffix().isEmpty() ? "" : "." + fi.suffix());
    }
    if (filePath != destPath) {
        QFile::copy(filePath, destPath);
        mAttachmentPath = destPath;
    } else {
        mAttachmentPath = filePath;
    }

    if (mAttachLabel) mAttachLabel->setText(fi.fileName());
}

bool ResumeItemDialog::validateInput() {
    if (mCategoryType == CategoryType::Education) {
        if (mEduTypeCombo && mEduTypeCombo->currentText().isEmpty()) {
            QMessageBox::warning(this, ResumeConstants::MSG_TITLE_ERROR,
                "학력 구분을 선택해주세요.");
            return false;
        }
        if (mLineEdits.contains("school") && mLineEdits["school"]->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, ResumeConstants::MSG_TITLE_ERROR,
                ResumeConstants::MSG_VALIDATION_EMPTY);
            return false;
        }
        return true;
    }

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
    case FieldDefinition::FileAttach:
        return mAttachmentPath;
    }
    return "";
}

ResumeItem ResumeItemDialog::getItem() const {
    ResumeItem item;
    item.setId(mItemId);

    if (mCategoryType == CategoryType::Education) {
        if (mEduTypeCombo)
            item.setField("educationType", mEduTypeCombo->currentText());
        for (auto it = mLineEdits.constBegin(); it != mLineEdits.constEnd(); ++it)
            item.setField(it.key(), it.value()->text());
        for (auto it = mDateEdits.constBegin(); it != mDateEdits.constEnd(); ++it) {
            QDate d = it.value()->date();
            item.setField(it.key(), d.isValid() ? d.toString("yyyy-MM") : "");
        }
        for (auto it = mComboBoxes.constBegin(); it != mComboBoxes.constEnd(); ++it) {
            if (it.key() == "educationType") continue;
            item.setField(it.key(), it.value()->currentText());
        }
        item.setField("attachmentPath", mAttachmentPath);
        return item;
    }

    for (const FieldDefinition &field : mFieldDefs) {
        item.setField(field.key, getFieldValue(field));
    }
    return item;
}

void ResumeItemDialog::onSaveClicked() {
    if (validateInput()) accept();
}
