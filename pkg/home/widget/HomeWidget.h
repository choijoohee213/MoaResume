#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class QVBoxLayout;

class HomeWidget : public QWidget {
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);

    ~HomeWidget() = default;

signals:
    void navigateToWorkspace();

private slots:
    void onWorkSpaceClicked();

private:
    void setupUi();

    void connectSignals();

    QLabel *titleLabel;
    QPushButton *startButton;
    QVBoxLayout *mainLayout;
};

#endif
