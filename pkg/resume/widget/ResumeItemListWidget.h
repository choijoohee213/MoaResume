#ifndef RESUMEITEMLISTWIDGET_H
#define RESUMEITEMLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "../service/ResumeService.h"
#include "../ResumeConstants.h"

class ResumeItemListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResumeItemListWidget(QWidget *parent = nullptr);
    ~ResumeItemListWidget() = default;

    void setCategory(int categoryId);

private:
    void setupUi();
    void connectSignals();
    void loadItems();
    void updateButtonStates();
    int getSelectedItemId() const;

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onCopyClicked();
    void onSelectionChanged();
    void onDataChanged(int categoryId);
    void onItemDoubleClicked(QListWidgetItem *listItem);

private:
    int mCurrentCategoryId;
    CategoryType mCurrentCategoryType;

    QLabel *mCategoryLabel;
    QListWidget *mListWidget;
    QPushButton *mAddButton;
    QPushButton *mDeleteButton;
    QPushButton *mCopyButton;

    ResumeService &mService;
};

#endif // RESUMEITEMLISTWIDGET_H
