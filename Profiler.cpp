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

    QMenu* mDossier = menuBar()->addMenu("&Dossier");
    QAction *chargerDossier = mDossier->addAction("&Charger Dossier");
    QAction *nouveauDossier = mDossier->addAction("&Nouveau Dossier");

    QMenu* mFormation = menuBar()->addMenu("&Formation");
    QAction *supprCursus = mFormation->addAction("&Supprimer cursus");
    QAction *chargerCursus = mFormation->addAction("&Charger cursus");
    QAction *editerCursus = mFormation->addAction("&Editer cursus");

    connect(actionChargerUV, SIGNAL(triggered()),this,SLOT(openChargerUV()));
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionUV, SIGNAL(triggered()),this,SLOT(openUV()));
    connect(ajoutUV, SIGNAL(triggered()),this,SLOT(openAjoutUV()));
    connect(supprUV, SIGNAL(triggered()),this,SLOT(openSupprUV()));
//    connect(chargerDossier, SIGNAL(triggered()),this,SLOT());
    connect(nouveauDossier, SIGNAL(triggered()),this,SLOT(newDossier()));
    connect(supprCursus, SIGNAL(triggered()),this,SLOT(supprCursus()));
    connect(chargerCursus, SIGNAL(triggered()), this, SLOT(chargerCursus()));
    connect(editerCursus, SIGNAL(triggered()), this, SLOT(editCursus()));
}

void Profiler::openChargerUV() {
//    QString chemin = QFileDialog::getOpenFileName();
    try {
//        if(chemin != "") UVManager::getInstance().load(chemin);
        UVManager::getInstance().load("C:/Users/Jimmy/Documents/utc/GI/LO21/projet_lo21/UV_XML.xml");
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

void Profiler::newDossier()
{
    try {
        NewDossier* fenetre = new NewDossier(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Dossier", "Erreur"+e.getInfo());
    }
}

void Profiler::supprCursus()
{
    try {
        SupprimerFormation* fenetre = new SupprimerFormation(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Formation", "Erreur"+e.getInfo());
    }
}

void Profiler::chargerCursus()
{
//    QString chemin = QFileDialog::getOpenFileName();
    try {
//        if(chemin != "") CursusManager::getInstance().chargerFormation(chemin);
        CursusManager::getInstance().load("C:/Users/Jimmy/Documents/utc/GI/LO21/projet_lo21/UV_Cursus2.xml");
        QMessageBox::information(this, "Chargement catalogue cursus", "Le catalogue des cursus a ete charge");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Chargement catalogue cursus", e.getInfo());
    }
}

void Profiler::editCursus() {
    QString nom = QInputDialog::getText(this,"Entrez le nom de la formation à éditer","Formation");
    if (nom!="") {
        try {
            Formation& f = CursusManager::getInstance().getFormation(nom);
            CEditeur* fenetre = new CEditeur(&f, this);
            setCentralWidget(fenetre);
        }
        catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Editeur de cursus", e.getInfo());
        }
    }
}
