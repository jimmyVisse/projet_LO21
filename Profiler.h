#ifndef PROFILER_H
#define PROFILER_H

#include <QApplication>
#include <QMainWindow>
#include <QInputDialog>
#include <QString>
#include <QMenuBar>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "UVEditeur.h"

class Profiler: public QMainWindow {
    Q_OBJECT
public:
    explicit Profiler(QWidget *parent=0);

signals:
public slots:
    void openChargerUV();
    void openUV();
    void openAjoutUV();
    void openSupprUV();
};

#endif // PROFILER_H
