#ifndef APPLICATIONWIDGET_H
#define APPLICATIONWIDGET_H

#include <QWidget>

class ApplicationWidget : public QWidget {
    Q_OBJECT

public:
    explicit ApplicationWidget(QWidget *parent = nullptr);

    ~ApplicationWidget() = default;

private:
    void setupUi();
};


#endif //APPLICATIONWIDGET_H
