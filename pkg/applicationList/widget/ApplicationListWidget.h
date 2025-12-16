#ifndef APPLICATIONLISTWIDGET_H
#define APPLICATIONLISTWIDGET_H

#include <QWidget>

class ApplicationListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ApplicationListWidget(QWidget *parent = nullptr);

    ~ApplicationListWidget() = default;

private:
    void setupUi();
};


#endif //APPLICATIONLISTWIDGET_H
