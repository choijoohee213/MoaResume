#include "ApplicationListWidget.h"
#include "ApplicationDetailDialog.h"
#include "../ApplicationListConstants.h"
#include <QMessageBox>
#include <QFile>

ApplicationListWidget::ApplicationListWidget(QWidget *parent)
    : QWidget(parent), mService(ApplicationListService::getInstance()) {
    setupUi();
    setupToolbar();
    setupTable();
    connectSignals();
    loadStyles();
    loadApplications();
}

void ApplicationListWidget::setupUi() {
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(20, 20, 20, 20);
    mMainLayout->setSpacing(10);

    mTableWidget = new QTableWidget(this);

    setLayout(mMainLayout);
}

void ApplicationListWidget::setupToolbar() {
    mToolbarLayout = new QHBoxLayout();
    mToolbarLayout->setSpacing(10);
    mToolbarLayout->setContentsMargins(0, 0, 0, 10);

    mAddButton = new QPushButton(ApplicationListConstants::BTN_TEXT_ADD, this);
    mAddButton->setObjectName(ApplicationListConstants::BTN_NAME_ADD);
    mAddButton->setCursor(Qt::PointingHandCursor);

    mDeleteButton = new QPushButton(ApplicationListConstants::BTN_TEXT_DELETE, this);
    mDeleteButton->setObjectName(ApplicationListConstants::BTN_NAME_DELETE);
    mDeleteButton->setCursor(Qt::PointingHandCursor);
    mDeleteButton->setEnabled(false);

    mToolbarLayout->addWidget(mAddButton);
    mToolbarLayout->addWidget(mDeleteButton);
    mToolbarLayout->addStretch();

    mMainLayout->insertLayout(0, mToolbarLayout);
}

void ApplicationListWidget::setupTable() {
    mTableWidget->setColumnCount(4);
    QStringList headers = {"기업 이름", "직무", "상태", "마감일"};
    mTableWidget->setHorizontalHeaderLabels(headers);

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTableWidget->setAlternatingRowColors(true);
    mTableWidget->verticalHeader()->setVisible(false);

    mTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    mTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    mTableWidget->setColumnWidth(2, 120);
    mTableWidget->setColumnWidth(3, 150);

    mMainLayout->addWidget(mTableWidget);
}

void ApplicationListWidget::connectSignals() {
    connect(mAddButton, &QPushButton::clicked, this, &ApplicationListWidget::onAddClicked);
    connect(mDeleteButton, &QPushButton::clicked, this, &ApplicationListWidget::onDeleteClicked);

    connect(mTableWidget, &QTableWidget::itemSelectionChanged,
            this, &ApplicationListWidget::onSelectionChanged);

    connect(mTableWidget, &QTableWidget::cellDoubleClicked,
            this, &ApplicationListWidget::onTableDoubleClicked);

    connect(&mService, &ApplicationListService::dataChanged,
            this, &ApplicationListWidget::onDataChanged);
}

void ApplicationListWidget::loadStyles() {
    QFile styleFile(ApplicationListConstants::QSS_PATH);
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void ApplicationListWidget::loadApplications() {
    QList<Application> applications = mService.getAllApplications();

    mTableWidget->setRowCount(applications.size());

    for (int i = 0; i < applications.size(); ++i) {
        const Application &app = applications[i];

        QTableWidgetItem *companyItem = new QTableWidgetItem(app.getCompanyName());
        companyItem->setData(Qt::UserRole, app.getId());

        mTableWidget->setItem(i, 0, companyItem);
        mTableWidget->setItem(i, 1, new QTableWidgetItem(app.getPosition()));
        mTableWidget->setItem(i, 2, new QTableWidgetItem(app.getStatus()));
        mTableWidget->setItem(i, 3, new QTableWidgetItem(app.getDeadline().toString(Qt::ISODate)));
    }
}

void ApplicationListWidget::updateButtonStates() {
    bool hasSelection = !mTableWidget->selectedItems().isEmpty();
    mDeleteButton->setEnabled(hasSelection);
}

int ApplicationListWidget::getSelectedApplicationId() const {
    QList<QTableWidgetItem*> selectedItems = mTableWidget->selectedItems();
    if (selectedItems.isEmpty()) return -1;

    int row = selectedItems.first()->row();
    QTableWidgetItem *firstItem = mTableWidget->item(row, 0);
    return firstItem->data(Qt::UserRole).toInt();
}

void ApplicationListWidget::onAddClicked() {
    Application newApp;
    ApplicationDetailDialog dialog(newApp, this);
    if (dialog.exec() == QDialog::Accepted) {
        Application addedApp = dialog.getApplication();
        if (!mService.addApplication(addedApp)) {
            QMessageBox::warning(this,
                ApplicationListConstants::MSG_TITLE_ERROR,
                ApplicationListConstants::MSG_ADD_FAILED);
        }
    }
}

void ApplicationListWidget::onDeleteClicked() {
    int selectedId = getSelectedApplicationId();
    if (selectedId < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        ApplicationListConstants::MSG_TITLE_CONFIRM,
        ApplicationListConstants::MSG_DELETE_CONFIRM,
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!mService.deleteApplication(selectedId)) {
            QMessageBox::warning(this,
                ApplicationListConstants::MSG_TITLE_ERROR,
                ApplicationListConstants::MSG_DELETE_FAILED);
        }
    }
}

void ApplicationListWidget::onSelectionChanged() {
    updateButtonStates();
}

void ApplicationListWidget::onDataChanged() {
    loadApplications();
}

void ApplicationListWidget::onTableDoubleClicked(int row, int column) {
    QTableWidgetItem *firstItem = mTableWidget->item(row, 0);
    if (!firstItem) return;

    int appId = firstItem->data(Qt::UserRole).toInt();

    Application app = mService.getApplicationById(appId);
    ApplicationDetailDialog dialog(app, this);

    if (dialog.exec() == QDialog::Accepted) {
        Application updatedApp = dialog.getApplication();
        if (!mService.updateApplication(updatedApp)) {
            QMessageBox::warning(this,
                ApplicationListConstants::MSG_TITLE_ERROR,
                ApplicationListConstants::MSG_UPDATE_FAILED);
        }
    }
}
