#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include "UTProfiler.h"
#include "UVEditeur.h"
#include "Profiler.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Profiler profiler;

    profiler.show();
    return app.exec();
}
