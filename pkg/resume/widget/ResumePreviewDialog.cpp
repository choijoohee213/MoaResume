#include "ResumePreviewDialog.h"
#include "../ResumeHtmlBuilder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPrinter>
#include <QPageSize>
#include <QTextDocument>
#include <QApplication>
#include <QProgressDialog>
#include <QScrollArea>
#include <QSet>

ResumePreviewDialog::ResumePreviewDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi();
    loadCategories();
    refreshPreview();
    connect(mExportButton, &QPushButton::clicked, this, &ResumePreviewDialog::onExportPdf);
}

void ResumePreviewDialog::setupUi() {
    setWindowTitle("이력서 미리보기");
    resize(960, 960);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 좌측 체크리스트 패널
    QWidget *leftPanel = new QWidget(this);
    leftPanel->setFixedWidth(200);
    leftPanel->setStyleSheet("background-color: #F0F9FF; border-right: 1px solid #BFDBFE;");

    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(12, 16, 12, 16);
    leftLayout->setSpacing(4);

    QLabel *panelTitle = new QLabel("포함 섹션", leftPanel);
    panelTitle->setStyleSheet(
        "font-weight: bold; font-size: 11pt; color: #1E3A5F;"
        "padding-bottom: 8px; border-bottom: 1px solid #BFDBFE;");
    leftLayout->addWidget(panelTitle);

    QScrollArea *scroll = new QScrollArea(leftPanel);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("background: transparent;");

    mChecklistWidget = new QWidget(scroll);
    mChecklistWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *checkLayout = new QVBoxLayout(mChecklistWidget);
    checkLayout->setContentsMargins(0, 8, 0, 0);
    checkLayout->setSpacing(8);

    scroll->setWidget(mChecklistWidget);
    leftLayout->addWidget(scroll, 1);
    mainLayout->addWidget(leftPanel);

    // 우측 미리보기 + 버튼 영역
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 8);
    rightLayout->setSpacing(0);

    mBrowser = new QTextBrowser(rightPanel);
    mBrowser->setOpenExternalLinks(true);
    mBrowser->setStyleSheet(
        "QTextBrowser {"
        "  background-color: #ffffff;"
        "  border: none;"
        "  padding: 32px 48px;"
        "}");
    rightLayout->addWidget(mBrowser, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(16, 0, 16, 0);
    mExportButton = new QPushButton("PDF로 저장", rightPanel);
    mExportButton->setMinimumSize(130, 36);
    QPushButton *closeBtn = new QPushButton("닫기", rightPanel);
    closeBtn->setMinimumSize(80, 36);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addStretch();
    btnLayout->addWidget(mExportButton);
    btnLayout->addWidget(closeBtn);
    rightLayout->addLayout(btnLayout);

    mainLayout->addWidget(rightPanel, 1);
    setLayout(mainLayout);
}

void ResumePreviewDialog::loadCategories() {
    QList<ResumeCategory> cats = ResumeService::getInstance().getCategories();
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(mChecklistWidget->layout());

    for (const ResumeCategory &cat : cats) {
        bool hasData = (cat.getType() == CategoryType::BasicInfo)
                       ? true
                       : !cat.getItems().isEmpty();

        QString label = cat.getName();
        if (cat.getType() != CategoryType::BasicInfo) {
            int count = cat.getItems().size();
            label += count > 0
                ? QString(" (%1)").arg(count)
                : " (없음)";
        }

        QCheckBox *cb = new QCheckBox(label, mChecklistWidget);
        cb->setChecked(hasData);
        cb->setStyleSheet(hasData
            ? "color: #1E3A5F;"
            : "color: #9CA3AF;");

        connect(cb, &QCheckBox::toggled, this, &ResumePreviewDialog::refreshPreview);

        mCheckboxes.append(cb);
        mCategoryIds.append(cat.getId());
        layout->addWidget(cb);
    }
    layout->addStretch();
}

void ResumePreviewDialog::refreshPreview() {
    mBrowser->setHtml("<p style='color:#9CA3AF; text-align:center; margin-top:40px;'>불러오는 중...</p>");
    QApplication::processEvents();

    QSet<int> includedIds;
    for (int i = 0; i < mCheckboxes.size(); ++i) {
        if (mCheckboxes[i]->isChecked())
            includedIds.insert(mCategoryIds[i]);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mHtml = ResumeHtmlBuilder::build(ResumeService::getInstance(), includedIds);
    mBrowser->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

void ResumePreviewDialog::onExportPdf() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "PDF 저장", "이력서.pdf", "PDF 파일 (*.pdf)");
    if (filePath.isEmpty()) return;

    mExportButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QProgressDialog progress("PDF를 저장하는 중...", QString(), 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(300);
    progress.show();
    QApplication::processEvents();

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument doc;
    QFont defaultFont("Apple SD Gothic Neo", 10);
    doc.setDefaultFont(defaultFont);
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());
    doc.setHtml(mHtml);
    doc.print(&printer);

    progress.close();
    QApplication::restoreOverrideCursor();
    mExportButton->setEnabled(true);

    QMessageBox::information(this, "저장 완료",
        QString("PDF가 저장되었습니다:\n%1").arg(filePath));
}
