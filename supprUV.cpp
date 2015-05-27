#include "supprUV.h"

SupprUV::SupprUV(QWidget* parent): QWidget(parent)
{
    this->setWindowTitle("Suppression d'une UV");

    texte = new QLabel("Quelle UV souhaitez-vous supprimer ? ", this);
    chUV = new QLineEdit(this);
    bouton = new QPushButton("OK", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH1->addWidget(chUV);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(bouton);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    setLayout(couche);

    QObject::connect(bouton, SIGNAL(clicked()), this, SLOT(suppr()));
}

//Slot pour la suppression d'une UV
void SupprUV::suppr()
{
    try {
        //Utilisation d'une méthode de la classe UVManager pour agir sur le catalogue des UVs
        UVManager::getInstance().supprimerUV(chUV->text());
         QMessageBox::information(this, "Suppression d'une UV", "UV supprimée ...");
    }
    catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Suppression d'une UV", e.getInfo());
    }
}
