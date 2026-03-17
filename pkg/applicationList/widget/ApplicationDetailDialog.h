#ifndef APPLICATIONDETAILDIALOG_H
#define APPLICATIONDETAILDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFormLayout>
#include "../model/Application.h"

class ApplicationDetailDialog : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationDetailDialog(const Application &application, QWidget *parent = nullptr);
    ~ApplicationDetailDialog() = default;

    Application getApplication() const;

private:
    void setupUi();
    void setupInputFields();
    void connectSignals();
    void loadData(const Application &application);
    bool validateInput();

private slots:
    void onSaveClicked();

private:
    int mApplicationId;

    QLineEdit *mCompanyNameEdit;
    QLineEdit *mPositionEdit;
    QComboBox *mStatusComboBox;
    QDateEdit *mDeadlineEdit;
    QTextEdit *mNotesEdit;

    QPushButton *mSaveButton;
    QPushButton *mCancelButton;
};

#endif
