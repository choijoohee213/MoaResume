#include "ApplicationDetailDialog.h"
#include "../ApplicationListConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ApplicationDetailDialog::ApplicationDetailDialog(const Application &application, QWidget *parent)
    : QDialog(parent), mApplicationId(application.getId()), mPreviewVisible(true) {
    setupUi();
    setupInputFields();
    loadData(application);
    connectSignals();
}

void ApplicationDetailDialog::setupUi() {
    setWindowTitle(ApplicationListConstants::DIALOG_TITLE_DETAIL);
    setMinimumSize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    mCompanyNameEdit = new QLineEdit(this);
    mPositionEdit = new QLineEdit(this);
    mStatusComboBox = new QComboBox(this);
    mDeadlineEdit = new QDateEdit(this);
    mDeadlineEdit->setCalendarPopup(true);

    formLayout->addRow(ApplicationListConstants::LABEL_COMPANY_NAME, mCompanyNameEdit);
    formLayout->addRow(ApplicationListConstants::LABEL_POSITION, mPositionEdit);
    formLayout->addRow(ApplicationListConstants::LABEL_STATUS, mStatusComboBox);
    formLayout->addRow(ApplicationListConstants::LABEL_DEADLINE, mDeadlineEdit);

    QHBoxLayout *notesToolbarLayout = new QHBoxLayout();
    mTogglePreviewButton = new QPushButton("미리보기 숨기기", this);
    mTogglePreviewButton->setCursor(Qt::PointingHandCursor);
    notesToolbarLayout->addWidget(mTogglePreviewButton);
    notesToolbarLayout->addStretch();

    mNotesEdit = new QTextEdit(this);
    mNotesEdit->setAcceptRichText(false);
    mNotesEdit->setPlaceholderText("마크다운으로 작성하세요...");

    mNotesPreview = new QTextBrowser(this);
    mNotesPreview->setOpenExternalLinks(true);

    mNotesSplitter = new QSplitter(Qt::Horizontal, this);
    mNotesSplitter->addWidget(mNotesEdit);
    mNotesSplitter->addWidget(mNotesPreview);
    mNotesSplitter->setStretchFactor(0, 1);
    mNotesSplitter->setStretchFactor(1, 1);
    mNotesSplitter->setMinimumHeight(300);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mSaveButton = new QPushButton(ApplicationListConstants::BTN_TEXT_SAVE, this);
    mCancelButton = new QPushButton(ApplicationListConstants::BTN_TEXT_CANCEL, this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(mSaveButton);
    buttonLayout->addWidget(mCancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(notesToolbarLayout);
    mainLayout->addWidget(mNotesSplitter);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void ApplicationDetailDialog::setupInputFields() {
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_APPLIED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_DOCUMENT_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_CODING_TEST_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FIRST_INTERVIEW_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FINAL_INTERVIEW_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FINAL_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_REJECTED);
}

void ApplicationDetailDialog::connectSignals() {
    connect(mSaveButton, &QPushButton::clicked, this, &ApplicationDetailDialog::onSaveClicked);
    connect(mCancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(mNotesEdit, &QTextEdit::textChanged, this, &ApplicationDetailDialog::onNotesTextChanged);
    connect(mTogglePreviewButton, &QPushButton::clicked, this, &ApplicationDetailDialog::onTogglePreviewClicked);
}

void ApplicationDetailDialog::loadData(const Application &application) {
    mCompanyNameEdit->setText(application.getCompanyName());
    mPositionEdit->setText(application.getPosition());

    int statusIndex = mStatusComboBox->findText(application.getStatus());
    if (statusIndex >= 0) {
        mStatusComboBox->setCurrentIndex(statusIndex);
    }

    mDeadlineEdit->setDate(application.getDeadline());
    mNotesEdit->setPlainText(application.getNotes());
    mNotesPreview->setMarkdown(application.getNotes());
}

bool ApplicationDetailDialog::validateInput() {
    if (mCompanyNameEdit->text().trimmed().isEmpty() ||
        mPositionEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,
                             ApplicationListConstants::MSG_TITLE_ERROR,
                             ApplicationListConstants::MSG_VALIDATION_EMPTY);
        return false;
    }

    if (!mDeadlineEdit->date().isValid()) {
        QMessageBox::warning(this,
                             ApplicationListConstants::MSG_TITLE_ERROR,
                             ApplicationListConstants::MSG_VALIDATION_EMPTY);
        return false;
    }

    return true;
}

void ApplicationDetailDialog::onSaveClicked() {
    if (validateInput()) {
        accept();
    }
}

Application ApplicationDetailDialog::getApplication() const {
    Application app;
    app.setId(mApplicationId);
    app.setCompanyName(mCompanyNameEdit->text().trimmed());
    app.setPosition(mPositionEdit->text().trimmed());
    app.setStatus(mStatusComboBox->currentText());
    app.setDeadline(mDeadlineEdit->date());
    app.setNotes(mNotesEdit->toPlainText());

    return app;
}

void ApplicationDetailDialog::onNotesTextChanged() {
    mNotesPreview->setMarkdown(mNotesEdit->toPlainText());
}

void ApplicationDetailDialog::onTogglePreviewClicked() {
    mPreviewVisible = !mPreviewVisible;
    mNotesPreview->setVisible(mPreviewVisible);
    mTogglePreviewButton->setText(mPreviewVisible ? "미리보기 숨기기" : "미리보기 보기");
}
