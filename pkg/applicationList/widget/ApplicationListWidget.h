#ifndef APPLICATIONLISTWIDGET_H
#define APPLICATIONLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include "../service/ApplicationListService.h"

class ApplicationListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ApplicationListWidget(QWidget *parent = nullptr);

    ~ApplicationListWidget() = default;

private:
    void setupUi();

    void setupToolbar();

    void setupTable();

    void connectSignals();

    void loadStyles();

    void loadApplications();

    void updateButtonStates();

    int getSelectedApplicationId() const;

private slots:
    void onAddClicked();

    void onDeleteClicked();

    void onSelectionChanged();

    void onDataChanged();

    void onTableDoubleClicked(int row, int column);

private:
    QVBoxLayout *mMainLayout;
    QHBoxLayout *mToolbarLayout;
    QTableWidget *mTableWidget;

    QPushButton *mAddButton;
    QPushButton *mDeleteButton;

    ApplicationListService &mService;
};


#endif //APPLICATIONLISTWIDGET_H
