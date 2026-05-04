#ifndef BASICINFOWIDGET_H
#define BASICINFOWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>
#include "../service/ResumeService.h"
#include "../config/ResumeCategoryConfig.h"

class BasicInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit BasicInfoWidget(QWidget *parent = nullptr);

    void activate();

private:
    void setupUi();
    void loadData();
    void connectSignals();

private slots:
    void onSaveClicked();

private:
    QLabel      *mTitleLabel;
    QFormLayout *mFormLayout;
    QPushButton *mSaveButton;

    QMap<QString, QLineEdit*> mLineEdits;
    QMap<QString, QDateEdit*> mDateEdits;
    QList<FieldDefinition>    mFieldDefs;

    ResumeService &mService;
    int mCategoryId{-1};
    int mItemId{-1};
};

#endif // BASICINFOWIDGET_H
