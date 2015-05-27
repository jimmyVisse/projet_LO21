#include "DossierUI.h"

DossierEdit::DossierEdit(QWidget* parent): QWidget(parent) {
    texte = new QLabel(this);

    nomLabel = new QLabel("Nom: ", this);
    prenomLabel = new QLabel("Prénom: ", this);
    cursusLabel = new QLabel("Cursus: ", this);

    nom = new QLineEdit(this);
    prenom = new QLineEdit(this);
    cursus = new QLineEdit(this);

    bouton =  new QPushButton("Valider", this);

    /* Mise en forme générale */

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(nomLabel);
    coucheH2->addWidget(nom);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(prenomLabel);
    coucheH3->addWidget(prenom);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(cursusLabel);
    coucheH4->addWidget(cursus);
    coucheH5 = new QHBoxLayout;
    coucheH5->addWidget(bouton);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);
    couche->addLayout(coucheH5);
}

NewDossier::NewDossier(QWidget *parent): DossierEdit(parent)
{
    texte->setText("Bienvenue dans la creation d'un nouvau dossier");

    semestre = new QLabel("semestre: ");
    nbSem = new QSpinBox(this);
    nbSem->setRange(1, 9);

    coucheH4->addWidget(semestre);
    coucheH4->addWidget(nbSem);

    QMessageBox::information(this, "Nouveau dossier", "Indiquer le fichier xml hôte du nouveau dossier");
    chemin = QFileDialog::getOpenFileName();

    setLayout(couche);

    connect(bouton, SIGNAL(clicked()), this, SLOT(creationDossier()));
}

void NewDossier::creationDossier() {
    try {
        Dossier::getInstance().setNom(nom->text());
        Dossier::getInstance().setPrenom(prenom->text());
        Dossier::getInstance().setCursus(Formation(cursus->text()));
        Dossier::getInstance().setFile(chemin);
        Dossier::getInstance().setNbSemestre(nbSem->value());
        QMessageBox::information(this, "Creation du dossier", "Dossier créé");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Creation dossier", "Erreur"+e.getInfo());
    }
}

EditDossier::EditDossier(Dossier *dToEdit, QWidget *parent): DossierEdit(parent), d(dToEdit) {
    texte->setText("Edition du dossier");

    ajouter = new QPushButton("Ajouter inscription", this);

    cred = new QLabel("Nombre de crédits: ", this);
    credits = new QSpinBox(this);
    credits->setValue(d->getNbCredits());
    credits->setReadOnly(true);

    insc_label = new QLabel("Inscriptions: ", this);
    inscriptions = new QComboBox(this);
    //utilisation de l'itérateur deu vecteur d'inscription pour parcourir les inscriptions en dehors de la classe
    for(QVector<Inscription*>::iterator it = d->begin_ins(); it!=d->end_ins(); it++) {
        QString uv = (*it)->getUV().getCode();
        QString semestre = SemestreToString((*it)->getSemestre());
        QString note = NoteToString((*it)->getResultat());
        inscriptions->addItem(uv+" "+semestre+" "+note);
    }

    nom->setText(d->getNom());
    prenom->setText(d->getPrenom());
    cursus->setText(d->getCursus().getNom());

    coucheH4->addWidget(cred);
    coucheH4->addWidget(credits);
    coucheH4->addWidget(insc_label);
    coucheH4->addWidget(inscriptions);
    coucheH5->addWidget(ajouter);
    setLayout(couche);

    connect(bouton, SIGNAL(clicked()), this, SLOT(modifDossier()));
    connect(ajouter, SIGNAL(clicked()), this, SLOT(ajoutIns()));
}

void EditDossier::modifDossier()
{
    Dossier::getInstance().setNom(nom->text());
    Dossier::getInstance().setPrenom(prenom->text());
    Dossier::getInstance().setCursus(Formation(cursus->text()));
    QMessageBox::information(this, "Edition d'un dossier", "Changements effectués");
}

void EditDossier::ajoutIns()
{
    AjoutDossier* fen = new AjoutDossier(this);
    fen->exec();
}

EditDossier::~EditDossier()
{
    d=0;
}

//Constructeur de la fenêtre de dialogue d'ajout d'inscription
AjoutDossier::AjoutDossier(QWidget *parent): QDialog(parent)
{
    uvLabel = new QLabel("UV: ", this);
    noteLabel = new QLabel("Note: ", this);
    semLabel = new QLabel("Semestre: ", this);

    uv = new QLineEdit(this);
    note = new QLineEdit(this);
    semestre = new QLineEdit(this);

    valider = new QPushButton("Valider", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(uvLabel);
    coucheH1->addWidget(uv);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(noteLabel);
    coucheH2->addWidget(note);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(semLabel);
    coucheH3->addWidget(semestre);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(valider);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);
    setLayout(couche);

    connect(valider, SIGNAL(clicked()), this, SLOT(ajouter()));
}

//Slot de validation des informations rentrées dans la fenêtre de dialogue
void AjoutDossier::ajouter()
{
    try {
        //Utilisation d'une méthode de la classe Dossier
        Dossier::getInstance().ajoutInscription(uv->text(), note->text(), semestre->text());
        QMessageBox::information(this, "Ajouter une inscription", "Inscription ajoutée");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Ajouter une inscription", "Erreur dans l'ajout de l'inscription: "+e.getInfo());
    }
}

//Constructeur de la fenêtre classique d'ajout d'une inscription
AjouterIns::AjouterIns(QWidget *parent): QWidget(parent) {
    uvLabel = new QLabel("UV: ", this);
    noteLabel = new QLabel("Note: ", this);
    semLabel = new QLabel("Semestre: ", this);

    uv = new QLineEdit(this);
    note = new QLineEdit(this);
    semestre = new QLineEdit(this);

    valider = new QPushButton("Valider", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(uvLabel);
    coucheH1->addWidget(uv);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(noteLabel);
    coucheH2->addWidget(note);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(semLabel);
    coucheH3->addWidget(semestre);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(valider);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);
    setLayout(couche);

    connect(valider, SIGNAL(clicked()), this, SLOT(ajouter()));
}

void AjouterIns::ajouter()
{
    try {
        Dossier::getInstance().ajoutInscription(uv->text(), note->text(), semestre->text());
        QMessageBox::information(this, "Ajouter une inscription", "Inscription ajoutée");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Ajouter une inscription", "Erreur dans l'ajout de l'inscription: "+e.getInfo());
    }
}
