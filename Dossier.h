#ifndef DOSSIER_H
#define DOSSIER_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include "UTProfiler.h"

class DossierEdit: public QWidget {
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
    Q_OBJECT
public:
    explicit NewDossier(QWidget *parent=0);
public slots:
};

class EditDossier: public DossierEdit {
    Q_OBJECT
private:
    Dossier& d;
    QLabel* cred;
    QPushButton* ajouter;
    QSpinBox* credits;
public:
    explicit EditDossier(Dossier &dToEdit, QWidget* parent=0);
public slots:
    void modifDossier();
    void ajoutIns();
};

class AjoutDossier: public QDialog {
    Q_OBJECT
private:
    QLabel* uvLabel;
    QLabel* noteLabel;
    QLabel* semLabel;
    QLineEdit* uv;
    QLineEdit* note;
    QLineEdit* semestre;
    QPushButton* valider;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QVBoxLayout* couche;
public slots:
    void ajouter();
public:
    explicit AjoutDossier(QWidget* parent=0);
};

#endif // DOSSIER_H
