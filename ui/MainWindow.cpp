#include "MainWindow.h"
#include "../pkg/home/widget/HomeWidget.h"
#include "../common/Constants.h"
#include "UIConstants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
    connectSignals();
}

void MainWindow::setupUi() {
    setWindowTitle(Constants::APP_TITLE);
    setMinimumSize(UIConstants::MAIN_WINDOW_MIN_WIDTH_SIZE, UIConstants::MAIN_WINDOW_MIN_HEIGHT_SIZE);
    resize(UIConstants::MAIN_WINDOW_INITIAL_WIDTH_SIZE, UIConstants::MAIN_WINDOW_INITIAL_HEIGHT_SIZE);

    mStackedWidget = new QStackedWidget(this);
    setCentralWidget(mStackedWidget);

    mHomeWidget = new HomeWidget(this);
    mStackedWidget->addWidget(mHomeWidget);
}

void MainWindow::connectSignals() {
}
