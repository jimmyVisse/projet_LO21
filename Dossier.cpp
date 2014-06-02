#include "Dossier.h"

DossierEdit::DossierEdit(QWidget* parent): QWidget(parent) {
    texte = new QLabel(this);

    nomLabel = new QLabel("Nom: ", this);
    prenomLabel = new QLabel("Prénom: ", this);
    cursusLabel = new QLabel("Cursus: ", this);

    nom = new QLineEdit(this);
    prenom = new QLineEdit(this);
    cursus = new QLineEdit(this);

    bouton =  new QPushButton("Valider", this);

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
    setLayout(couche);
}

EditDossier::EditDossier(Dossier& dToEdit, QWidget *parent): d(dToEdit), DossierEdit(parent) {
    texte->setText("Edition du dossier");

    ajouter = new QPushButton("Ajouter inscription", this);

    cred = new QLabel("Nombre de crédits: ", this);
    credits = new QSpinBox(this);
    credits->setValue(d.getNbCredits());

    nom->setText(d.getNom());
    prenom->setText(d.getPrenom());
    cursus->setText(d.getCursus().getNom());
    cursus->setReadOnly(true);

    coucheH4->addWidget(cred);
    coucheH4->addWidget(credits);
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

void AjoutDossier::ajouter()
{
    try {
        Dossier::getInstance().ajoutInscription(uv->text(), note->text(), semestre->text());
        QMessageBox::information(this, "Ajouter une inscription", "Inscription ajoutée");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Ajouter une inscription", "Erreur dans l'ajout de l'inscription: "+e.getInfo());
    }
}
