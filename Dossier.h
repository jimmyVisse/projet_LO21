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

class NewDossier: public QWidget {
    Q_OBJECT
private:
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
    explicit NewDossier(QWidget* parent=0);
};

class EditDossier: public QWidget {
    Q_OBJECT
private:
    Dossier *d;
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
    explicit EditDossier(Dossier *dToEdit, QWidget* parent=0);
};

#endif // DOSSIER_H
