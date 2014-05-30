#include "UVEditeur.h"

Editeur::Editeur(QWidget *parent): QWidget(parent) {
    this->setWindowTitle("Ajout d'une UV");

    codeLabel = new QLabel("Code: ", this);
    titreLabel = new QLabel("Titre: ", this);
    ouverture = new QLabel("ouverture: ", this);
    catLabel = new QLabel("categorie: ", this);
    creditLabel = new QLabel("Credits: ", this);

    code = new QLineEdit(this);
    titre = new QTextEdit(this);

    automne = new QCheckBox("automne", this);
    printemps = new QCheckBox("printemps", this);

    annuler = new QPushButton("annuler", this);
    sauver = new QPushButton("sauver", this);
    sauver->setEnabled(false);

    cred = new QSpinBox(this);
    cred->setRange(1,8);
    cred->setValue(6);

    catego = new QComboBox(this);
    catego->addItem("CS");
    catego->addItem("TM");
    catego->addItem("TSH");
    catego->addItem("SP");

    coucheH1 = new QHBoxLayout;
    coucheH1->addWidget(codeLabel);
    coucheH1->addWidget(code);
    coucheH1->addWidget(catLabel);
    coucheH1->addWidget(catego);
    coucheH1->addWidget(creditLabel);
    coucheH1->addWidget(cred);
    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(titreLabel);
    coucheH2->addWidget(titre);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(ouverture);
    coucheH3->addWidget(automne);
    coucheH3->addWidget(printemps);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(annuler);
    coucheH4->addWidget(sauver);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);
}


UVEditeur::UVEditeur(UV& uvToEdit, QWidget *parent): Editeur(parent), uv(uvToEdit)
{
    this->setWindowTitle(QString("Edition de l' UV ")+uv.getCode());

    code->setText(uv.getCode());
    titre->setPlainText(uv.getTitre());

    automne->setChecked(uv.ouvertureAutomne());
    printemps->setChecked(uv.ouverturePrintemps());

    sauver->setEnabled(false);

    cred->setRange(1,8);
    cred->setValue(uv.getNbCredits());

    catego->setCurrentIndex(int(uv.getCategorie()));

    setLayout(couche);

    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(sauverUV()));
    QObject::connect(code, SIGNAL(textEdited(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(titre, SIGNAL(textChanged()), this, SLOT(activerSauver()));
    QObject::connect(catego, SIGNAL(currentIndexChanged(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(cred, SIGNAL(valueChanged(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(automne, SIGNAL(clicked()), this, SLOT(activerSauver()));
    QObject::connect(printemps, SIGNAL(clicked()), this, SLOT(activerSauver()));
    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
}

void UVEditeur::sauverUV()
{
    uv.setCode(code->text());
    uv.setTitre(titre->toPlainText());
    uv.setNbCredits(cred->value());
    uv.setCategorie(Categorie(catego->currentIndex()));
    uv.setOuverturePrintemps(printemps->isChecked());
    uv.setOuvertureAutomne(automne->isChecked());
    QMessageBox::information(this, "Sauvegarde", "UV sauvegardée ...");
    sauver->setEnabled(false);
}

void UVEditeur::activerSauver(QString)
{
    sauver->setEnabled(true);
}

UVAjout::UVAjout(QWidget* parent): Editeur(parent)
{
    this->setWindowTitle("Ajout d'une UV");
    setLayout(couche);

    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(ajouterUV()));
    QObject::connect(code, SIGNAL(textEdited(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(titre, SIGNAL(textChanged()), this, SLOT(activerSauver()));
    QObject::connect(catego, SIGNAL(currentIndexChanged(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(cred, SIGNAL(valueChanged(QString)), this, SLOT(activerSauver(QString)));
    QObject::connect(automne, SIGNAL(clicked()), this, SLOT(activerSauver()));
    QObject::connect(printemps, SIGNAL(clicked()), this, SLOT(activerSauver()));
    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
}

void UVAjout::ajouterUV()
{
    try {
        UVManager::getInstance().ajouterUV(code->text(), titre->toPlainText(), cred->value(), Categorie(catego->currentIndex()), automne->isChecked(), printemps->isChecked());
        QMessageBox::information(this, "Sauvegarde", "UV sauvegardée ...");
    }
    catch(UTProfilerException& e) {
        QMessageBox::critical(this, "Erreur", e.getInfo());
    }
    sauver->setEnabled(false);
}

void UVAjout::activerSauver(QString)
{
    sauver->setEnabled(true);
}
