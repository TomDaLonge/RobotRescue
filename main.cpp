#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow test;
    test.resize(600, 600);
    test.show();
    
    return app.exec();
}
