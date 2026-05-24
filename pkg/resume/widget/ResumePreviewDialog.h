#ifndef RESUMEPREVIEWDIALOG_H
#define RESUMEPREVIEWDIALOG_H

#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../service/ResumeService.h"

class ResumePreviewDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResumePreviewDialog(QWidget *parent = nullptr);

private:
    void setupUi();
    void loadPreview();

private slots:
    void onExportPdf();

private:
    QTextBrowser *mBrowser;
    QPushButton  *mExportButton;
    QString       mHtml;
};

#endif // RESUMEPREVIEWDIALOG_H
