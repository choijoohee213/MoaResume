#ifndef WORKSPACESIDEBAR_H
#define WORKSPACESIDEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>

class WorkspaceSidebar : public QWidget {
    Q_OBJECT

public:
    explicit WorkspaceSidebar(QWidget *parent = nullptr);
    ~WorkspaceSidebar() = default;

signals:
    void menuChanged(int menuIndex);

private:
    void setupUi();
    void setupMenuButtons();
    void connectSignals();

private slots:
    void onMenuButtonClicked(int id);

private:
    QVBoxLayout *mMainLayout;
    QButtonGroup *mMenuButtonGroup;

    QPushButton *mApplicationBtn;
    QPushButton *mResumeBtn;
};

#endif
