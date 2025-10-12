#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "WorkspaceSidebar.h"

class WorkspaceWidget : public QWidget {
    Q_OBJECT

public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    ~WorkspaceWidget() = default;

private:
    void setupUi();
    void loadStyles();
    void connectSignals();

private slots:
    void onMenuChanged(int menuIndex);

private:
    QHBoxLayout *mMainLayout;
    WorkspaceSidebar *mSidebar;
    QStackedWidget *mStackedWidget;
};


#endif
