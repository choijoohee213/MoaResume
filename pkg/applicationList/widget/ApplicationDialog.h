#ifndef APPLICATIONDIALOG_H
#define APPLICATIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "../model/Application.h"

class ApplicationDialog : public QDialog {
    Q_OBJECT

public:
    enum Mode {
        ADD,
        EDIT
    };

    explicit ApplicationDialog(Mode mode, QWidget *parent = nullptr);
    explicit ApplicationDialog(Mode mode, const Application &application, QWidget *parent = nullptr);
    ~ApplicationDialog() = default;

    Application getApplication() const;

private:
    void setupUi();
    void setupInputFields();
    void connectSignals();
    void loadData(const Application &application);
    bool validateInput();

private slots:
    void onAccepted();

private:
    Mode mMode;
    int mApplicationId;

    QFormLayout *mFormLayout;
    QLineEdit *mCompanyNameEdit;
    QLineEdit *mPositionEdit;
    QComboBox *mStatusComboBox;
    QDateEdit *mDeadlineEdit;
    QDialogButtonBox *mButtonBox;
};

#endif
