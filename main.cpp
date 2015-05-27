#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow profiler;

    profiler.show();
    return app.exec();
}
