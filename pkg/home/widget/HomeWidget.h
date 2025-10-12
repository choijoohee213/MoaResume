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

    void loadStyles();

    void setupTitleLabel();

    void setupStartBtn();

    QLabel *mTitleLabel;
    QPushButton *mStartButton;
    QVBoxLayout *mMainLayout;
};

#endif
