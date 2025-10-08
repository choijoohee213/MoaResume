#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    int unused; // test for reviewdog
    QApplication a(argc, argv);
    QPushButton button("Hello MoaResume!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
