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
    QPushButton* valider;
    QComboBox* listeUV;
};


#endif // CURSUSEDITEUR_H
