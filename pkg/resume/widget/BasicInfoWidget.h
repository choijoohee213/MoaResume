#ifndef BASICINFOWIDGET_H
#define BASICINFOWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
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
    void setPhoto(const QString &path);
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onSaveClicked();
    void onPhotoClicked();

private:
    QLabel      *mTitleLabel;
    QLabel      *mPhotoLabel;
    QFormLayout *mFormLayout;
    QPushButton *mSaveButton;

    QMap<QString, QLineEdit*> mLineEdits;
    QMap<QString, QDateEdit*> mDateEdits;
    QList<FieldDefinition>    mFieldDefs;

    QString        mPhotoPath;
    ResumeService &mService;
    int mCategoryId{-1};
    int mItemId{-1};
};

#endif // BASICINFOWIDGET_H
