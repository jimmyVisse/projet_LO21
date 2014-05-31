#include "CursusEditeur.h"

CEditeur::CEditeur(Formation *f, QWidget* parent): f(f), QWidget(parent)
{
    setWindowTitle("Editeur de cursus");

    nomLabel = new QLabel("nom du cursus: ", this);
    nom = new QLineEdit(f->getNom(), this);
    uvLabel = new QLabel("UVs: ", this);

    listeUV = new QComboBox(this);
    for(Formation::Iterateur it = CursusManager::getInstance().getFormation(f->getNom()).begin(); !it.isDone(); it.next()) {
        listeUV->addItem(it.currentItem().getCode());
    }

    ajouter = new QPushButton("Ajouter une UV", this);
    supprimer = new QPushButton("Supprimer l'UV", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(nomLabel);
    coucheH1->addWidget(nom);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(ajouter);
    coucheH2->addWidget(supprimer);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(uvLabel);
    coucheH3->addWidget(listeUV);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH2);
    setLayout(couche);

    connect(ajouter, SIGNAL(clicked()), this, SLOT(ajoutUV()));
    connect(supprimer, SIGNAL(clicked()), this, SLOT(supprUV()));
}

void CEditeur::ajoutUV()
{
    QString code=QInputDialog::getText(this,"Entrez le code de l’UV à éditer","UV");
    if(code!="") {
        try {
            CursusManager::getInstance().ajouterUVCursus(nom->text(), code);
            QMessageBox::information(this, "Ajouter une UV à un cursus", "UV insérée à la formation");
        }
        catch(UTProfilerException& e) {
            QMessageBox::warning(this, "Ajouter une UV à un cursus", QString("Erreur: ")+e.getInfo());
        }
    }
}

//Slot pour supprimer l'UV sélectionnée dans la ComboBox
void CEditeur::supprUV()
{
    try {
        unsigned int i=0;
        //On retrouve l'UV sélectionnée par l'itérateur
        Formation::Iterateur it = CursusManager::getInstance().getFormation(f->getNom()).begin();
        while(i<listeUV->currentIndex()) { i++; it.next(); }
        CursusManager::getInstance().supprimerUVCursus(nom->text(), it.currentItem().getCode());
        QMessageBox::information(this, "Suppression d'une UV à un cursus", "UV supprimée du cursus");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression d'une UV à un cursus", "Erreur"+e.getInfo());
    }
}
