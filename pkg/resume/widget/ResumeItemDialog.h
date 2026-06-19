#ifndef RESUMEITEMDIALOG_H
#define RESUMEITEMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QMap>
#include "../model/ResumeItem.h"
#include "../ResumeConstants.h"
#include "../config/ResumeCategoryConfig.h"

class ResumeItemDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResumeItemDialog(const ResumeItem &item, CategoryType categoryType, QWidget *parent = nullptr);
    ~ResumeItemDialog() = default;

    ResumeItem getItem() const;

private:
    void setupUi();
    void setupEducationUi();
    void rebuildEducationFields(const QString &eduType);
    void clearDynamicRows();
    void addFieldRow(const FieldDefinition &field);
    void connectSignals();
    void loadData(const ResumeItem &item);
    bool validateInput();
    QString getFieldValue(const FieldDefinition &field) const;

private slots:
    void onSaveClicked();
    void onEduTypeChanged(const QString &type);
    void onMinorTypeChanged(const QString &type);
    void onAttachClicked();

private:
    int mItemId;
    CategoryType mCategoryType;
    QList<FieldDefinition> mFieldDefs;

    QFormLayout *mFormLayout{nullptr};
    QWidget     *mFormContainer{nullptr};

    QMap<QString, QLineEdit*>  mLineEdits;
    QMap<QString, QDateEdit*>  mDateEdits;
    QMap<QString, QComboBox*>  mComboBoxes;
    QMap<QString, QTextEdit*>  mTextEdits;

    // Education 전용
    QComboBox  *mEduTypeCombo{nullptr};
    QLabel     *mAttachLabel{nullptr};
    QString     mAttachmentPath;
    int         mDynamicRowStart{0};
    QStringList mDynamicKeys;

    // 부/복수전공 조건부 위젯
    QLabel    *mMinorMajorLabel{nullptr};
    QLineEdit *mMinorMajorEdit{nullptr};

    QPushButton *mSaveButton{nullptr};
    QPushButton *mCancelButton{nullptr};
};

#endif // RESUMEITEMDIALOG_H
