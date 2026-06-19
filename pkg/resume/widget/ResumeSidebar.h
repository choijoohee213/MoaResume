#ifndef RESUMESIDEBAR_H
#define RESUMESIDEBAR_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include "../service/ResumeService.h"

class ResumeSidebar : public QWidget {
    Q_OBJECT

public:
    explicit ResumeSidebar(QWidget *parent = nullptr);
    ~ResumeSidebar() = default;

    void selectFirst();

signals:
    void categorySelected(int categoryId);

private:
    void setupUi();
    void connectSignals();
    void loadCategories();

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    QListWidget *mListWidget;
    ResumeService &mService;
};

#endif // RESUMESIDEBAR_H
