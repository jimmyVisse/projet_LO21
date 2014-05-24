#include "CursusEditeur.h"

CEditeur::CEditeur(Formation *f, QWidget* parent): f(f), QWidget(parent)
{
    setWindowTitle("Editeur de cursus");

    nomLabel = new QLabel("nom du cursus: ", this);
    nom = new QLineEdit(f->getNom(), this);
    uvLabel = new QLabel("UVs: ", this);

    listeUV = new QComboBox(this);
    UV* uvs = CursusManager::getInstance().getUVsCursus(f->getNom());
    listeUV->addItem(uvs->getCode());

    valider = new QPushButton("valider", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(nomLabel);
    coucheH1->addWidget(nom);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(valider);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(uvLabel);
    coucheH3->addWidget(listeUV);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH2);
    setLayout(couche);
}
