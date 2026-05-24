#include "ResumePreviewDialog.h"
#include "../ResumeHtmlBuilder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPrinter>
#include <QPageSize>
#include <QTextDocument>

ResumePreviewDialog::ResumePreviewDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi();
    loadPreview();
    connect(mExportButton, &QPushButton::clicked, this, &ResumePreviewDialog::onExportPdf);
}

void ResumePreviewDialog::setupUi() {
    setWindowTitle("이력서 미리보기");
    resize(720, 960);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 8);
    layout->setSpacing(0);

    mBrowser = new QTextBrowser(this);
    mBrowser->setOpenExternalLinks(true);
    mBrowser->setStyleSheet(
        "QTextBrowser {"
        "  background-color: #ffffff;"
        "  border: none;"
        "  padding: 32px 48px;"
        "}"
    );
    layout->addWidget(mBrowser, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(16, 0, 16, 0);
    mExportButton = new QPushButton("PDF로 저장", this);
    mExportButton->setMinimumSize(130, 36);
    QPushButton *closeBtn = new QPushButton("닫기", this);
    closeBtn->setMinimumSize(80, 36);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addStretch();
    btnLayout->addWidget(mExportButton);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    setLayout(layout);
}

void ResumePreviewDialog::loadPreview() {
    mHtml = ResumeHtmlBuilder::build(ResumeService::getInstance());
    mBrowser->setHtml(mHtml);
}

void ResumePreviewDialog::onExportPdf() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "PDF 저장", "이력서.pdf", "PDF 파일 (*.pdf)");
    if (filePath.isEmpty()) return;

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

    QMessageBox::information(this, "저장 완료",
        QString("PDF가 저장되었습니다:\n%1").arg(filePath));
}
