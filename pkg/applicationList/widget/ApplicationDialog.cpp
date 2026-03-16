#include "ApplicationDialog.h"
#include "../ApplicationListConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

ApplicationDialog::ApplicationDialog(Mode mode, QWidget *parent)
    : QDialog(parent), mMode(mode), mApplicationId(-1) {
    setupUi();
    setupInputFields();
    connectSignals();
}

ApplicationDialog::ApplicationDialog(Mode mode, const Application &application, QWidget *parent)
    : QDialog(parent), mMode(mode), mApplicationId(application.getId()) {
    setupUi();
    setupInputFields();
    loadData(application);
    connectSignals();
}

void ApplicationDialog::setupUi() {
    if (mMode == ADD) {
        setWindowTitle(ApplicationListConstants::DIALOG_TITLE_ADD);
    } else {
        setWindowTitle(ApplicationListConstants::DIALOG_TITLE_EDIT);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mFormLayout = new QFormLayout();

    mCompanyNameEdit = new QLineEdit(this);
    mPositionEdit = new QLineEdit(this);
    mStatusComboBox = new QComboBox(this);
    mDeadlineEdit = new QDateEdit(this);
    mDeadlineEdit->setCalendarPopup(true);
    mDeadlineEdit->setDate(QDate::currentDate());

    mFormLayout->addRow(ApplicationListConstants::LABEL_COMPANY_NAME, mCompanyNameEdit);
    mFormLayout->addRow(ApplicationListConstants::LABEL_POSITION, mPositionEdit);
    mFormLayout->addRow(ApplicationListConstants::LABEL_STATUS, mStatusComboBox);
    mFormLayout->addRow(ApplicationListConstants::LABEL_DEADLINE, mDeadlineEdit);

    mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    mainLayout->addLayout(mFormLayout);
    mainLayout->addWidget(mButtonBox);

    setLayout(mainLayout);
}

void ApplicationDialog::setupInputFields() {
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_APPLIED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_DOCUMENT_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_CODING_TEST_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FIRST_INTERVIEW_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FINAL_INTERVIEW_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_FINAL_PASSED);
    mStatusComboBox->addItem(ApplicationListConstants::STATUS_REJECTED);
}

void ApplicationDialog::connectSignals() {
    connect(mButtonBox, &QDialogButtonBox::accepted, this, &ApplicationDialog::onAccepted);
    connect(mButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ApplicationDialog::loadData(const Application &application) {
    mCompanyNameEdit->setText(application.getCompanyName());
    mPositionEdit->setText(application.getPosition());

    int statusIndex = mStatusComboBox->findText(application.getStatus());
    if (statusIndex >= 0) {
        mStatusComboBox->setCurrentIndex(statusIndex);
    }

    mDeadlineEdit->setDate(application.getDeadline());
}

bool ApplicationDialog::validateInput() {
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

void ApplicationDialog::onAccepted() {
    if (validateInput()) {
        accept();
    }
}

Application ApplicationDialog::getApplication() const {
    Application app;

    if (mMode == EDIT) {
        app.setId(mApplicationId);
    }

    app.setCompanyName(mCompanyNameEdit->text().trimmed());
    app.setPosition(mPositionEdit->text().trimmed());
    app.setStatus(mStatusComboBox->currentText());
    app.setDeadline(mDeadlineEdit->date());

    return app;
}
