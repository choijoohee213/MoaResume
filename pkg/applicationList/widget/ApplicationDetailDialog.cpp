#include "ApplicationDetailDialog.h"
#include "../ApplicationListConstants.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

ApplicationDetailDialog::ApplicationDetailDialog(const Application &application, QWidget *parent)
    : QDialog(parent), mApplicationId(application.getId()), mPreviewVisible(true) {
    setupUi();

    setupInputFields();

    loadData(application);

    connectSignals();
}

void ApplicationDetailDialog::setupUi() {
    setWindowTitle(ApplicationListConstants::DIALOG_TITLE_DETAIL);
    setMinimumSize(800, 700);
    resize(900, 750);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);

    mCompanyNameEdit = new QLineEdit(this);
    mPositionEdit = new QLineEdit(this);
    mStatusComboBox = new QComboBox(this);
    mDeadlineEdit = new QDateEdit(this);
    mDeadlineEdit->setCalendarPopup(true);

    mLogoPreview = new QLabel(this);
    mLogoPreview->setFixedSize(64, 64);
    mLogoPreview->setScaledContents(true);
    mLogoPreview->setFrameShape(QFrame::Box);
    mLogoPreview->setText("없음");
    mLogoPreview->setAlignment(Qt::AlignCenter);

    mLogoUploadButton = new QPushButton("업로드", this);
    mLogoUploadButton->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *logoLayout = new QHBoxLayout();
    logoLayout->setContentsMargins(0, 0, 0, 0);
    logoLayout->addWidget(mLogoPreview);
    logoLayout->addWidget(mLogoUploadButton);
    logoLayout->addStretch();

    QWidget *logoWidget = new QWidget(this);
    logoWidget->setLayout(logoLayout);

    formLayout->addRow(ApplicationListConstants::LABEL_LOGO, logoWidget);
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
    mNotesSplitter->setMinimumHeight(200);

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
    connect(mLogoUploadButton, &QPushButton::clicked, this, &ApplicationDetailDialog::onLogoUploadClicked);
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

    mLogoPath = application.getLogoPath();
    if (!mLogoPath.isEmpty()) {
        QPixmap pixmap(mLogoPath);
        if (!pixmap.isNull()) {
            mLogoPreview->setPixmap(pixmap);
            mLogoPreview->setText("");
        }
    }
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
    app.setLogoPath(mLogoPath);

    return app;
}

void ApplicationDetailDialog::onNotesTextChanged() {
    mNotesPreview->setMarkdown(mNotesEdit->toPlainText());
}

void ApplicationDetailDialog::onLogoUploadClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "로고 이미지 선택",
        "",
        "이미지 파일 (*.png *.jpg *.jpeg *.webp *.svg)"
    );
    if (filePath.isEmpty()) return;

    QPixmap pixmap(filePath);
    if (!pixmap.isNull()) {
        mLogoPath = filePath;
        mLogoPreview->setPixmap(pixmap);
        mLogoPreview->setText("");
    }
}

void ApplicationDetailDialog::onTogglePreviewClicked() {
    mPreviewVisible = !mPreviewVisible;
    mNotesPreview->setVisible(mPreviewVisible);
    mTogglePreviewButton->setText(mPreviewVisible ? "미리보기 숨기기" : "미리보기 보기");
}
