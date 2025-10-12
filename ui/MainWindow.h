#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../pkg/home/widget/HomeWidget.h"
#include "../pkg/workspace/widget/WorkspaceWidget.h"
#include <QMainWindow>
#include <QStackedWidget>

class HomeWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() = default;

private slots:
    void showHome();

    void showWorkspace();

private:
    void setupUi();

    void connectSignals();

    QStackedWidget *mStackedWidget;

    HomeWidget *mHomeWidget;

    WorkspaceWidget *mWorkspaceWidget;
};

#endif
