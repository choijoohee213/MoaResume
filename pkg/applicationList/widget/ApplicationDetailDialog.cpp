#include "ApplicationDetailDialog.h"
#include "../ApplicationListConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ApplicationDetailDialog::ApplicationDetailDialog(const Application &application, QWidget *parent)
    : QDialog(parent), mApplicationId(application.getId()) {
    setupUi();
    setupInputFields();
    loadData(application);
    connectSignals();
}

void ApplicationDetailDialog::setupUi() {
    setWindowTitle(ApplicationListConstants::DIALOG_TITLE_DETAIL);
    setMinimumSize(500, 600);

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

    mNotesEdit = new QTextEdit(this);
    mNotesEdit->setAcceptRichText(false);
    mNotesEdit->setMinimumHeight(300);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mSaveButton = new QPushButton(ApplicationListConstants::BTN_TEXT_SAVE, this);
    mCancelButton = new QPushButton(ApplicationListConstants::BTN_TEXT_CANCEL, this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(mSaveButton);
    buttonLayout->addWidget(mCancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(mNotesEdit);
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
