/*
 * Inteface graphique autour du dossier
*/

#ifndef DOSSIERUI_H
#define DOSSIERUI_H

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
#include <QFileDialog>
#include <QComboBox>
#include "dossier.h"

//Class emère permettant de centraliser les attributs communs
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

//Classe pour créer un nouveau dossier
class NewDossier: public DossierEdit {
    Q_OBJECT
private:
    QString chemin;
    QLabel* semestre;
    QSpinBox* nbSem;
public:
    explicit NewDossier(QWidget *parent=0);
public slots:
    void creationDossier();
};

//Classe pour l'édition d'un dossier spécifique transmis dans le constructeur
class EditDossier: public DossierEdit {
    Q_OBJECT
private:
    Dossier* d;
    QLabel* cred;
    QPushButton* ajouter;
    QSpinBox* credits;
    QLabel* insc_label;
    QComboBox* inscriptions;
public:
    explicit EditDossier(Dossier *dToEdit, QWidget* parent=0);
    ~EditDossier();
public slots:
    void modifDossier();
    void ajoutIns();
};


/* Classe permettant d'afficher une fenêtre de dialogue pour insérer une inscription depuis l'éditeur de dossier */
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


/* Classe permettant d'ajouter une UV depuis l'onglet dans la fenêtre */
class AjouterIns: public QWidget {
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
    explicit AjouterIns(QWidget* parent=0);
};

#endif // DOSSIERUI_H
