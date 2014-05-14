#include "Profiler.h"
#include "UVEditeur.h"
#include "UTProfiler.h"
#include "UVAjout.h"
#include "supprUV.h"

Profiler::Profiler(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("UT-Profiler");
    QMenu* mFichier = menuBar()->addMenu("&Fichier");
    QMenu* mCharger = menuBar()->addMenu("&Charger");
    QAction *actionChargerUV = mCharger->addAction("Catalogue UVs");
    mFichier->addSeparator();
    QAction *actionQuitter = mFichier->addAction("&Quitter");
    QMenu* mEdition = menuBar()->addMenu("&Edition");
    QAction *actionUV = mEdition->addAction("&Editer UV");
    QAction *ajoutUV = mEdition->addAction("&Ajouter UV");
    QAction *supprUV = mEdition->addAction("&Supprimer UV");

    connect(actionChargerUV, SIGNAL(triggered()),this,SLOT(openChargerUV()));
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionUV, SIGNAL(triggered()),this,SLOT(openUV()));
    connect(ajoutUV, SIGNAL(triggered()),this,SLOT(openAjoutUV()));
    connect(supprUV, SIGNAL(triggered()),this,SLOT(openSupprUV()));
}

void Profiler::openChargerUV() {
    QString chemin = QFileDialog::getOpenFileName();
    try {
        if(chemin != "") UVManager::getInstance().load(chemin);
        QMessageBox::information(this, "Chargement catalogue", "Le catalogue d'UVs a ete charge");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Chargement catalogue", "Erreur lors du chargement du fichier (non valide ?)");
    }
}

void Profiler::openUV() {
    QString code=QInputDialog::getText(this,"Entrez le code de l’UV à éditer","UV");
    if (code!="") {
        try {
            UV& uv = UVManager::getInstance().getUV(code);
            UVEditeur* fenetre=new UVEditeur(uv,this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e){
            QMessageBox::warning(this, "Edition UV", QString("Erreur : l’UV ")+code+" n’existe pas.");
        }
    }
}

void Profiler::openAjoutUV() {
    try {
        UVAjout* fenetre=new UVAjout(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "Ajout", "Erreur");
    }
}

void Profiler::openSupprUV()
{
    try {
        SupprUV* fenetre = new SupprUV(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression", "Erreur"+e.getInfo());
    }
}
