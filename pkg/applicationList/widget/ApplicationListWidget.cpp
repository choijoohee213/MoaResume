#include "ApplicationListWidget.h"

ApplicationListWidget::ApplicationListWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
    setupTable();
    loadSampleData();
}

void ApplicationListWidget::setupUi() {
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(20, 20, 20, 20);
    mMainLayout->setSpacing(0);

    mTableWidget = new QTableWidget(this);
    mMainLayout->addWidget(mTableWidget);

    setLayout(mMainLayout);
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
}

void ApplicationListWidget::loadSampleData() {
    mTableWidget->setRowCount(3);

    mTableWidget->setItem(0, 0, new QTableWidgetItem("네이버"));
    mTableWidget->setItem(0, 1, new QTableWidgetItem("백엔드 개발자"));
    mTableWidget->setItem(0, 2, new QTableWidgetItem("서류 통과"));
    mTableWidget->setItem(0, 3, new QTableWidgetItem("2025-01-15"));

    mTableWidget->setItem(1, 0, new QTableWidgetItem("카카오"));
    mTableWidget->setItem(1, 1, new QTableWidgetItem("프론트엔드 개발자"));
    mTableWidget->setItem(1, 2, new QTableWidgetItem("지원 완료"));
    mTableWidget->setItem(1, 3, new QTableWidgetItem("2025-01-20"));

    mTableWidget->setItem(2, 0, new QTableWidgetItem("라인"));
    mTableWidget->setItem(2, 1, new QTableWidgetItem("풀스택 개발자"));
    mTableWidget->setItem(2, 2, new QTableWidgetItem("면접 예정"));
    mTableWidget->setItem(2, 3, new QTableWidgetItem("2025-01-10"));
}
