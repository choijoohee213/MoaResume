#include "ResumeSidebar.h"

ResumeSidebar::ResumeSidebar(QWidget *parent)
    : QWidget(parent), mService(ResumeService::getInstance()) {
    setupUi();
    connectSignals();
    loadCategories();
}

void ResumeSidebar::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    mListWidget = new QListWidget(this);
    mListWidget->setFixedWidth(160);

    layout->addWidget(mListWidget);
    setLayout(layout);
}

void ResumeSidebar::connectSignals() {
    connect(mListWidget, &QListWidget::itemClicked, this, &ResumeSidebar::onItemClicked);
}

void ResumeSidebar::loadCategories() {
    mListWidget->clear();
    for (const ResumeCategory &category : mService.getCategories()) {
        QListWidgetItem *item = new QListWidgetItem(category.getName());
        item->setData(Qt::UserRole, category.getId());
        mListWidget->addItem(item);
    }

    if (mListWidget->count() > 0) {
        mListWidget->setCurrentRow(0);
        QListWidgetItem *first = mListWidget->item(0);
        emit categorySelected(first->data(Qt::UserRole).toInt());
    }
}

void ResumeSidebar::onItemClicked(QListWidgetItem *item) {
    emit categorySelected(item->data(Qt::UserRole).toInt());
}
