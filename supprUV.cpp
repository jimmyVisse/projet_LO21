#include "supprUV.h"

SupprUV::SupprUV(QWidget* parent): QWidget(parent)
{
    this->setWindowTitle("Suppression d'une UV");

    texte = new QLabel("Quelle est la categorie de l'UV à supprimer: ", this);

    choixCat = new QComboBox(this);
    choixCat->addItem("CS");
    choixCat->addItem("TM");
    choixCat->addItem("TSH");
    choixCat->addItem("SP");

    bouton = new QPushButton("OK", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH1->addWidget(choixCat);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(bouton);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    setLayout(couche);

    QObject::connect(bouton, SIGNAL(clicked()), this, SLOT(openChoix()));
}

void SupprUV::openChoix()
{
    FenetreChoix fenetre(choixCat->currentIndex());
    fenetre.exec();
}

FenetreChoix::FenetreChoix(int nb, QDialog *parent): QDialog(parent), placeUV(nb)
{
    this->setWindowTitle("Suppression d'une UV-Choix de l'UV");

    texte = new QLabel("UV a supprimer: ", this);

    choix = new QComboBox(this);
    for(UVManager::FilterIterator it = UVManager::getInstance().getFilterIterator(Categorie(placeUV)); !it.isDone(); it.next())
    {
        choix->addItem(it.current().getCode());
    }

    ok = new QPushButton("Choisir", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH1->addWidget(choix);
    couche = new QVBoxLayout;
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(ok);
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    setLayout(couche);

    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(supprimer()));
}

void FenetreChoix::supprimer()
{
    try {
        unsigned int i=0;
        UVManager::FilterIterator it  = UVManager::getInstance().getFilterIterator(Categorie(placeUV));
        while(i!=choix->currentIndex()) {
            it.next();
            i++;
        }
        UVManager::getInstance().supprimerUV(it.current().getCode());
        QMessageBox::information(this, "Suppression d'une UV-Choix de l'UV", "UV supprimée ...");
    }
    catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression d'une UV", e.getInfo());
    }
}
