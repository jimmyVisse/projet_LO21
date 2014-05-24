#include "supprFormation.h"

SupprimerFormation::SupprimerFormation(QWidget* parent): QWidget(parent)
{
    texte = new QLabel("Sélectionner la formation à supprimer: ", this);

    valider = new QPushButton("Valider", this);

    choixCursus = new QLineEdit(this);

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

void SupprimerFormation::supprimer()
{
    try {
        CursusManager::getInstance().supprimerCurusus(choixCursus->text());
        QMessageBox::information(this, "Suppression Formation", "Cursus supprimé ...");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression Formation", e.getInfo());
    }
}
