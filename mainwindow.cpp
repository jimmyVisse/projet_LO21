#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //Chargement du catalogue des UVs et des cursus au lancement de l'application
    try {
        UVManager::getInstance().load("UV_XML.xml");
        CursusManager::getInstance().load("UV_Cursus.xml");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Chargement des catalogues", "Erreur"+e.getInfo());
    }

    //Chargement et affichage des éléments créés par QtDesigner
    ui->setupUi(this);

    //Connexion entre les différents widgets de l'interface et les slots de la classe
    connect(ui->actionFermer_dossier, SIGNAL(triggered()), this, SLOT(fermerDossier()));
    connect(ui->valider, SIGNAL(clicked()), this, SLOT(choixValider()));
    connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAjouter_UV, SIGNAL(triggered()), this, SLOT(openAjoutUV()));
    connect(ui->actionSupprimer_UV, SIGNAL(triggered()), this, SLOT(openSupprUV()));
    connect(ui->actionEditer_UV, SIGNAL(triggered()), this, SLOT(openUV()));
    connect(ui->actionAjouter_Cursus, SIGNAL(triggered()), this, SLOT(addCursus()));
    connect(ui->actionSupprimer_Cursus, SIGNAL(triggered()), this, SLOT(supprCursus()));
    connect(ui->actionEditer_Cursus, SIGNAL(triggered()), this, SLOT(editCursus()));
    connect(ui->actionDossier, SIGNAL(triggered()), this, SLOT(chargerDossier()));
    connect(ui->actionEditer_Dossier, SIGNAL(triggered()), this, SLOT(editDossier()));
    connect(ui->actionAjouter_Inscription, SIGNAL(triggered()), this, SLOT(addInsc()));
    connect(ui->actionCompl_ter_dossier, SIGNAL(triggered()), this, SLOT(completerDossier()));
}

//Slot gérant quel bouton radio est coché dans la page d'accueil pour l'affichage de la fenêtre associée
void MainWindow::choixValider() {
    if(ui->ajoutUV->isChecked()) {
        try {
            UVAjout* fenetre=new UVAjout(this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e){
            QMessageBox::warning(this, "Ajout", "Erreur");
        }
    }
    else if(ui->supprUV->isChecked()) {
        try {
            SupprUV* fenetre = new SupprUV(this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Suppression", "Erreur"+e.getInfo());
        }
    }
    else if(ui->editUV->isChecked()) {
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
    else if(ui->ajoutCursus->isChecked()) {
        try {
            CAjout *fenetre = new CAjout(this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Ajouter cursus", e.getInfo());
        }
    }
    else if(ui->supprCursus->isChecked()) {
        try {
            SupprimerFormation* fenetre = new SupprimerFormation(this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Formation", "Erreur"+e.getInfo());
        }
    }
    else if(ui->editCursus->isChecked()) {
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
    else if(ui->chargerDossier->isChecked()) {
        QString chemin = QFileDialog::getOpenFileName();
        try {
            if(chemin != "") {
                Dossier::getInstance().chargerDossier(chemin);
                QMessageBox::information(this, "Chargement dossier", "Le dossier a ete charge");
            }
        }catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Chargement catalogue cursus", e.getInfo());
        }
    }
    else if(ui->creerDossier->isChecked()) {
        try {
            NewDossier* fenetre = new NewDossier(this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Dossier", "Erreur"+e.getInfo());
        }
    }
    else {
        QMessageBox::warning(this, "Erreur","Aucune case n'est cochée");
    }
}

//Affichage de la fenetre de gestion d'UVs
void MainWindow::openUV() {
    //On demande le code de l'UV à modifier
    QString code=QInputDialog::getText(this,"Entrez le code de l’UV à éditer","UV");
    if (code!="") {
        try {
            //On récupére l'UV en question si elle existe et on lance l'éditeur d'UV
            UV& uv = UVManager::getInstance().getUV(code);
            UVEditeur* fenetre=new UVEditeur(uv,this);
            setCentralWidget(fenetre);
        }catch(UTProfilerException& e){
            //Erreur si l'UV n'existe pas
            QMessageBox::warning(this, "Edition UV", QString("Erreur : l’UV ")+code+" n’existe pas.");
        }
    }
}

//Affichage de la fenêtre d'ajout d'une UV
void MainWindow::openAjoutUV() {
    try {
        UVAjout* fenetre=new UVAjout(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "Ajout", "Erreur");
    }
}

//Affichage de la fenêtre de suppression d'une UV
void MainWindow::openSupprUV()
{
    try {
        SupprUV* fenetre = new SupprUV(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression", "Erreur"+e.getInfo());
    }
}

//Nouvelle fenêtre d'ajout d'un dossier
void MainWindow::newDossier()
{
    try {
        NewDossier* fenetre = new NewDossier(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Dossier", "Erreur"+e.getInfo());
    }
}

//Fenêtre de suppression d'un nouveau cursus
void MainWindow::supprCursus()
{
    try {
        SupprimerFormation* fenetre = new SupprimerFormation(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Formation", "Erreur"+e.getInfo());
    }
}

//Affichage d'une fenêtre d'édition d'un cursus
void MainWindow::editCursus() {
    //Demande d'un nom de cursus
    QString nom = QInputDialog::getText(this,"Entrez le nom de la formation à éditer","Formation");
    if (nom!="") {
        try {
            //si ce cursus existe, on affiche la fenpetre chargée avec ce cursus
            Formation& f = CursusManager::getInstance().getFormation(nom);
            CEditeur* fenetre = new CEditeur(&f, this);
            setCentralWidget(fenetre);
        }
        catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Editeur de cursus", e.getInfo());
        }
    }
}

//Chargement d'un dossier
void MainWindow::chargerDossier()
{
    //On ouvre une fenêtre pour que l'utilisateur sélectionne un dossier, et on récupére le chemin absolu de ce fichier
    QString chemin = QFileDialog::getOpenFileName();
    try {
        if(chemin != "") {
            Dossier::getInstance().chargerDossier(chemin);
            QMessageBox::information(this, "Chargement dossier", "Le dossier a ete charge");
        }
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Chargement catalogue cursus", e.getInfo());
    }
}

//Nouvelle fenêtre d'édition du dossier
void MainWindow::editDossier()
{
    try {
        //on récupére le dossier courant et on le transmet à la fenêtre d'édition
        Dossier& d = Dossier::getInstance();
        EditDossier* fenetre = new EditDossier(&d, this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Chargement catalogue cursus", e.getInfo());
    }
}

//Nouvelle fenêtre d'ajout d'une formation
void MainWindow::addCursus()
{
    try {
        CAjout *fenetre = new CAjout(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Ajouter cursus", e.getInfo());
    }
}

//Slot permettant d'ajouter une inscription à un dossier
void MainWindow::addInsc()
{
    try {
        AjouterIns *fen = new AjouterIns(this);
        setCentralWidget(fen);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Inscription dossier", e.getInfo());
    }
}

//Slot appelant la complétion du dossier
void MainWindow::completerDossier() {
    try {
        Completer* fenetre = new Completer(this);
        setCentralWidget(fenetre);
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Completer dossier", e.getInfo());
    }
}

//Slot permettant de fermer le dossier courant
void MainWindow::fermerDossier()
{
    try {
        Dossier::getInstance().libererInstance();
        QMessageBox::information(this, "Fermeture dossier", "Dossier fermé");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Fermer le dossier", e.getInfo());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
