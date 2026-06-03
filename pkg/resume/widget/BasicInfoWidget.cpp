#include "BasicInfoWidget.h"
#include "../ResumeConstants.h"
#include <QHBoxLayout>
#include <QFont>
#include <QFrame>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QPixmap>
#include <QPainter>
#include <QDateTime>
#include <QEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

static const QString VIEW_EDIT_STYLE =
    "QLineEdit, QDateEdit {"
    "  background-color: #ffffff;"
    "  border: 1.5px solid #BFDBFE;"
    "  border-radius: 8px;"
    "  padding: 6px 10px;"
    "  color: #374151;"
    "  min-height: 32px;"
    "}"
    "QLineEdit:focus, QDateEdit:focus {"
    "  border-color: #60A5FA;"
    "  background-color: #F0F9FF;"
    "}";

static const QString VIEW_ONLY_STYLE =
    "QLineEdit, QDateEdit {"
    "  background: transparent;"
    "  border: none;"
    "  padding: 6px 2px;"
    "  color: #374151;"
    "  min-height: 32px;"
    "}"
    "QDateEdit::drop-down { width: 0; border: none; }"
    "QDateEdit::down-arrow { width: 0; height: 0; }";

static QPixmap makeCircularPixmap(const QPixmap &src, int size) {
    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width()  - size) / 2;
    int y = (scaled.height() - size) / 2;
    scaled = scaled.copy(x, y, size, size);
    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter p(&result);
    p.setRenderHint(QPainter::Antialiasing);
    p.setClipRegion(QRegion(0, 0, size, size, QRegion::Ellipse));
    p.drawPixmap(0, 0, scaled);
    return result;
}

