#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class HomeWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void showHome();

private:
    void setupUi();
    void connectSignals();

    QStackedWidget* stackedWidget;

    HomeWidget* homeWidget;
};

#endif