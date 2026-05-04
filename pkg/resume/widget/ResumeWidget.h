#ifndef RESUMEWIDGET_H
#define RESUMEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QFile>
#include "ResumeSidebar.h"
#include "ResumeItemListWidget.h"
#include "BasicInfoWidget.h"

class ResumeWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResumeWidget(QWidget *parent = nullptr);
    ~ResumeWidget() = default;

private:
    void setupUi();
    void connectSignals();
    void loadStyles();

private slots:
    void onCategorySelected(int categoryId);

private:
    ResumeSidebar        *mSidebar;
    QStackedWidget       *mStack;
    BasicInfoWidget      *mBasicInfoWidget;
    ResumeItemListWidget *mItemListWidget;
};

#endif //RESUMEWIDGET_H