static QString toHref(const QString &url) {
    if (url.startsWith("http://") || url.startsWith("https://"))
        return url;
    return "https://" + url;
}

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

    QLabel *titleLabel = new QLabel("기본정보", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    divider->setStyleSheet("color: #BFDBFE;");
    mainLayout->addWidget(divider);

    // 사진
    QHBoxLayout *photoLayout = new QHBoxLayout();
    mPhotoLabel = new QLabel(this);
    mPhotoLabel->setFixedSize(110, 110);
    mPhotoLabel->setAlignment(Qt::AlignCenter);
    mPhotoLabel->installEventFilter(this);
    photoLayout->addWidget(mPhotoLabel);
    photoLayout->addStretch();
    mainLayout->addLayout(photoLayout);

    // 폼
    QWidget *formContainer = new QWidget(this);
    QFormLayout *formLayout = new QFormLayout(formContainer);
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setHorizontalSpacing(16);
    formLayout->setVerticalSpacing(12);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    // github, portfolio 관련 키는 아래에서 수동으로 처리
    static const QStringList manualKeys = {"github", "portfolio", "portfolioType", "portfolioFile"};

    for (const FieldDefinition &field : mFieldDefs) {
        if (manualKeys.contains(field.key)) continue;
        if (field.type == FieldDefinition::Text) {
            QLineEdit *edit = new QLineEdit(formContainer);
            edit->setMinimumHeight(32);
            mLineEdits[field.key] = edit;
            formLayout->addRow(field.label + ":", edit);
        } else if (field.type == FieldDefinition::Date) {
            QDateEdit *edit = new QDateEdit(formContainer);
            edit->setCalendarPopup(true);
            edit->setDisplayFormat("yyyy-MM-dd");
            edit->setDate(QDate::currentDate());
            edit->setSpecialValueText(" ");
            edit->setMinimumDate(QDate(1900, 1, 1));
            edit->setMinimumHeight(32);
            mDateEdits[field.key] = edit;
            formLayout->addRow(field.label + ":", edit);
        }
    }

    // --- GitHub 행 ---
    {
        QWidget *container = new QWidget(formContainer);
        QHBoxLayout *hl = new QHBoxLayout(container);
        hl->setContentsMargins(0, 0, 0, 0);
        hl->setSpacing(0);

        QLineEdit *edit = new QLineEdit(container);
        edit->setMinimumHeight(32);
        mLineEdits["github"] = edit;

        mGithubLinkLabel = new QLabel(container);
        mGithubLinkLabel->setOpenExternalLinks(true);
        mGithubLinkLabel->setTextFormat(Qt::RichText);
        mGithubLinkLabel->setAlignment(Qt::AlignVCenter);
        mGithubLinkLabel->setMinimumHeight(32);
        mGithubLinkLabel->setVisible(false);

        hl->addWidget(edit);
        hl->addWidget(mGithubLinkLabel);
        formLayout->addRow("GitHub:", container);
    }

    // --- 포트폴리오 행 ---
    {
        QWidget *container = new QWidget(formContainer);
        QVBoxLayout *vl = new QVBoxLayout(container);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(6);

        // 유형 선택 (편집 모드)
        mPortfolioTypeCombo = new QComboBox(container);
        mPortfolioTypeCombo->addItems({"링크", "파일"});
        vl->addWidget(mPortfolioTypeCombo);

        // URL 입력 (편집 모드 + 링크 유형)
        mPortfolioUrlEdit = new QLineEdit(container);
        mPortfolioUrlEdit->setMinimumHeight(32);
        mPortfolioUrlEdit->setPlaceholderText("https://");
        mLineEdits["portfolio"] = mPortfolioUrlEdit;
        vl->addWidget(mPortfolioUrlEdit);

        // 파일 선택 (편집 모드 + 파일 유형)
        mPortfolioFileWidget = new QWidget(container);
        QHBoxLayout *fl = new QHBoxLayout(mPortfolioFileWidget);
        fl->setContentsMargins(0, 0, 0, 0);
        fl->setSpacing(8);
        mPortfolioFileNameLabel = new QLabel("파일 없음", mPortfolioFileWidget);
        mPortfolioFileNameLabel->setStyleSheet("color: #6B7280;");
        mPortfolioFileBtn = new QPushButton("파일 선택", mPortfolioFileWidget);
        fl->addWidget(mPortfolioFileNameLabel, 1);
        fl->addWidget(mPortfolioFileBtn);
        mPortfolioFileWidget->setVisible(false);
        vl->addWidget(mPortfolioFileWidget);

        // 링크 표시 (뷰 모드 + 링크 유형)
        mPortfolioLinkLabel = new QLabel(container);
        mPortfolioLinkLabel->setOpenExternalLinks(true);
        mPortfolioLinkLabel->setTextFormat(Qt::RichText);
        mPortfolioLinkLabel->setAlignment(Qt::AlignVCenter);
        mPortfolioLinkLabel->setMinimumHeight(32);
        mPortfolioLinkLabel->setVisible(false);
        vl->addWidget(mPortfolioLinkLabel);

        // 파일 열기 (뷰 모드 + 파일 유형)
        mPortfolioOpenFileBtn = new QPushButton(container);
        mPortfolioOpenFileBtn->setVisible(false);
        vl->addWidget(mPortfolioOpenFileBtn);

        formLayout->addRow("포트폴리오:", container);

        connect(mPortfolioTypeCombo, &QComboBox::currentTextChanged,
                this, &BasicInfoWidget::onPortfolioTypeChanged);
        connect(mPortfolioFileBtn, &QPushButton::clicked,
                this, &BasicInfoWidget::onPortfolioFileClicked);
        connect(mPortfolioOpenFileBtn, &QPushButton::clicked, this, [this]() {
            if (!mPortfolioFilePath.isEmpty())
                QDesktopServices::openUrl(QUrl::fromLocalFile(mPortfolioFilePath));
        });
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(formContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea, 1);

    // 버튼 - 뷰 모드
    mViewButtons = new QWidget(this);
    QHBoxLayout *viewBtnLayout = new QHBoxLayout(mViewButtons);
    viewBtnLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *editBtn = new QPushButton("수정", mViewButtons);
    editBtn->setMinimumSize(100, 36);
    connect(editBtn, &QPushButton::clicked, this, &BasicInfoWidget::onEditClicked);
    viewBtnLayout->addStretch();
    viewBtnLayout->addWidget(editBtn);

    // 버튼 - 편집 모드
    mEditButtons = new QWidget(this);
    QHBoxLayout *editBtnLayout = new QHBoxLayout(mEditButtons);
    editBtnLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *saveBtn   = new QPushButton("저장", mEditButtons);
    QPushButton *cancelBtn = new QPushButton("취소", mEditButtons);
    saveBtn->setMinimumSize(100, 36);
    cancelBtn->setMinimumSize(100, 36);
    connect(saveBtn,   &QPushButton::clicked, this, &BasicInfoWidget::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &BasicInfoWidget::onCancelClicked);
    editBtnLayout->addStretch();
    editBtnLayout->addWidget(saveBtn);
    editBtnLayout->addWidget(cancelBtn);

    mainLayout->addWidget(mViewButtons);
    mainLayout->addWidget(mEditButtons);
    setLayout(mainLayout);
}

void BasicInfoWidget::setViewMode(bool viewMode) {
    for (auto it = mLineEdits.begin(); it != mLineEdits.end(); ++it) {
        it.value()->setReadOnly(viewMode);
        it.value()->setStyleSheet(viewMode ? VIEW_ONLY_STYLE : "");
    }
    for (auto it = mDateEdits.begin(); it != mDateEdits.end(); ++it) {
        it.value()->setReadOnly(viewMode);
        it.value()->setButtonSymbols(viewMode
            ? QAbstractSpinBox::NoButtons
            : QAbstractSpinBox::UpDownArrows);
        it.value()->setStyleSheet(viewMode ? VIEW_ONLY_STYLE : "");
    }

    mViewButtons->setVisible(viewMode);
    mEditButtons->setVisible(!viewMode);
    setPhoto(mPhotoPath, !viewMode);

    // GitHub 링크
    QString githubUrl = mLineEdits["github"]->text().trimmed();
    if (viewMode && !githubUrl.isEmpty()) {
        mLineEdits["github"]->setVisible(false);
        mGithubLinkLabel->setText(QString(
            "<a href='%1' style='color:#3B82F6;'>%2</a>")
            .arg(toHref(githubUrl), githubUrl.toHtmlEscaped()));
        mGithubLinkLabel->setVisible(true);
    } else {
        mLineEdits["github"]->setVisible(true);
        mGithubLinkLabel->setVisible(false);
    }

    // 포트폴리오
    QString pType = mPortfolioTypeCombo->currentText();
    mPortfolioTypeCombo->setVisible(!viewMode);

    if (viewMode) {
        mPortfolioUrlEdit->setVisible(false);
        mPortfolioFileWidget->setVisible(false);

        if (pType == "파일" && !mPortfolioFilePath.isEmpty()) {
            mPortfolioOpenFileBtn->setText("  " + QFileInfo(mPortfolioFilePath).fileName() + "  열기");
            mPortfolioOpenFileBtn->setVisible(true);
            mPortfolioLinkLabel->setVisible(false);
        } else {
            QString pUrl = mPortfolioUrlEdit->text().trimmed();
            if (!pUrl.isEmpty()) {
                mPortfolioLinkLabel->setText(QString(
                    "<a href='%1' style='color:#3B82F6;'>%2</a>")
                    .arg(toHref(pUrl), pUrl.toHtmlEscaped()));
                mPortfolioLinkLabel->setVisible(true);
            } else {
                mPortfolioLinkLabel->setVisible(false);
            }
            mPortfolioOpenFileBtn->setVisible(false);
        }
    } else {
        mPortfolioLinkLabel->setVisible(false);
        mPortfolioOpenFileBtn->setVisible(false);
        onPortfolioTypeChanged(pType);
    }
}

void BasicInfoWidget::setPhoto(const QString &path, bool editable) {
    mPhotoLabel->setCursor(editable ? Qt::PointingHandCursor : Qt::ArrowCursor);
    mPhotoLabel->setToolTip(editable ? "클릭하여 사진 변경" : "");

    if (path.isEmpty()) {
        mPhotoLabel->setPixmap(QPixmap());
        if (editable) {
            mPhotoLabel->setText("사진 추가\n클릭");
            mPhotoLabel->setStyleSheet(
                "QLabel { border:2px dashed #93C5FD; border-radius:55px;"
                "  background:#EFF6FF; color:#93C5FD; font-size:11px; }"
                "QLabel:hover { border-color:#60A5FA; background:#DBEAFE; }");
        } else {
            mPhotoLabel->setText("사진 없음");
            mPhotoLabel->setStyleSheet(
                "QLabel { border:2px dashed #BFDBFE; border-radius:55px;"
                "  background:#F0F9FF; color:#BFDBFE; font-size:11px; }");
        }
    } else {
        QPixmap px(path);
        if (!px.isNull()) {
            mPhotoLabel->setText("");
            mPhotoLabel->setPixmap(makeCircularPixmap(px, 110));
            mPhotoLabel->setStyleSheet(
                "QLabel { border:2px solid #93C5FD; border-radius:55px; background:transparent; }"
                + QString(editable ? "QLabel:hover { border-color:#60A5FA; }" : ""));
        }
    }
}

void BasicInfoWidget::loadData() {
    if (mCategoryId < 0) {
        for (const ResumeCategory &cat : mService.getCategories()) {
            if (cat.getType() == CategoryType::BasicInfo) {
                mCategoryId = cat.getId();
                break;
            }
        }
    }
    if (mCategoryId < 0) return;

    ResumeCategory cat = mService.getCategoryById(mCategoryId);
    if (cat.getItems().isEmpty()) {
        mService.addItem(mCategoryId, ResumeItem());
        cat = mService.getCategoryById(mCategoryId);
    }
    ResumeItem item = cat.getItems().first();
    mItemId = item.getId();

    for (const FieldDefinition &field : mFieldDefs) {
        QString value = item.getField(field.key);
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key)) {
            mLineEdits[field.key]->setText(value);
        } else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate date = QDate::fromString(value, "yyyy-MM-dd");
            if (date.isValid()) mDateEdits[field.key]->setDate(date);
        }
    }

    // 포트폴리오 유형
    QString portfolioType = item.getField("portfolioType");
    if (portfolioType.isEmpty()) portfolioType = "링크";
    int typeIdx = mPortfolioTypeCombo->findText(portfolioType);
    if (typeIdx >= 0) mPortfolioTypeCombo->setCurrentIndex(typeIdx);

    // 포트폴리오 파일
    mPortfolioFilePath = item.getField("portfolioFile");
    if (!mPortfolioFilePath.isEmpty())
        mPortfolioFileNameLabel->setText(QFileInfo(mPortfolioFilePath).fileName());
    else
        mPortfolioFileNameLabel->setText("파일 없음");

    mPhotoPath = item.getField("photoPath");
    setViewMode(true);
}

