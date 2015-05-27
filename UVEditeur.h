/*
 * Interface graphique pour l'édition ou l'ajout d'une UV
*/

#ifndef UVEDITEUR_H
#define UVEDITEUR_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include "uv.h"

//Classe générale possédant les attributs communs aux fenêtres d'édition et d'ajout d'une UV
class Editeur: public QWidget {
protected:
    QVBoxLayout* couche;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QLineEdit* code;
    QTextEdit* titre;
    QLabel *codeLabel, *titreLabel, *ouverture, *catLabel, *creditLabel;
    QCheckBox *automne, *printemps;
    QPushButton *annuler, *sauver;
    QSpinBox *cred;
    QComboBox *catego;

public:
    explicit Editeur(QWidget* parent=0);
};

//Classe pour l'édition d'une UV particulière
class UVEditeur : public Editeur
{
    Q_OBJECT
private:
    UV& uv;
    public slots:
    void sauverUV();  //slot permettant de sauvegarder les modifications effectuées
private slots:
    void activerSauver(QString str="");  //slot privé permettant l'activation du bouton sauver
public:
    explicit UVEditeur(UV& uvToEdit, QWidget *parent=0);  //Transmission de l'UV à éditer dans le constructeur de la classe
};

//Classe pour l'ajout d'une UV
class UVAjout: public Editeur {
    Q_OBJECT
    public slots:
    void ajouterUV();
private slots:
    void activerSauver(QString str="");
public:
    explicit UVAjout(QWidget* parent=0);
};


#endif // UVEDITEUR_H
