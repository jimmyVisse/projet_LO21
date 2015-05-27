/*
 * Définition de l'interface graphique de complétion
*/

#include "completer.h"

Completer::Completer(QWidget *parent): QWidget(parent) {
    setWindowTitle("Completer un dossier");

    texte = new QLabel("Choisissez une UV et son degré de préférence (vous devez faire cela pour tous vos rejets et vos préférences)");
    uv_label = new QLabel("UV: ", this);
    pref_label = new QLabel("Préférence: ", this);
    uv = new QLineEdit(this);
    preference = new QSpinBox(this);
    preference->setRange(-1, 5);
    preference->setValue(0);
    ajouter = new QPushButton("Ajouter cette préférence", this);
    valider =  new QPushButton("valider le choix final", this);

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(texte);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(uv_label);
    coucheH2->addWidget(uv);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(pref_label);
    coucheH3->addWidget(preference);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(ajouter);
    coucheH4->addWidget(valider);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);
    setLayout(couche);

    //Connection des boutons avec les slots de la classe
    connect(ajouter, SIGNAL(clicked()), this, SLOT(ajouterPreference()));
    connect(valider, SIGNAL(clicked()), this, SLOT(validerChoix()));
}

void Completer::ajouterPreference() {
    //Récupération de l'UV à modifier la préférence
    UV& uvCible = UVManager::getInstance().getUV(uv->text());
    //Modification de la préférence
    uvCible.setPreference(preference->value());
    QMessageBox::information(this, "Ajout preference", "Préférence ajoutée");
}

void Completer::validerChoix() {
    try {
        //Appel à la méthode de complétion
        Dossier::getInstance().completerCursus();
        QMessageBox::information(this, "Compléter dossier", "Dossier complété");
    }catch(UTProfilerException& e) {
        QMessageBox::warning(this, "Compléter dossier", "Erreur"+e.getInfo());
    }
}
