#include "Dossier.h"

NewDossier::NewDossier(QWidget* parent)
{
    texte = new QLabel("Bienvenue dans la creation d'un nouvau dossier", this);

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
    setLayout(couche);
}

EditDossier::EditDossier(Dossier* dToEdit, QWidget *parent): d(dToEdit), QWidget(parent) {
    texte = new QLabel("Edition du dossier", this);

    nomLabel = new QLabel("Nom: ", this);
    prenomLabel = new QLabel("Prénom: ", this);
    cursusLabel = new QLabel("Cursus: ", this);

    nom = new QLineEdit(d->getNom(), this);
    prenom = new QLineEdit(d->getPrenom(), this);
    cursus = new QLineEdit(d->getCursus().getNom(), this);

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
    setLayout(couche);
}
