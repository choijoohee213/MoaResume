#ifndef RESUMEWIDGET_H
#define RESUMEWIDGET_H

#include <QWidget>

class ResumeWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResumeWidget(QWidget *parent = nullptr);

    ~ResumeWidget() = default;

private:
    void setupUi();
};


#endif //RESUMEWIDGET_H