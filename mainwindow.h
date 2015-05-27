/*
 * Fenêtre d'accueil de l'application, définie via QtDesigner
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "UVEditeur.h"
#include "DossierUI.h"
#include "supprFormation.h"
#include "CursusEditeur.h"
#include "UVEditeur.h"
#include "dossier.h"   //include dossier.h permet d'inclure tous les autres fichiers .h utiles
#include "supprUV.h"
#include "completer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void choixValider();
    void openUV();
    void openAjoutUV();
    void openSupprUV();
    void newDossier();
    void supprCursus();
    void editCursus();
    void editDossier();
    void chargerDossier();
    void addCursus();
    void addInsc();
    void completerDossier();
    void fermerDossier();
};

#endif // MAINWINDOW_H
