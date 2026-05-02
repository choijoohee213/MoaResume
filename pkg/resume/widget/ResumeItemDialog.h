#ifndef RESUMEITEMDIALOG_H
#define RESUMEITEMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFormLayout>
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
    void connectSignals();
    void loadData(const ResumeItem &item);
    bool validateInput();
    QString getFieldValue(const FieldDefinition &field) const;

private slots:
    void onSaveClicked();

private:
    int mItemId;
    CategoryType mCategoryType;
    QList<FieldDefinition> mFieldDefs;

    QFormLayout *mFormLayout;

    QMap<QString, QLineEdit*>  mLineEdits;
    QMap<QString, QDateEdit*>  mDateEdits;
    QMap<QString, QComboBox*>  mComboBoxes;
    QMap<QString, QTextEdit*>  mTextEdits;

    QPushButton *mSaveButton;
    QPushButton *mCancelButton;
};

#endif // RESUMEITEMDIALOG_H
