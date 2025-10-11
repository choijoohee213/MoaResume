#include "MainWindow.h"
#include "../pkg/home/widget/HomeWidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
    connectSignals();
    qDebug() << "MainWindow 초기화 완료";
}

void MainWindow::setupUi() {
}

void MainWindow::connectSignals() {
}
