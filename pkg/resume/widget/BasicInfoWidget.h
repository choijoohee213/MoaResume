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
    void setViewMode(bool viewMode);
    void setPhoto(const QString &path, bool editable);
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onEditClicked();
    void onSaveClicked();
    void onCancelClicked();
    void onPhotoClicked();

private:
    QLabel         *mPhotoLabel;
    QWidget        *mViewButtons;
    QWidget        *mEditButtons;

    QMap<QString, QLineEdit*> mLineEdits;
    QMap<QString, QDateEdit*> mDateEdits;
    QList<FieldDefinition>    mFieldDefs;

    // 취소 시 복원용
    QMap<QString, QString> mOriginalFields;
    QString mOriginalPhotoPath;

    QString        mPhotoPath;
    ResumeService &mService;
    int mCategoryId{-1};
    int mItemId{-1};
};

#endif // BASICINFOWIDGET_H
