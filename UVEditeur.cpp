#include "UVEditeur.h"

UVEditeur::UVEditeur(UV& uvToEdit, QWidget *parent): QWidget(parent), uv(uvToEdit)
{
    this->setWindowTitle(QString("Edition de l' UV ")+uv.getCode());

    codeLabel = new QLabel("Code: ", this);
    titreLabel = new QLabel("Titre: ", this);
    ouverture = new QLabel("ouverture: ", this);
    catLabel = new QLabel("categorie: ", this);
    creditLabel = new QLabel("Credits: ", this);

    code = new QLineEdit(uv.getCode(), this);
    titre = new QTextEdit(uv.getTitre(), this);

    automne = new QCheckBox("automne", this);
    automne->setChecked(uv.ouvertureAutomne());
    printemps = new QCheckBox("printemps", this);
    printemps->setChecked(uv.ouverturePrintemps());

    annuler = new QPushButton("annuler", this);
    sauver = new QPushButton("sauver", this);
    sauver->setEnabled(false);

    cred = new QSpinBox(this);
    cred->setRange(1,8);
    cred->setValue(uv.getNbCredits());

    catego = new QComboBox(this);
    catego->addItem("CS");
    catego->addItem("TM");
    catego->addItem("TSH");
    catego->addItem("SP");
    catego->setCurrentIndex(int(uv.getCategorie()));

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
