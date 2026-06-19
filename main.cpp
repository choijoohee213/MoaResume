#include "ui/MainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/common/styles/global.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
