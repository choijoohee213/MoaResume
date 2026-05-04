#include "BasicInfoWidget.h"
#include "../ResumeConstants.h"
#include <QMessageBox>
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
#include <QBitmap>
#include <QDateTime>
#include <QMouseEvent>
#include <QCursor>

static QPixmap makeCircularPixmap(const QPixmap &src, int size) {
    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    // 중앙 크롭
    int x = (scaled.width()  - size) / 2;
    int y = (scaled.height() - size) / 2;
    scaled = scaled.copy(x, y, size, size);

    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(QRegion(0, 0, size, size, QRegion::Ellipse));
    painter.drawPixmap(0, 0, scaled);
    return result;
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

    // 사진 영역
    QHBoxLayout *photoLayout = new QHBoxLayout();
    photoLayout->setSpacing(16);

    mPhotoLabel = new QLabel(this);
    mPhotoLabel->setFixedSize(110, 110);
    mPhotoLabel->setAlignment(Qt::AlignCenter);
    mPhotoLabel->setCursor(Qt::PointingHandCursor);
    mPhotoLabel->setToolTip("클릭하여 사진 선택");
    mPhotoLabel->setStyleSheet(
        "QLabel {"
        "  border: 2px dashed #93C5FD;"
        "  border-radius: 55px;"
        "  background-color: #EFF6FF;"
        "  color: #93C5FD;"
        "  font-size: 11px;"
        "}"
        "QLabel:hover {"
        "  border-color: #60A5FA;"
        "  background-color: #DBEAFE;"
        "}"
    );
    mPhotoLabel->setText("사진 추가\n클릭");
    mPhotoLabel->installEventFilter(this);

    QVBoxLayout *photoHint = new QVBoxLayout();
    QLabel *hintLabel = new QLabel("JPG, PNG 지원\n클릭하여 사진을 선택하세요", this);
    hintLabel->setStyleSheet("color: #93C5FD; font-size: 12px;");
    photoHint->addStretch();
    photoHint->addWidget(hintLabel);
    photoHint->addStretch();

    photoLayout->addWidget(mPhotoLabel);
    photoLayout->addLayout(photoHint);
    photoLayout->addStretch();
    mainLayout->addLayout(photoLayout);

    // 입력 폼
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

bool BasicInfoWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mPhotoLabel && event->type() == QEvent::MouseButtonRelease) {
        onPhotoClicked();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void BasicInfoWidget::setPhoto(const QString &path) {
    if (path.isEmpty()) return;
    QPixmap px(path);
    if (px.isNull()) return;
    mPhotoLabel->setStyleSheet(
        "QLabel {"
        "  border: 2px solid #93C5FD;"
        "  border-radius: 55px;"
        "  background-color: transparent;"
        "}"
        "QLabel:hover {"
        "  border-color: #60A5FA;"
        "}"
    );
    mPhotoLabel->setPixmap(makeCircularPixmap(px, 110));
    mPhotoLabel->setText("");
}

void BasicInfoWidget::loadData() {
    for (const ResumeCategory &cat : mService.getCategories()) {
        if (cat.getType() == CategoryType::BasicInfo) {
            mCategoryId = cat.getId();
            break;
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
        if (value.isEmpty()) continue;
        if (field.type == FieldDefinition::Text && mLineEdits.contains(field.key)) {
            mLineEdits[field.key]->setText(value);
        } else if (field.type == FieldDefinition::Date && mDateEdits.contains(field.key)) {
            QDate date = QDate::fromString(value, "yyyy-MM-dd");
            if (date.isValid()) mDateEdits[field.key]->setDate(date);
        }
    }

    mPhotoPath = item.getField("photoPath");
    setPhoto(mPhotoPath);
}

void BasicInfoWidget::activate() {
    for (auto it = mLineEdits.begin(); it != mLineEdits.end(); ++it)
        it.value()->clear();
    mPhotoPath.clear();
    mPhotoLabel->setPixmap(QPixmap());
    mPhotoLabel->setText("사진 추가\n클릭");
    mPhotoLabel->setStyleSheet(
        "QLabel {"
        "  border: 2px dashed #93C5FD;"
        "  border-radius: 55px;"
        "  background-color: #EFF6FF;"
        "  color: #93C5FD;"
        "  font-size: 11px;"
        "}"
        "QLabel:hover {"
        "  border-color: #60A5FA;"
        "  background-color: #DBEAFE;"
        "}"
    );
    loadData();
}

void BasicInfoWidget::connectSignals() {
    connect(mSaveButton, &QPushButton::clicked, this, &BasicInfoWidget::onSaveClicked);
}

void BasicInfoWidget::onPhotoClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "사진 선택", QString(),
        "이미지 파일 (*.jpg *.jpeg *.png *.bmp);;모든 파일 (*)");
    if (filePath.isEmpty()) return;

    QString dataDir   = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString photoDir  = dataDir + "/photos";
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
    setPhoto(mPhotoPath);
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
    item.setField("photoPath", mPhotoPath);

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