void BasicInfoWidget::activate() {
    loadData();
}

void BasicInfoWidget::connectSignals() {}

bool BasicInfoWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mPhotoLabel && event->type() == QEvent::MouseButtonRelease) {
        if (!mViewButtons->isVisible())
            onPhotoClicked();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void BasicInfoWidget::onEditClicked() {
    for (const FieldDefinition &field : mFieldDefs) {
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key))
            mOriginalFields[field.key] = mLineEdits[field.key]->text();
        else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key))
            mOriginalFields[field.key] = mDateEdits[field.key]->date().toString("yyyy-MM-dd");
    }
    mOriginalPhotoPath = mPhotoPath;
    mOriginalPortfolioFilePath = mPortfolioFilePath;
    mOriginalPortfolioType = mPortfolioTypeCombo->currentText();
    setViewMode(false);
}

void BasicInfoWidget::onCancelClicked() {
    for (const FieldDefinition &field : mFieldDefs) {
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key))
            mLineEdits[field.key]->setText(mOriginalFields.value(field.key));
        else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate d = QDate::fromString(mOriginalFields.value(field.key), "yyyy-MM-dd");
            if (d.isValid()) mDateEdits[field.key]->setDate(d);
        }
    }
    mPhotoPath = mOriginalPhotoPath;
    mPortfolioFilePath = mOriginalPortfolioFilePath;
    int idx = mPortfolioTypeCombo->findText(mOriginalPortfolioType);
    if (idx >= 0) mPortfolioTypeCombo->setCurrentIndex(idx);
    if (!mPortfolioFilePath.isEmpty())
        mPortfolioFileNameLabel->setText(QFileInfo(mPortfolioFilePath).fileName());
    else
        mPortfolioFileNameLabel->setText("파일 없음");
    setViewMode(true);
}

