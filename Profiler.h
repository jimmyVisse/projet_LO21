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
#include "Dossier.h"
#include "supprFormation.h"
#include "CursusEditeur.h"

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
    void newDossier();
    void supprCursus();
    void chargerCursus();
    void editCursus();
    void editDossier();
    void chargerDossier();
    void addCursus();
};

#endif // PROFILER_H
