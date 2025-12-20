#ifndef APPLICATIONLISTWIDGET_H
#define APPLICATIONLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>

class ApplicationListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ApplicationListWidget(QWidget *parent = nullptr);

    ~ApplicationListWidget() = default;

private:
    void setupUi();

    void setupTable();

    void loadSampleData();

private:
    QVBoxLayout *mMainLayout;
    QTableWidget *mTableWidget;
};


#endif //APPLICATIONLISTWIDGET_H
