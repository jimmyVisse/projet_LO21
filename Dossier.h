#ifndef DOSSIER_H
#define DOSSIER_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "UTProfiler.h"

class DossierEdit: public QWidget {
    Q_OBJECT
protected:
    QLabel* texte;
    QLabel* nomLabel;
    QLabel* prenomLabel;
    QLabel* cursusLabel;
    QLineEdit* nom;
    QLineEdit* prenom;
    QLineEdit* cursus;
    QPushButton* bouton;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QHBoxLayout* coucheH5;
    QVBoxLayout* couche;
public:
    DossierEdit(QWidget* parent=0);
};


class NewDossier: public DossierEdit {
public:
    explicit NewDossier(QWidget *parent=0);
};

class EditDossier: public DossierEdit {
private:
    Dossier* d;
public:
    explicit EditDossier(Dossier *dToEdit, QWidget* parent=0);
};

#endif // DOSSIER_H
