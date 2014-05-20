#include "supprFormation.h"

SupprimerFormation::SupprimerFormation(QWidget* parent): QWidget(parent)
{
    texte = new QLabel("Sélectionner la formation à supprimer: ", this);

    valider = new QPushButton("Valider", this);

    choixCursus = new QComboBox(this);
//    for(QMap<QString, Formation*>::iterator it = CursusManager::getInstance().getCursus().begin(); it!= CursusManager::getInstance().getCursus().end(); it++)
//    {
//        choixCursus->addItem((*it)->getNom());
//    }

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
        int i=0;
        QMap<QString, Formation*>::iterator it = CursusManager::getInstance().getCursus().begin();
        while(i<choixCursus->currentIndex()) {
            i++;
            it++;
        }
        CursusManager::getInstance().supprimerCurusus((*it)->getNom());
        QMessageBox::information(this, "Suppression Formation", "Cursus supprimé ...");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression Formation", e.getInfo());
    }
}