void BasicInfoWidget::onPhotoClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "사진 선택", QString(),
        "이미지 파일 (*.jpg *.jpeg *.png *.bmp);;모든 파일 (*)");
    if (filePath.isEmpty()) return;

    QString dataDir  = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString photoDir = dataDir + "/photos";
    QDir().mkpath(photoDir);

    QFileInfo fi(filePath);
    QString destPath = photoDir + "/" + fi.fileName();
    if (QFile::exists(destPath) && destPath != filePath) {
        QString ts = QString::number(QDateTime::currentMSecsSinceEpoch());
        destPath = photoDir + "/" + fi.completeBaseName() + "_" + ts
                   + (fi.suffix().isEmpty() ? "" : "." + fi.suffix());
    }
    if (filePath != destPath)
        QFile::copy(filePath, destPath);

    mPhotoPath = (filePath != destPath) ? destPath : filePath;
    setPhoto(mPhotoPath, true);
}

void BasicInfoWidget::onPortfolioTypeChanged(const QString &type) {
    bool isFile = (type == "파일");
    mPortfolioUrlEdit->setVisible(!isFile);
    mPortfolioFileWidget->setVisible(isFile);
}

void BasicInfoWidget::onPortfolioFileClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "포트폴리오 파일 선택", QString(),
        "문서 파일 (*.pdf *.ppt *.pptx *.doc *.docx);;모든 파일 (*)");
    if (filePath.isEmpty()) return;

    QString dataDir      = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString portfolioDir = dataDir + "/portfolio";
    QDir().mkpath(portfolioDir);

    QFileInfo fi(filePath);
    QString destPath = portfolioDir + "/" + fi.fileName();
    if (QFile::exists(destPath) && destPath != filePath) {
        QString ts = QString::number(QDateTime::currentMSecsSinceEpoch());
        destPath = portfolioDir + "/" + fi.completeBaseName() + "_" + ts
                   + (fi.suffix().isEmpty() ? "" : "." + fi.suffix());
    }
    if (filePath != destPath)
        QFile::copy(filePath, destPath);

    mPortfolioFilePath = (filePath != destPath) ? destPath : filePath;
    mPortfolioFileNameLabel->setText(fi.fileName());
}

void BasicInfoWidget::onSaveClicked() {
    if (mCategoryId < 0) return;

    ResumeItem item;
    item.setId(mItemId);
    for (const FieldDefinition &field : mFieldDefs) {
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key))
            item.setField(field.key, mLineEdits[field.key]->text());
        else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate d = mDateEdits[field.key]->date();
            item.setField(field.key, d.isValid() ? d.toString("yyyy-MM-dd") : "");
        }
    }
    item.setField("photoPath",     mPhotoPath);
    item.setField("portfolioType", mPortfolioTypeCombo->currentText());
    item.setField("portfolioFile", mPortfolioFilePath);

    if (mItemId < 0) {
        mService.addItem(mCategoryId, item);
        ResumeCategory cat = mService.getCategoryById(mCategoryId);
        if (!cat.getItems().isEmpty())
            mItemId = cat.getItems().first().getId();
    } else {
        mService.updateItem(mCategoryId, item);
    }

    setViewMode(true);
}
