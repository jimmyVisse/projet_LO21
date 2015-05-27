#include "supprFormation.h"

SupprimerFormation::SupprimerFormation(QWidget* parent): QWidget(parent)
{
    /* Création des widgets de la classe */

    texte = new QLabel("Sélectionner la formation à supprimer: ", this);
    valider = new QPushButton("Valider", this);
    choixCursus = new QLineEdit(this);

    /* Mise en forme */

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH1->addWidget(choixCursus);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(valider);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    setLayout(couche);

    connect(valider, SIGNAL(clicked()), this, SLOT(supprimer()));
}

//Suppression du cursus
void SupprimerFormation::supprimer()
{
    try {
        //utilisation d'une méthode de la classe CursusManager pour agir sur le catalogue des cursus
        CursusManager::getInstance().supprimerCursus(choixCursus->text());
        QMessageBox::information(this, "Suppression Formation", "Cursus supprimé ...");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression Formation", e.getInfo());
    }
}
