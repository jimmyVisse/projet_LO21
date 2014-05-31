#ifndef CURSUSEDITEUR_H
#define CURSUSEDITEUR_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include "UTProfiler.h"

class CEditeur: public QWidget {
    Q_OBJECT
public:
    explicit CEditeur(Formation* f, QWidget* parent=0);

private:
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QVBoxLayout* couche;
    Formation* f;
    QLabel* nomLabel;
    QLabel* uvLabel;
    QLineEdit* nom;
    QPushButton* ajouter;
    QPushButton* supprimer;
    QComboBox* listeUV;

public slots:
    void ajoutUV();
    void supprUV();
};

class CAjout: public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QVBoxLayout* couche;
    QLabel* nomLabel;
    QLineEdit* nom;
    QPushButton* ajouter;
public:
    explicit CAjout(QWidget* parent=0);
public slots:
    void ajout();
};

#endif // CURSUSEDITEUR_H
