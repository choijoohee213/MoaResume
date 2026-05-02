#ifndef RESUMEWIDGET_H
#define RESUMEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFile>
#include "ResumeSidebar.h"
#include "ResumeItemListWidget.h"

class ResumeWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResumeWidget(QWidget *parent = nullptr);
    ~ResumeWidget() = default;

private:
    void setupUi();
    void connectSignals();
    void loadStyles();

private:
    ResumeSidebar *mSidebar;
    ResumeItemListWidget *mItemListWidget;
};

#endif //RESUMEWIDGET_H
