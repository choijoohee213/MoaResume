#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>

class WorkspaceWidget : public QWidget {
    Q_OBJECT

public:
    explicit WorkspaceWidget(QWidget *parent = nullptr);

    ~WorkspaceWidget() = default;

private:
    void setupUi();
    void connectSignals();

    QSt
};


#endif
